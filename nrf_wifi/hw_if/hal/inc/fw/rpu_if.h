/** @file rpu_if.h
 * @brief Common structures and definitions for RPU interface.
 */

#ifndef __RPU_IF_H__
#define __RPU_IF_H__
#include "pack_def.h"

#define RPU_ADDR_GRAM_START 0xB7000000
#define RPU_ADDR_GRAM_END 0xB70101FF
#define RPU_ADDR_SBUS_START 0xA4000000
#define RPU_ADDR_SBUS_END 0xA4007FFF
#define RPU_ADDR_PBUS_START 0xA5000000
#define RPU_ADDR_PBUS_END 0xA503FFFF
#define RPU_ADDR_BEV_START 0xBFC00000
#define RPU_ADDR_BEV_END 0xBFCFFFFF
#define RPU_ADDR_PKTRAM_START 0xB0000000
#define RPU_ADDR_PKTRAM_END 0xB0030FFF

/* Needed for calculatating sleep controller address */
#define RPU_ADDR_LMAC_CORE_RET_START 0x80040000
#define RPU_ADDR_UMAC_CORE_RET_START 0x80080000

enum RPU_MCU_ADDR_REGIONS {
	RPU_MCU_ADDR_REGION_ROM = 0,
	RPU_MCU_ADDR_REGION_RETENTION,
	RPU_MCU_ADDR_REGION_SCRATCH,
	RPU_MCU_ADDR_REGION_MAX,
};

struct rpu_addr_region {
	unsigned int start;
	unsigned int end;
};

struct rpu_addr_map {
	struct rpu_addr_region regions[RPU_MCU_ADDR_REGION_MAX];
};

static const struct rpu_addr_map RPU_ADDR_MAP_MCU[] = {
	/* MCU1 - LMAC */
	{
		{
			{0x80000000, 0x80033FFF},
			{0x80040000, 0x8004BFFF},
			{0x80080000, 0x8008FFFF}
		},
	},
	/* MCU2 - UMAC */
	{
		{
			{0x80000000, 0x800617FF},
			{0x80080000, 0x800A3FFF},
			{0x80100000, 0x80137FFF},
		}
	},
};

#define RPU_MCU_MAX_BOOT_VECTORS 4
struct rpu_mcu_boot_vector {
	unsigned int addr;
	unsigned int val;
};

struct rpu_mcu_boot_vectors {
	struct rpu_mcu_boot_vector vectors[RPU_MCU_MAX_BOOT_VECTORS];
};

#define RPU_ADDR_MASK_BASE 0xFF000000
#define RPU_ADDR_MASK_OFFSET 0x00FFFFFF
#define RPU_ADDR_MASK_BEV_OFFSET 0x000FFFFF

#define RPU_REG_INT_FROM_RPU_CTRL 0xA4000400
#define RPU_REG_BIT_INT_FROM_RPU_CTRL 17

#define RPU_REG_INT_TO_MCU_CTRL 0xA4000480

#define RPU_REG_INT_FROM_MCU_ACK 0xA4000488
#define RPU_REG_BIT_INT_FROM_MCU_ACK 31

#define RPU_REG_INT_FROM_MCU_CTRL 0xA4000494
#define RPU_REG_BIT_INT_FROM_MCU_CTRL 31

#define RPU_REG_UCC_SLEEP_CTRL_DATA_0 0xA4002C2C
#define RPU_REG_UCC_SLEEP_CTRL_DATA_1 0xA4002C30
#define RPU_REG_MIPS_MCU_CONTROL 0xA4000000
#define RPU_REG_MIPS_MCU2_CONTROL 0xA4000100

#define RPU_REG_MIPS_MCU_UCCP_INT_STATUS 0xA4000004
#define RPU_REG_BIT_MIPS_WATCHDOG_INT_STATUS 1

#define RPU_REG_MIPS_MCU_TIMER 0xA400004C /* 24 bit timer@core clock ticks*/
#define RPU_REG_MIPS_MCU_TIMER_RESET_VAL 0xFFFFFF

#define RPU_REG_MIPS_MCU_UCCP_INT_CLEAR 0xA400000C
#define RPU_REG_BIT_MIPS_WATCHDOG_INT_CLEAR 1

