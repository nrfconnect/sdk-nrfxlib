/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef SOFTPERIPHERAL_REGIF_H__
#define SOFTPERIPHERAL_REGIF_H__

/* Shared between Host and Service, varies between platforms. */
#if defined (NRF54L05_XXAA) || defined (NRF54L09_ENGA_XXAA) || defined (NRF54L10_XXAA) || \
    defined (NRF54L15_XXAA) || defined (NRF54LM20A_ENGA_XXAA) ||                          \
    defined (NRF54LV10A_XXAA) || defined (NRF54LM20A_XXAA) || defined (NRF54LM20B_XXAA)
#define SP_VPR_EVENT_IDX       20
#define NRF_VPR                NRF_VPR00
/* Tasks 16..19 are the only ones the hardware wires to DPPIC00 (channel = index - 16), so they are
 * the DPPI slots. Tasks 20..22 are triggerable and raise interrupts but have no DPPI path, which is
 * where the barriers that must never be reachable from DPPI live. */
#define SP_VPR_TASK_DPPI_0_IDX 16 // DPPIC00 Channel 0 - slot 0
#define SP_VPR_TASK_SLOT1_IDX  17 // DPPIC00 Channel 1 - slot 1
#define SP_VPR_TASK_STOP_IDX   18 // DPPIC00 Channel 2 - slot 2
#define SP_VPR_TASK_SLOT3_IDX  19 // DPPIC00 Channel 3 - slot 3
#define SP_VPR_TASK_CONFIG_IDX 20 // no DPPI channel
#define SP_VPR_TASK_ACTION_IDX 21 // no DPPI channel
#define SP_VPR_IRQHandler      VPR00_IRQHandler
#define SP_VPR_IRQn            VPR00_IRQn
#define SP_VPR_BASE_FREQ_HZ    128000000
#elif defined(NRF54H20_XXAA)
#define SP_VPR_EVENT_IDX       28
#define NRF_VPR                NRF_VPR121
/* Tasks 24..27 are the only ones the hardware wires to DPPIC120, so they are the DPPI slots. Tasks
 * 16..23 are triggerable and raise interrupts but have no DPPI path - see the L series note. */
#define SP_VPR_TASK_DPPI_0_IDX 24 // DPPIC120 Channel 4 - slot 0
#define SP_VPR_TASK_SLOT1_IDX  25 // DPPIC120 Channel 5 - slot 1
#define SP_VPR_TASK_STOP_IDX   26 // DPPIC120 Channel 6 - slot 2
#define SP_VPR_TASK_SLOT3_IDX  27 // DPPIC120 Channel 7 - slot 3
#define SP_VPR_TASK_CONFIG_IDX 16 // no DPPI channel
#define SP_VPR_TASK_ACTION_IDX 17 // no DPPI channel
#define SP_VPR_IRQHandler      VPR121_IRQHandler
#define SP_VPR_IRQn            VPR121_IRQn
#define SP_VPR_BASE_FREQ_HZ    320000000
#else
#pragma warning "Processor not defined."
#endif

/* Task/role map, carried in the SPSYNC.DPPIMAP register of each soft peripheral's
 * register interface, where it is documented field by field and where its reset value defines the
 * safe default map.
 *
 * The definitions below mirror that register so host and service can pack and unpack it without
 * depending on a particular peripheral's header. Keep them in step with register interface header
 * file (for example, for sQSPI: nrf_sp_qspi.h).
 *
 * The host writes the register and raises the config barrier; the service applies it while parsing
 * the configuration.
 *
 * The soft peripheral owns SP_TASK_ENTRY_COUNT VEVIF tasks, addressed here as entries:
 *   entry 0..3  the DPPI slots - the tasks the hardware wires to a DPPI channel, entry n is
 *               channel n of the SoC's DPPI instance
 *   entry 4..5  tasks with no DPPI path at all, so nothing on the DPPI fabric can reach them
 *
 * Two independent fields:
 *   ROLE[entry] which handler that task invokes (allocation)
 *   PERMIT      whether the service may subscribe that slot's DPPI channel (enabling, slots only)
 *
 * A role lives on exactly one task: both a host task-register write and a DPPI event for that role
 * end up in the same handler. Moving a role therefore moves everything that reaches it, including
 * the barrier the host uses - see sp_dppi_role_task_update() below.
 *
 * VALID must read as SP_DPPI_MAP_VALID_Value or the whole word is ignored, so a cleared register
 * can never re-bind a role or permit a subscription.
 */
#define SP_DPPI_SLOT_COUNT          4
#define SP_TASK_ENTRY_COUNT         6
#define SP_TASK_ENTRY_NODPPI        4 /* first entry without a DPPI channel */

#define SP_DPPI_MAP_VALID_Pos       0UL
#define SP_DPPI_MAP_VALID_Msk       (0xFUL << SP_DPPI_MAP_VALID_Pos)
#define SP_DPPI_MAP_VALID_Value     0x5UL

