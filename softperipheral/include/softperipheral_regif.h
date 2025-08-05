/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef SOFTPERIPHERAL_REGIF_H__
#define SOFTPERIPHERAL_REGIF_H__

/* Shared between Host and Service, varies between platforms. */
#if defined (NRF54L05_XXAA) || defined (NRF54L09_ENGA_XXAA) || defined (NRF54L10_XXAA) || \
    defined (NRF54L15_XXAA)
#define SP_VPR_EVENT_IDX       20
#define NRF_VPR                NRF_VPR00
#define SP_VPR_TASK_DPPI_0_IDX 16 // Channel 0
#define SP_VPR_TASK_CONFIG_IDX 17
#define SP_VPR_TASK_ACTION_IDX 18
#define SP_VPR_TASK_STOP_IDX   19
#define SP_VPR_IRQHandler      VPR00_IRQHandler
#define SP_VPR_IRQn            VPR00_IRQn
#define SP_VPR_BASE_FREQ_HZ    128000000
#elif defined (NRF54L20_ENGA_XXAA) || defined (NRF54LM20A_ENGA_XXAA)
#define SP_VPR_EVENT_IDX       20
#define NRF_VPR                NRF_VPR00
#define SP_VPR_TASK_DPPI_0_IDX 16 // Channel 0
#define SP_VPR_TASK_CONFIG_IDX 17
#define SP_VPR_TASK_ACTION_IDX 18
#define SP_VPR_TASK_STOP_IDX   19
#define SP_EGU_TASK_CONFIG_IDX 1
#define SP_EGU_TASK_ACTION_IDX 2
#define SP_EGU_TASK_STOP_IDX   3
#define SP_VPR_IRQHandler      VPR00_IRQHandler
#define SP_VPR_IRQn            VPR00_IRQn
#ifndef SP_EGU_PERIPH
#define SP_EGU_PERIPH          NRF_EGU00
#endif // SP_EGU_PERIPH
#define SP_VPR_BASE_FREQ_HZ    128000000
#elif defined(NRF54H20_XXAA)
#define SP_VPR_EVENT_IDX       28
#define NRF_VPR                NRF_VPR121
#define SP_VPR_TASK_DPPI_0_IDX 24 // Channel 0
#define SP_VPR_TASK_CONFIG_IDX 25
#define SP_VPR_TASK_ACTION_IDX 26
#define SP_VPR_TASK_STOP_IDX   27
#define SP_VPR_IRQHandler      VPR121_IRQHandler
#define SP_VPR_IRQn            VPR121_IRQn
#define SP_VPR_BASE_FREQ_HZ    320000000
#else
#pragma warning "Processor not defined."
#endif

#if defined(NRF_APPLICATION)

#ifndef SP_VPR_FIRMWARE_ADDRESS
#define SP_VPR_FIRMWARE_ADDRESS 0x00040000
#endif

#if defined (NRF54L20_ENGA_XXAA) || defined (NRF54LM20A_ENGA_XXAA)

/* Config Synchronization Barrier (ASB). */
#if 1
#define __XSBx(R, P, T)                                                                    \
    do {                                                                                   \
        nrf_qspi2_core_dr_x(R, m_task_count, 20);                                          \
        nrf_egu_task_trigger(P, (nrf_egu_task_t)offsetof(NRF_EGU_Type, TASKS_TRIGGER[T])); \
        while (nrf_qspi2_core_dr_x_get(R, 20) != nrf_qspi2_core_dr_x_get(R, 21)) {         \
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

#define __CSB(R) __XSBx(R, SP_EGU_PERIPH, SP_EGU_TASK_CONFIG_IDX);
#define __ASB(R) __XSBx(R, SP_EGU_PERIPH, SP_EGU_TASK_ACTION_IDX);
#define __SSB(R) __XSBx(R, SP_EGU_PERIPH, SP_EGU_TASK_STOP_IDX);

#else // # !defined (NRF54L20_ENGA_XXAA) &&  !defined (NRF54LM20A_ENGA_XXAA)

/* Config Synchronization Barrier (ASB). */
#if 1
#define __XSBx(R, P, T)                                                                    \
    do {                                                                                   \
        nrf_qspi2_core_dr_x(R, m_task_count, 20);                                          \
        nrf_vpr_task_trigger(P, (nrf_vpr_task_t)offsetof(NRF_VPR_Type, TASKS_TRIGGER[T])); \
        while (nrf_qspi2_core_dr_x_get(R, 20) != nrf_qspi2_core_dr_x_get(R, 21)) {         \
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

#define __CSB(R) __XSBx(R, NRF_VPR, SP_VPR_TASK_CONFIG_IDX);
#define __ASB(R) __XSBx(R, NRF_VPR, SP_VPR_TASK_ACTION_IDX);
#define __SSB(R) __XSBx(R, NRF_VPR, SP_VPR_TASK_STOP_IDX);

#endif // if defined (NRF54L20_ENGA_XXAA) || defined (NRF54LM20A_ENGA_XXAA)
#endif // NRF_APPLICATION
#endif // SOFTPERIPHERAL_REGIF_H__