#define RPU_REG_MIPS_MCU_SYS_CORE_MEM_CTRL 0xA4000030
#define RPU_REG_MIPS_MCU_SYS_CORE_MEM_WDATA 0xA4000034
#define RPU_REG_MIPS_MCU_BOOT_EXCP_INSTR_0 0xA4000050
#define RPU_REG_MIPS_MCU_BOOT_EXCP_INSTR_1 0xA4000054
#define RPU_REG_MIPS_MCU_BOOT_EXCP_INSTR_2 0xA4000058
#define RPU_REG_MIPS_MCU_BOOT_EXCP_INSTR_3 0xA400005C

#define RPU_REG_MIPS_MCU2_SYS_CORE_MEM_CTRL 0xA4000130
#define RPU_REG_MIPS_MCU2_SYS_CORE_MEM_WDATA 0xA4000134
#define RPU_REG_MIPS_MCU2_BOOT_EXCP_INSTR_0 0xA4000150
#define RPU_REG_MIPS_MCU2_BOOT_EXCP_INSTR_1 0xA4000154
#define RPU_REG_MIPS_MCU2_BOOT_EXCP_INSTR_2 0xA4000158
#define RPU_REG_MIPS_MCU2_BOOT_EXCP_INSTR_3 0xA400015C

#define RPU_REG_BIT_PS_STATE 1
#define RPU_REG_BIT_READY_STATE 2
#define RPU_MEM_RX_CMD_BASE 0xB7000D58

#define RPU_MEM_HPQ_INFO 0xB0000024
#define RPU_MEM_TX_CMD_BASE 0xB00000B8
#define RPU_MEM_OTP_FT_PROG_VERSION 0xB0004FD8
#define RPU_MEM_OTP_INFO_FLAGS 0xB0004FDC
#define RPU_MEM_OTP_PACKAGE_TYPE 0xB0004FD4

#define RPU_MEM_PKT_BASE 0xB0005000
#define RPU_CMD_START_MAGIC 0xDEAD
#define RPU_DATA_CMD_SIZE_MAX_RX 8
#define RPU_DATA_CMD_SIZE_MAX_TX 148
#define RPU_EVENT_COMMON_SIZE_MAX 128

#define MAX_EVENT_POOL_LEN 1000
#define MAX_NUM_OF_RX_QUEUES 3

#define RPU_PKTRAM_SIZE (RPU_ADDR_PKTRAM_END - RPU_MEM_PKT_BASE + 1)

#ifdef CONFIG_NRF700X_RADIO_TEST
#define RPU_MEM_RF_TEST_CAP_BASE 0xB0006000
#endif /* CONFIG_NRF700X_RADIO_TEST */

/* REGION PROTECT : OTP Address offsets (word offsets) */
#define REGION_PROTECT 64
#define PRODTEST_FT_PROGVERSION 29
#define PRODTEST_TRIM0 32
#define PRODTEST_TRIM1 33
#define PRODTEST_TRIM2 34
#define PRODTEST_TRIM3 35
#define PRODTEST_TRIM4 36
#define PRODTEST_TRIM5 37
#define PRODTEST_TRIM6 38
#define PRODTEST_TRIM7 39
#define PRODTEST_TRIM8 40
#define PRODTEST_TRIM9 41
#define PRODTEST_TRIM10 42
#define PRODTEST_TRIM11 43
#define PRODTEST_TRIM12 44
#define PRODTEST_TRIM13 45
#define PRODTEST_TRIM14 46
#define INFO_PART 48
#define INFO_VARIANT 49
#define INFO_UUID 52
#define QSPI_KEY 68
#define MAC0_ADDR 72
#define MAC1_ADDR 74
#define CALIB_XO 76
#define REGION_DEFAULTS 85
#define PRODRETEST_PROGVERSION 86
#define PRODRETEST_TRIM0 87
#define PRODRETEST_TRIM1 88
#define PRODRETEST_TRIM2 89
#define PRODRETEST_TRIM3 90
#define PRODRETEST_TRIM4 91
#define PRODRETEST_TRIM5 92
#define PRODRETEST_TRIM6 93
#define PRODRETEST_TRIM7 94
#define PRODRETEST_TRIM8 95
#define PRODRETEST_TRIM9 96
#define PRODRETEST_TRIM10 97
#define PRODRETEST_TRIM11 98
#define PRODRETEST_TRIM12 99
#define PRODRETEST_TRIM13 100
#define PRODRETEST_TRIM14 101
#define OTP_MAX_WORD_LEN 128
#define QSPI_KEY_LENGTH_BYTES 16
#define RETRIM_LEN 15

/* Size of OTP fields in bytes */
#define OTP_SZ_CALIB_XO 1