#define SP_DPPI_MAP_ROLE_Pos(entry) (4UL + (4UL * (entry)))
#define SP_DPPI_MAP_ROLE_Msk(entry) (0xFUL << SP_DPPI_MAP_ROLE_Pos(entry))

#define SP_DPPI_MAP_PERMIT_Pos      28UL
#define SP_DPPI_MAP_PERMIT_Msk      (0xFUL << SP_DPPI_MAP_PERMIT_Pos)

/* VPR task index of an entry. Entries 0..3 are consecutive from the first slot. */
#define SP_VPR_TASK_IDX(entry)                                           \
    (((entry) < SP_DPPI_SLOT_COUNT) ? (SP_VPR_TASK_DPPI_0_IDX + (entry)) \
     : (((entry) == SP_TASK_ENTRY_NODPPI) ? SP_VPR_TASK_CONFIG_IDX       \
                                          : SP_VPR_TASK_ACTION_IDX))

/* Role values. Must stay in sync with the service side. */
#define SP_DPPI_ROLE_NONE   0UL /* task dispatches nothing */
#define SP_DPPI_ROLE_DPPI_0 1UL /* green0_fsm_handle_dppi_0 - start a prepared transfer */
#define SP_DPPI_ROLE_CONFIG 2UL /* green0_fsm_handle_config - target of __CSB */
#define SP_DPPI_ROLE_ACTION 3UL /* green0_fsm_handle_action - target of __ASB */
#define SP_DPPI_ROLE_STOP   4UL /* green0_fsm_handle_stop   - target of __SSB */
#define SP_DPPI_ROLE_COUNT  5UL

/* The reset value of SPSYNC.DPPIMAP, repeated here because the service needs it before it has read
 * the register and the host needs it as the base for a partial change: channel 0 starts a prepared
 * request, channel 2 stops it, and the two channels in between dispatch nothing - those are the
 * ones other subsystems (MPSL/SDC) drive. Config and action sit on tasks with no DPPI path.
 * Nothing is permitted. Must equal <SP>_SPSYNC_DPPIMAP_ResetValue. */
#define SP_DPPI_MAP_DEFAULT                             \
    ((SP_DPPI_MAP_VALID_Value << SP_DPPI_MAP_VALID_Pos) \
     | (SP_DPPI_ROLE_DPPI_0 << SP_DPPI_MAP_ROLE_Pos(0)) \
     | (SP_DPPI_ROLE_NONE << SP_DPPI_MAP_ROLE_Pos(1))   \
     | (SP_DPPI_ROLE_STOP << SP_DPPI_MAP_ROLE_Pos(2))   \
     | (SP_DPPI_ROLE_NONE << SP_DPPI_MAP_ROLE_Pos(3))   \
     | (SP_DPPI_ROLE_CONFIG << SP_DPPI_MAP_ROLE_Pos(4)) \
     | (SP_DPPI_ROLE_ACTION << SP_DPPI_MAP_ROLE_Pos(5)))

/* The wiring soft peripherals used before: every role on a DPPI-capable slot, so all four
 * channels reach the peripheral. Reachable through the API for applications that own those
 * channels and want the old behaviour back; combine with SP_DPPI_MAP_PERMIT_ALL. */
#define SP_DPPI_MAP_LEGACY                              \
    ((SP_DPPI_MAP_VALID_Value << SP_DPPI_MAP_VALID_Pos) \
     | (SP_DPPI_ROLE_DPPI_0 << SP_DPPI_MAP_ROLE_Pos(0)) \
     | (SP_DPPI_ROLE_CONFIG << SP_DPPI_MAP_ROLE_Pos(1)) \
     | (SP_DPPI_ROLE_ACTION << SP_DPPI_MAP_ROLE_Pos(2)) \
     | (SP_DPPI_ROLE_STOP << SP_DPPI_MAP_ROLE_Pos(3))   \
     | (SP_DPPI_ROLE_NONE << SP_DPPI_MAP_ROLE_Pos(4))   \
     | (SP_DPPI_ROLE_NONE << SP_DPPI_MAP_ROLE_Pos(5)))

#define SP_DPPI_MAP_PERMIT_ALL (0xFUL << SP_DPPI_MAP_PERMIT_Pos)

#define SP_DPPI_MAP_ROLE_GET(word, slot) \
    (((word) & SP_DPPI_MAP_ROLE_Msk(slot)) >> SP_DPPI_MAP_ROLE_Pos(slot))

#define SP_DPPI_MAP_PERMIT_GET(word) \
    (((word) & SP_DPPI_MAP_PERMIT_Msk) >> SP_DPPI_MAP_PERMIT_Pos)

#if defined(NRF_APPLICATION)

#ifndef SP_VPR_FIRMWARE_ADDRESS
#define SP_VPR_FIRMWARE_ADDRESS 0x00040000
#endif