/* Offsets of OTP calib values in the calib field */
#define OTP_OFF_CALIB_XO 0

/* MASKS to program bit fields in REGION_DEFAULTS register */
#define QSPI_KEY_FLAG_MASK ~(1U<<0)
#define MAC0_ADDR_FLAG_MASK ~(1U<<1)
#define MAC1_ADDR_FLAG_MASK ~(1U<<2)
#define CALIB_XO_FLAG_MASK ~(1U<<3)

/* OTP Device address definitions */
#define OTP_VOLTCTRL_ADDR 0x19004
#define OTP_VOLTCTRL_2V5 0x3b
#define OTP_VOLTCTRL_1V8 0xb

#define OTP_POLL_ADDR 0x01B804
#define OTP_WR_DONE 0x1
#define OTP_READ_VALID 0x2
#define OTP_READY 0x4


#define OTP_RWSBMODE_ADDR 0x01B800
#define OTP_READ_MODE 0x1
#define OTP_BYTE_WRITE_MODE 0x42


#define OTP_RDENABLE_ADDR 0x01B810
#define OTP_READREG_ADDR 0x01B814

#define OTP_WRENABLE_ADDR 0x01B808
#define OTP_WRITEREG_ADDR 0x01B80C

#define OTP_TIMING_REG1_ADDR 0x01B820
#define OTP_TIMING_REG1_VAL 0x0
#define OTP_TIMING_REG2_ADDR 0x01B824
#define OTP_TIMING_REG2_VAL 0x030D8B

#define OTP_FRESH_FROM_FAB 0xFFFFFFFF
#define OTP_PROGRAMMED 0x00000000
#define OTP_ENABLE_PATTERN 0x50FA50FA
#define OTP_INVALID 0xDEADBEEF

#define FT_PROG_VER_MASK 0xF0000

/**
 * @brief Data that host may want to read from the Power IP.
 * This structure represents the Power IP monitoring data.
 */
struct nrf_wifi_rpu_pwr_data {
	/** Estimated Lo Frequency Clock error in ppm. */
	int lfc_err;
	/** Vbat monitor readout. The actual Vbat in volt equals 2.5 + 0.07*vbat_mon. */
	int vbat_mon;
	/** Estimated die temperature (degC). */
	int temp;
};

/**
 * @brief RX buffer related information to be passed to he RPU.
 *
 * This structure encapsulates the information to be passed to the RPU for
 * buffers which the RPU will use to pass the received frames.
 */

struct host_rpu_rx_buf_info {
	/** Address in the host memory where the RX buffer is located. */
	unsigned int addr;
} __NRF_WIFI_PKD;

/**
 * @brief Hostport Queue (HPQ) information.

 * This structure encapsulates the information which represents a HPQ.
 */

struct host_rpu_hpq {
	/** HPQ address where the host can post the address of a message intended for the RPU. */
	unsigned int enqueue_addr;
	/** HPQ address where the host can get the address of a message intended for the host. */
	unsigned int dequeue_addr;
} __NRF_WIFI_PKD;

/**
 * @brief Information about Hostport Queues (HPQ) to be used
 *	for exchanging information between the Host and RPU.
 *
 * Hostport queue information passed by the RPU to the host, which the host can
 * use, to communicate with the RPU.
 */

struct host_rpu_hpqm_info {
	/** Queue which the RPU uses to inform the host about events. */
	struct host_rpu_hpq event_busy_queue;
	/** Queue on which the consumed events are pushed so that RPU can reuse them. */
	struct host_rpu_hpq event_avl_queue;
	/** Queue used by the host to push commands to the RPU. */
	struct host_rpu_hpq cmd_busy_queue;
	/** Queue which RPU uses to inform host about command buffers which can be used to
	 * push commands to the RPU.
	 */
	struct host_rpu_hpq cmd_avl_queue;
	/** Queue used by the host to push RX buffers to the RPU. */
	struct host_rpu_hpq rx_buf_busy_queue[MAX_NUM_OF_RX_QUEUES];
} __NRF_WIFI_PKD;

/**
 * @brief Common header included in each command/event.
 * This structure encapsulates the common information included at the start of
 * each command/event exchanged with the RPU.
 */

struct host_rpu_msg_hdr {
	/** Length of the message. */
	unsigned int len;
	/** Flag to indicate whether the recipient is expected to resubmit the
	 * cmd/event address back to the trasmitting entity.
	 */
	unsigned int resubmit;
} __NRF_WIFI_PKD;

#endif /* __RPU_IF_H__ */