/* Config Synchronization Barrier (ASB). */
#if 1
#define __XSBx(R, P, T)                                                                    \
    do {                                                                                   \
        sp_handshake_set(R, m_task_count, 0);                                              \
        nrf_vpr_task_trigger(P, (nrf_vpr_task_t)offsetof(NRF_VPR_Type, TASKS_TRIGGER[T])); \
        while (sp_handshake_get(R, 0) != sp_handshake_get(R, 1)) {                         \
           __NOP();                                                                        \
           __NOP();                                                                        \
           __NOP();                                                                        \
        }                                                                                  \
        m_task_count++;                                                                    \
    } while (0);
#else
#define __XSBx(R, P, T)                                                                    \
    do {                                                                                   \
        nrf_vpr_task_trigger(P, (nrf_vpr_task_t)offsetof(NRF_VPR_Type, TASKS_TRIGGER[T])); \
    } while (0);
#endif

/* Task index per role. The barriers below trigger the task that currently carries the role, so a
 * role that the application moves to another slot keeps working - the service dispatches by role,
 * the host triggers by role. Defaults are the dedicated tasks: config and action have no DPPI
 * channel at all, stop sits on its slot. sp_dppi_role_task_update() refreshes this whenever a new
 * map is published; until then the defaults apply. */
static uint8_t m_sp_role_task[SP_DPPI_ROLE_COUNT] __attribute__((unused)) =
{
    [SP_DPPI_ROLE_NONE]   = SP_VPR_TASK_CONFIG_IDX, /* never triggered */
    [SP_DPPI_ROLE_DPPI_0] = SP_VPR_TASK_DPPI_0_IDX,
    [SP_DPPI_ROLE_CONFIG] = SP_VPR_TASK_CONFIG_IDX,
    [SP_DPPI_ROLE_ACTION] = SP_VPR_TASK_ACTION_IDX,
    [SP_DPPI_ROLE_STOP]   = SP_VPR_TASK_STOP_IDX,
};

/* Helpers for the DPPI map word (see the field definitions above). A word that does not carry the
 * VALID pattern is treated as "not written yet" and replaced by the default map, so an application
 * that only wants to change one field does not have to know the rest. */
static inline uint32_t sp_dppi_map_normalize(uint32_t word)
{
    return (((word & SP_DPPI_MAP_VALID_Msk) >> SP_DPPI_MAP_VALID_Pos) == SP_DPPI_MAP_VALID_Value)
           ? word
           : (uint32_t)SP_DPPI_MAP_DEFAULT;
}

static inline void sp_dppi_role_task_update(uint32_t map_word)
{
    map_word = sp_dppi_map_normalize(map_word);

    for (uint8_t entry = 0; entry < SP_TASK_ENTRY_COUNT; entry++)
    {
        uint32_t role = SP_DPPI_MAP_ROLE_GET(map_word, entry);

        if (role != SP_DPPI_ROLE_NONE)
        {
            m_sp_role_task[role] = (uint8_t)SP_VPR_TASK_IDX(entry);
        }
    }
}

#define __CSB(R) __XSBx(R, NRF_VPR, m_sp_role_task[SP_DPPI_ROLE_CONFIG]);
#define __ASB(R) __XSBx(R, NRF_VPR, m_sp_role_task[SP_DPPI_ROLE_ACTION]);
#define __SSB(R) __XSBx(R, NRF_VPR, m_sp_role_task[SP_DPPI_ROLE_STOP]);

/* Pack a complete role map into a map word, keeping the permission bits that are already there.
 * p_roles has one SP_DPPI_ROLE_* per task entry, index 0..SP_TASK_ENTRY_COUNT-1. Returns false and
 * leaves *p_word alone if a role is out of range or claimed by more than one entry - a role has to
 * resolve to a single task, otherwise a task register write and a DPPI event for it would end up in
 * different handlers. */
static inline bool sp_dppi_role_map_pack(uint32_t * p_word, const uint8_t * p_roles)
{
    uint32_t word = sp_dppi_map_normalize(*p_word) & SP_DPPI_MAP_PERMIT_Msk;
    uint8_t  seen = 0;

    for (uint8_t entry = 0; entry < SP_TASK_ENTRY_COUNT; entry++)
    {
        uint8_t role = p_roles[entry];

        if (role >= SP_DPPI_ROLE_COUNT)
        {
            return false;
        }
        if (role != SP_DPPI_ROLE_NONE)
        {
            if ((seen & (1U << role)) != 0U)
            {
                return false;
            }
            seen |= (uint8_t)(1U << role);
        }
        word |= ((uint32_t)role << SP_DPPI_MAP_ROLE_Pos(entry));
    }

    *p_word = word | (SP_DPPI_MAP_VALID_Value << SP_DPPI_MAP_VALID_Pos);
    return true;
}

static inline uint32_t sp_dppi_map_permit_set(uint32_t word, uint8_t slot, bool enable)
{
    uint32_t bit = 1UL << (SP_DPPI_MAP_PERMIT_Pos + slot);

    word = sp_dppi_map_normalize(word);
    return enable ? (word | bit) : (word & ~bit);
}

#endif // NRF_APPLICATION
#endif // SOFTPERIPHERAL_REGIF_H__
