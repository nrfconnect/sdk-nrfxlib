/*
 * ZBOSS Zigbee 3.0
 *
 * Copyright (c) 2012-2020 DSR Corporation, Denver CO, USA.
 * http://www.dsr-zboss.com
 * http://www.dsr-corporation.com
 * All rights reserved.
 *
 *
 * Use in source and binary forms, redistribution in binary form only, with
 * or without modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 2. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 3. This software, with or without modification, must only be used with a Nordic
 *    Semiconductor ASA integrated circuit.
 *
 * 4. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/* PURPOSE: Configuration file: configuration- and platform-specific definitions,
constants etc.
*/

#ifndef ZB_CONFIG_H
#define ZB_CONFIG_H 1

/**
   Load vendor specific configurations
*/

/* \defgroup buildconfig Build Configurations
   @{ */
#include "zb_vendor.h"
/**
   Load common ZBOSS definitions
*/
#include "zb_config_common.h"

/*
 * ZC/ZR - GPPB role library     (default) (implementation of Basic Proxy is mandatory for Zigbee 3.0 ZR)
 * ZC/ZR - GPCB role library               (Options file shall use ZB_ENABLE_ZGP_COMBO define)
 * ZC/ZR - GP Commissioning Tool           (Options file shall use ZGP_COMMISSIONING_TOOL define)
 * ZED   - Target role library   (default) (Options file shall use ZB_ENABLE_ZGP_TARGET define)
 * ZED   - Target+ role library            (Options file shall use ZB_ENABLE_ZGP_TARGET_PLUS define)
 *
 * To enable advanced features (not covered by the current version of GP specification) please
 * define ZB_ENABLE_ZGP_ADVANCED, so in this case the GPPB/GPCB/GPCT basic libs will be re-compiled
 * to GPP/GPC/GPCT advanced libs
 *
 * GP infrastructure devices:
 *
 *   A.3.2.1 GP Target device ability:
 *    - ability to receive any GP frame in tunneled mode
 *
 *   A.3.2.2 GP Target+ device ability:
 *    - ability to receive any GP frame both in direct mode and in tunneled mode
 *
 *   A.3.2.3 GP Proxy device ability:
 *    - ability to receive any GP frame in direct mode when the proxy is in the radio range of the
 *      GPD
 *    - ability to send to the registered sink devices a GP Notification command with the received
 *      GP frame
 *    - ability to maintain a Proxy Table at commissioning time to register sink devices which are
 *      asking for GP frame forwarding service
 *    - ability to update the Proxy Table based on the observed GP traffic in order to enable GP
 *      device mobility in the network
 *    - ability to drop scheduled tunneling of GP frame, based on received GP commands related to
 *      the same GP frame
 *
 *   A.3.2.4 GP Combo device
 *    - ability to receive any GP frame both in direct mode and in tunneled mode
 *
 * For migration old sink dataset use ZB_ENABLE_ZGP_MIGRATE_OLD_SINK_DATASET define
 */

#ifdef ZB_ZGPD_ROLE
#define ZB_ENABLE_ZGP_DIRECT
#define ZB_ENABLE_ZGP_SECUR
#define APP_ONLY_NVRAM
#elif defined ZB_ENABLE_ZGP && !defined ZB_ENABLE_ZGP_TARGET && defined ZB_ED_ROLE
#undef ZB_ENABLE_ZGP
#endif


#if defined ZB_ENABLE_ZGP && !defined ZB_ZGPD_ROLE

/*
#define ZB_ENABLE_ZGP_ADVANCED
*/

#define ZB_ENABLE_ZGP_CLUSTER
#define ZB_ENABLE_ZGP_SECUR
#define ZB_USEALIAS

#ifndef ZB_ED_ROLE
/* ZED can only be implemented as GP Target/Target+ */
#define ZB_ENABLE_ZGP_PROXY
#endif

#ifndef ZB_ENABLE_ZGP_TARGET
#define ZB_ENABLE_ZGP_DIRECT
#define ZB_ENABLE_ZGP_TX_QUEUE
#endif

/** Maximum payload length in translation table entry */
#ifndef ZB_ZGP_TRANSL_CMD_PLD_MAX_SIZE
#define ZB_ZGP_TRANSL_CMD_PLD_MAX_SIZE  3
#endif

#if (defined ZB_ENABLE_ZGP_COMBO || defined ZB_ENABLE_ZGP_TARGET || defined ZB_ENABLE_ZGP_TARGET_PLUS || defined ZGP_COMMISSIONING_TOOL)
#define ZB_ENABLE_ZGP_SINK

/** Max number of command identifiers in one
 *  functionality matching table entry */
#define ZB_ZGP_MATCH_TBL_MAX_CMDS_FOR_MATCH 5

/** Max number of cluster identifiers in one
 *  functionality matching table entry */
#define ZB_ZGP_TBL_MAX_CLUSTERS 5
#endif

#ifndef ZB_ENABLE_ZGP_SINK
#undef ZB_ENABLE_ZGP_MIGRATE_OLD_SINK_DATASET
#endif

/** Sink table size */
#ifndef ZB_ZGP_SINK_TBL_SIZE
#define ZB_ZGP_SINK_TBL_SIZE 32
#endif /* ZB_ZGP_SINK_TBL_SIZE */



/* Obsolete define to support old sink table migration
 * Looks like for correct migration needs to upgrade old system to zgp_dataset_info_ver_6_0_s using
 * the same ZB_ZGP_SINK_TBL_SIZE number */
/* Anyway we don't need do migration for translation table so let's keep this define as is. */
#define ZB_ZGP_TRANSL_TBL_SIZE 4*ZB_ZGP_SINK_TBL_SIZE

/* 5.1.2.3.2 test specification - The default value for DUT-GPP being a Basic Combo pr a Basic Proxy
 * is "ZigBeeAlliance09", i.e. {0x5A 0x69 0x67 0x42 0x65 0x65 0x41 0x6C 0x6C 0x69 0x61 0x6E
 * 0x63 0x65 0x30 0x39}. */
#define ZB_ZGP_DEFAULT_LINK_KEY "ZigBeeAlliance09"

/* 5.1.2.3.2 test specification - The default value for DUT-GPP being a Basic Proxy or Basic Combo
 * is 0b000. */
#define ZB_ZGP_DEFAULT_SHARED_SECURITY_KEY_TYPE ZB_ZGP_SEC_KEY_TYPE_NO_KEY

/* A.3.3.2.5 The default value is 180 seconds. */
#ifndef ZB_ZGP_DEFAULT_COMMISSIONING_WINDOW
#define ZB_ZGP_DEFAULT_COMMISSIONING_WINDOW 180
#endif

/* in bytes */
#define ZB_ZGP_MIN_PROXY_TABLE_ENTRY_SIZE 7

#ifndef ZB_ZGP_PROXY_COMMISSIONING_DEFAULT_COMMUNICATION_MODE
#define ZB_ZGP_PROXY_COMMISSIONING_DEFAULT_COMMUNICATION_MODE ZGP_PROXY_COMM_MODE_UNICAST
#endif

#define ZB_ZGP_DEFAULT_COMMISSIONING_EXIT_MODE ZGP_COMMISSIONING_EXIT_MODE_ON_GP_PROXY_COMMISSIONING_MODE_EXIT

#define ZB_ZGP_DEFAULT_SEC_LEVEL_PROTECTION_WITH_GP_LINK_KEY 0
#define ZB_ZGP_DEFAULT_SEC_LEVEL_INVOLVE_TC 0
#define ZB_ZGP_MAX_TEMP_MASTER_COUNT 0x03
#define ZB_ZGP_DMAX_FOR_ACCUMULATE_TEMP_MASTER_INFO 100
#define ZB_ZGP_MIN_SINK_TABLE_ENTRY_SIZE 8
#ifndef MAX_ZGP_CLUSTER_GPDF_PAYLOAD_SIZE
#define MAX_ZGP_CLUSTER_GPDF_PAYLOAD_SIZE 65 /* (64 for APP_000 + 1 byte header size) */
#endif
#define ZB_GP_DMIN_U_MS 5
#define ZB_GP_DMIN_D_MS 32

#ifndef ZB_ZGP_DEFAULT_COMMUNICATION_MODE
#define ZB_ZGP_DEFAULT_COMMUNICATION_MODE ZGP_COMMUNICATION_MODE_LIGHTWEIGHT_UNICAST
#endif

#define ZB_ZGP_DEFAULT_SECURITY_LEVEL ZB_ZGP_SEC_LEVEL_NO_SECURITY

#ifdef ZB_ENABLE_ZGP_PROXY
#ifndef ZB_ZGP_PROXY_TBL_SIZE
#define ZB_ZGP_PROXY_TBL_SIZE 16
#endif /* ZB_ZGP_PROXY_TBL_SIZE */
#endif  /* ZB_ENABLE_ZGP_PROXY */

#endif  /* !ZB_ZGPD_ROLE */


#ifdef ZB_ZCL_SUPPORT_CLUSTER_WWAH
#define ZB_ENABLE_NWK_RETRANSMIT
/* #ifdef ZB_ENABLE_SE */
/* #pragma error SE inconsistent with WWAH. Disable ZB_ENABLE_SE */
/* #endif */
#endif /* ZB_ZCL_SUPPORT_CLUSTER_WWAH */

#ifdef ZB_SUBGHZ_MODE
#define ZB_R22_MULTIMAC
#define ZB_MAC_DUTY_CYCLE_MONITORING
#define ZB_SUB_GHZ_LBT
#define ZB_FILTER_OUT_CLUSTERS
#define ZB_MAC_POWER_CONTROL
#define ZB_ZCL_SUPPORT_CLUSTER_SUBGHZ
#endif

/**
   Number of frames in GPFS (repeated frames with same mac seq number)

   For debug purposes set it to 1
*/

#ifndef ZB_ZGP_TX_QUEUE_SIZE
#define ZB_ZGP_TX_QUEUE_SIZE 10
#endif

#ifdef ZB_ZGP_IMMED_TX
/** Number of zb_zgp_tx_pinfo_t in zb_zgp_tx_packet_info_q_t. */
#define ZB_ZGP_TX_PACKET_INFO_COUNT ZB_ZGP_TX_QUEUE_SIZE + (ZB_IOBUF_POOL_SIZE / 4)
/** Value with which immed_tx_frame_counter is incremented upon a reboot. */
#define ZB_GP_IMMED_TX_FRAME_COUNTER_UPDATE_INTERVAL 1000
#define ZGP_INCLUDE_DST_LONG_ADDR
#else
#define ZB_ZGP_TX_PACKET_INFO_COUNT ZB_ZGP_TX_QUEUE_SIZE
#endif  /* ZB_ZGP_IMMED_TX */
/** Maximum payload length in outgoing ZGP frames */
#ifndef ZB_ZGP_TX_CMD_PLD_MAX_SIZE
#define ZB_ZGP_TX_CMD_PLD_MAX_SIZE  64 /* Maximum payload size of APP_0000 packet */
#endif

#define ZB_ZGP_LED_COMMISSIONING_PARAMS ZB_LED_ARG_CREATE(0, ZB_LED_BLINK_HALF_SEC)

/* some preconditions and dependencies */
#ifdef ZB_BDB_TOUCHLINK
#define ZB_ENABLE_ZLL
#define ZB_ENABLE_INTER_PAN_EXCHANGE
#endif

/*
  Features to be specified in vendor config:

  - ZB_ENABLE_HA
  - ZB_ENABLE_ZLL
  - ENABLE_ZGP_TARGET_PLUS

  HA specific:
  -ZB_ALL_DEVICE_SUPPORT

*/

/* BDB uses HA clusters */
#if defined ZB_BDB_MODE && !defined ZB_ENABLE_HA
#define ZB_ENABLE_HA
#endif

/**
   Size of table used for long addresses compression: 3 bytes of manufacturer ID.

   ZBOSS implements long address compression: 3 bytes of manufacturer
   ID are stored in the separate table; reference to manufacturer
   entry is stored in the long address giving 2 bytes economy.

   That is an absolute limit of numbers of manufacturers known to the device.

   @note All that machinery will not work if instead of legal
   manufacturer IDs (or illegal, but fixed IDs) use random values.
 */
#ifndef ZB_DEV_MANUFACTURER_TABLE_SIZE
#define ZB_DEV_MANUFACTURER_TABLE_SIZE 32
#endif

/* There were configuration options for different platforms.
   Since in ZOI we are separating stack and platform making not all platforms visible,
   include here current platform-specific stuff.

   Now every platform has its own platform/include/zb_config_platform.h where we moved stuff like
#if (defined ZB_CONFIG_LINUX_NSNG || defined ZB_CONFIG_SGW_NSNG)
#define ZB_NSNG
#define ZB_DSR_MAC
...


Ideally should rework the whole zb_config.h to suit better for that new concept.

 */
#include "zb_config_platform.h"

#ifndef ZB_ZGPD_GPFS_SIZE
#define ZB_ZGPD_GPFS_SIZE 4
#endif



/* #define ZB_TRANSCEIVER_ALL_CHANNELS_MASK   (0xffff << 11) */ /* 0000.0111 1111.1111 1111.1000 0000.0000*/
/* C51 doesn't like long shifts, it just cut last two bytes. (11-26) */
/* TODO: Remove old subgig definitions */
#define ZB_TRANSCEIVER_ALL_CHANNELS_MASK   0x07FFF800 /* 0000.0111 1111.1111 1111.1000 0000.0000*/

#define MAC_DEVICE_TABLE_SIZE 4

#if defined ZB_ENABLE_INTER_PAN_EXCHANGE && ! defined ZB_MAC_EXT_DATA_REQ
#define ZB_MAC_EXT_DATA_REQ
#endif /* defined ZB_ENABLE_INTER_PAN_EXCHANGE && ! defined ZB_MAC_EXT_DATA_REQ */





/* MAC transport in Linux, line NSNG, uart/macsplit etc */

/***********************************************************************/
/****************************General stack options**********************/
/***********************************************************************/

#define ZB_VERSION "0.2.0.1"

/*! \addtogroup ZB_CONFIG */
/*! @{ */

/** Network Selection Stack Profile ID*/
#define STACK_NETWORK_SELECT  0x00
/** 2007 Stack Profile ID */
#define STACK_2007            0x01
/** Pro Stack Profile ID */
#define STACK_PRO             0x02

/**
   This define switches PRO version on or off.
*/
#define ZB_PRO_STACK

/**
   If defined, 2007 stack profile is implemented
*/
//#define ZB_STACK_PROFILE_2007

/**
 *    If defined, NVRAM is used
 *    Create *.nvram file
 *
 *    To be used near always to prevent flash damage (flash can do ~1000 rewrites only)
 *
 *    For certification test need comment.
 *    Else change Assignment to Rejoin for second and last runs
 *
 */
//#define NVRAM_NOT_AVAILABLE

#ifndef NVRAM_NOT_AVAILABLE
#define ZB_USE_NVRAM
#endif
/*! @} */ /* ZB_CONFIG */

/**
   Use 32-bit timer
*/
#define ZB_TIMER_32

#ifndef ZB_CB_QUANT
#define ZB_CB_QUANT 1
#endif  /* !ZB_CB_QUANT */

/*! \addtogroup ZB_CONFIG */
/*! @{ */

/* MMDEVQ: what to do??? */
/**
   Zigbee channel selection.
   Default value of ZB_AIB().aps_channel_mask
   Channels to be used for discovery and join.
   If a single channel is selected, there is no need to perform energy scan.

   @note ZLL, HA, ZLL tests can redefine this mask with the means of other constants.
*/

/* ZB_DEFAULT_APS_CHANNEL_MASK can be redefined in zb_vendor.h */
#ifndef ZB_DEFAULT_APS_CHANNEL_MASK
//#define ZB_DEFAULT_APS_CHANNEL_MASK ((1l<<11))
/* Change that place if you want to set channel for internal tests */
//#define ZB_DEFAULT_APS_CHANNEL_MASK ((1l<<11))
/* #define ZB_DEFAULT_APS_CHANNEL_MASK ((1l<<16)) */
#define ZB_DEFAULT_APS_CHANNEL_MASK (ZB_TRANSCEIVER_ALL_CHANNELS_MASK)
#endif  /* ZB_DEFAULT_APS_CHANNEL_MASK */
/** @} */ /* ZB_CONFIG */


//**
//   If defined, do not compile some features, even Mandatory, to fit into 64k ROM at 2410/64k device in Keil debug build.
//*/
//#define ZB_LIMITED_FEATURES

/*  PRO stack specific details */
/**
   NWK: If defined, use stochastic address assignment (Zigbee PRO).
*/
#define ZB_NWK_STOCHASTIC_ADDRESS_ASSIGN

/*! Source routing path length, also called nwkMaxSourceRoute */
#define ZB_NWK_MAX_PATH_LENGTH  5
/*! Source route table capacity */
/* 10/21/2019 EE CR:MINOR Isn't it too many? Can we handle situation when source route table is not big enough? */
#define ZB_NWK_MAX_SRC_ROUTES   ZB_NEIGHBOR_TABLE_SIZE
/* 10/21/2019 EE CR:MINOR Better indicate in const name that this is time in seconds by adding _S suffix */
/*! Expiration time of the source route table (300 sec) */
#define ZB_NWK_SRC_ROUTE_TABLE_EXPIRY 60




/****************************Zigbee Roles*******************************/

/*
# Device roles per build.

We usually have ZC and ZR combined.
We can have ZR which can be switched to ZED (via reset to factory defaults), or ZR which is "just ZR".
It could be useful to have ZC-only and ZR-only libraries.
ZED rx-on-when-idle can be configurable at runtime or at compile time.

To better divide ZC an ZR define functionality groups:

- routing & parent srv (zc & zr) - ZB_ROUTER_ROLE (name is not perfect, but keep it to minimize code change)
- centralized TC - ZB_COORDINATOR_ROLE  (name is not perfect, but keep it to minimize code change)
- joiner client functionality (zr & zed) - ZB_JOIN_CLIENT
- formation functionality - ZB_FORMATION == ZB_COORDINATOR_ROLE || ZB_ROUTER_ROLE && ZB_DISTRIBUTED_SECURITY_ON. Also in request key etc
- distributed-only ZR build (do not link router's update device etc) - ZB_DISTRIBUTED_ONLY
- distributed security - ZB_ROUTER_ROLE & ZB_DISTRIBUTED_SECURITY_ON
  - ZR (link if enabled?)
- router security (upd dev send etc) ZB_ROUTER_SECURITY == ZB_ROUTER_ROLE && !ZB_COORDINATOR_ROLE && !ZB_DISTRIBUTED_ONLY
- By default

- panid conflict resolution (TBD)
  - ZC part - always, or if enabled
  - ZR part - if enabled

- address conflict resolution (TBD)
  - in ZR role
  - in ZC role (recheck: is it so?)

- ZED functionality (either ZED or ZR) - ZB_ED_FUNC. Can be present in ED-only or ZR build.

ZB_ED_ROLE == ZB_ED_FUNC && !ZB_ROUTER_ROLE && !ZB_COORDINATOR_ROLE

ZB_ED_FUNC is polling and ED aging.

ZB_ROUTER_ROLE, ZB_COORDINATOR_ROLE, ZB_ED_ROLE defines exists for the very beginning. Let's keep it in the code where it is appropriate.

In general, define ZB_COORDINATOR_ROLE to compile ZC-only build, ZB_ROUTER_ROLE to compile ZR+ZED build, ZB_ED_ROLE to compile ZED-only build.
 */

#if !defined ZB_ED_ROLE && !defined ZB_ZGPD_ROLE && !defined ZB_COORDINATOR_ROLE && !defined ZB_ROUTER_ROLE
/**
   If no any role defined, let it be max possible roles set: ZC + ZR switchable to ZED at runtime
*/
#define ZB_COORDINATOR_ROLE
#define ZB_ROUTER_ROLE
#ifndef ZB_ROUTER_NO_ED
/*! Define Zigbee end device functionality */
#define ZB_ED_FUNC
#endif
#endif

#if defined ZB_ED_ROLE || defined ZB_ROUTER_ROLE
/*! Define joiner client functionality */
#define ZB_JOIN_CLIENT
#endif

#if defined ZB_COORDINATOR_ROLE && !defined ZB_ROUTER_ROLE
/**
   If only ZB_COORDINATOR_ROLE is defined, only ZC role is compiled.
*/
#define ZB_ROUTER_ROLE
#define ZB_COORDINATOR_ONLY
#endif /*defined ZB_COORDINATOR_ROLE && !defined ZB_ROUTER_ROLE*/

#if defined ZB_ROUTER_ROLE && !defined ZB_COORDINATOR_ONLY && !defined ZB_ROUTER_NO_ED

/* By default ZR can be switched to ZED at runtime. */
/*! ZB end device functionality */
#define ZB_ED_FUNC
#endif

#ifdef ZB_ROUTER_NO_ED
#define ZB_LITE_NO_JOIN_ZR_AS_ZED
#endif

/*
Related defined
ZB_LITE_ROUTER_ONLY_ROLE
ZB_LITE_NO_JOIN_ZR_AS_ZED
ZB_ED_RX_OFF_WHEN_IDLE
 */

#if defined ZB_COORDINATOR_ROLE || (defined ZB_ROUTER_ROLE && defined ZB_DISTRIBUTED_SECURITY_ON)

/*! Formation is supported by ZC or ZR in Distributed mode */
#define ZB_FORMATION
#endif

#if defined ZB_ROUTER_ROLE && !defined ZB_COORDINATOR_ONLY && !defined ZB_DISTRIBUTED_ONLY
/*! Defines router security */
#define ZB_ROUTER_SECURITY
#endif

#if defined ZB_ED_ROLE && (defined ZB_ROUTER_ROLE || defined ZB_COORDINATOR_ROLE)
#error Conflicting defines: ZB_ED_ROLE excludes ZB_ROUTER_ROLE/ZB_COORDINATOR_ROLE
#endif

#ifdef ZB_ED_ROLE
/* ED functionality is implemented by ZED-only build or ZR switchable to ZED */
#define ZB_ED_FUNC
#endif
/** @addtogroup ZB_CONFIG */
/** @{ */
/**
 *   NWK: size of the neighbor table
 *
 *  @note This is a default value of the define. This value can be changed by user.
 */
#ifndef ZB_NEIGHBOR_TABLE_SIZE

#if defined ZB_COORDINATOR_ROLE
#define ZB_NEIGHBOR_TABLE_SIZE 32

#elif defined ZB_ROUTER_ROLE
#define ZB_NEIGHBOR_TABLE_SIZE 32

#elif defined ZB_ED_ROLE
#define ZB_NEIGHBOR_TABLE_SIZE 32

#elif defined ZB_ZGPD_ROLE
/* zcl_common.zgpd.o won't be built without any ZB_NEIGHBOR_TABLE_SIZE definition. */
#define ZB_NEIGHBOR_TABLE_SIZE 1

#else
#error No any role defined
#endif  /* roles */

#endif /* ZB_NEIGHBOR_TABLE_SIZE */

/**
 *  Scheduler callbacks queue size. Usually not need to change it.
 *
 *  @note This is a default value of the define. This value can be changed by user.
*/
#ifndef ZB_SCHEDULER_Q_SIZE
#ifdef ZB_ED_ROLE
#define ZB_SCHEDULER_Q_SIZE 16
#else
/* @note: If set ZB_SCHEDULER_Q_SIZE > 32 is set, change alarms bitmask in the scheduler */
#define ZB_SCHEDULER_Q_SIZE 32
#endif
#endif /*ZB_SCHEDULER_Q_SIZE*/
/** @} */ /* ZB_CONFIG */
#if defined ZB_ED_ROLE
/**
   End device idle time-out
*/
#define ZB_TIME_ED_IDLE ZB_MILLISECONDS_TO_BEACON_INTERVAL(7500)
#endif


#if defined ZB_ROUTER_ROLE
/**
   NWK: If defined, enable routing functionality
*/
#define ZB_NWK_ROUTING

/**
   Number of secure materials to store
*/
#define N_SECUR_MATERIAL 3


#ifndef ZB_NWK_STOCHASTIC_ADDRESS_ASSIGN
/**
   If not defined, implement tree routing
*/
#define ZB_NWK_TREE_ROUTING
#endif /*ZB_NWK_STOCHASTIC_ADDRESS_ASSIGN*/

/**
   NWK: if defined, implement mesh routing
*/
#define ZB_NWK_MESH_ROUTING

#else

#define N_SECUR_MATERIAL 1

#endif  /* coordinator or router */


/****************************APS layer options**************************/
/** @addtogroup ZB_CONFIG */
/** @{ */
#ifndef ZB_MAX_EP_NUMBER
/* max supported EP number, increase if needed */
/** Maximum number of supported endpoints per device */
#define ZB_MAX_EP_NUMBER 5
#endif
/* APS binding */

#ifndef ZB_MEMORY_COMPACT

#ifndef ZB_APS_SRC_BINDING_TABLE_SIZE
/**
 *  APS: SRC binding table size
 *
 *  @note This is a default value of the define. This value can be changed by user.
*/
#define ZB_APS_SRC_BINDING_TABLE_SIZE 32
#endif


#ifndef ZB_APS_DST_BINDING_TABLE_SIZE
/**
 *   APS: DST binding table size
 *
 *   @note This is a default value of the define. This value can be changed by user.
*/
#define ZB_APS_DST_BINDING_TABLE_SIZE 32
#endif


#ifndef ZB_APS_GROUP_TABLE_SIZE
/**
   APS: man number of groups in the system
*/
#define ZB_APS_GROUP_TABLE_SIZE       16
#endif


#ifndef ZB_APS_ENDPOINTS_IN_GROUP_TABLE
/**
   APS: max number of endpoints per group table entry
*/
#define ZB_APS_ENDPOINTS_IN_GROUP_TABLE 8
#endif

/**
   APS: size of queue to be used to pass
   incoming group addresses packets up
*/
#define ZB_APS_GROUP_UP_Q_SIZE 8

#else  /* ZB_MEMORY_COMPACT */

#ifndef ZB_APS_SRC_BINDING_TABLE_SIZE
#define ZB_APS_SRC_BINDING_TABLE_SIZE 8
#endif


#ifndef ZB_APS_DST_BINDING_TABLE_SIZE
/**
   APS: DST binding table size
*/
#define ZB_APS_DST_BINDING_TABLE_SIZE 8
#endif


#ifndef ZB_APS_GROUP_TABLE_SIZE
/**
   APS: man number of groups in the system
*/
#define ZB_APS_GROUP_TABLE_SIZE       4
#endif


#ifndef ZB_APS_ENDPOINTS_IN_GROUP_TABLE
/**
   APS: max number of endpoints per group table entry
*/
#define ZB_APS_ENDPOINTS_IN_GROUP_TABLE 4
#endif

/**
   APS: size of queue to be used to pass
   incoming group addresses packets up
*/
#define ZB_APS_GROUP_UP_Q_SIZE 4

#endif  /* ZB_MEMORY_COMPACT */


#ifndef ZB_N_APS_KEY_PAIR_ARR_MAX_SIZE
/**
 * APS: maximum number of tables with key-pair information
 *
 * @note This is a default value of the define. This value can be changed by user.
*/
#ifdef ZB_ED_ROLE
#define ZB_N_APS_KEY_PAIR_ARR_MAX_SIZE            5
#else
/* [EE] 05/25/2016 CR:MINOR Set it to some big value - say, 128. This is total #
 * of devices in the network, not neighbor table size */
#define ZB_N_APS_KEY_PAIR_ARR_MAX_SIZE            ZB_NEIGHBOR_TABLE_SIZE
#endif
#endif

/**
   If the joining device does not receive any of
   the keys within apsSecurityTimeOutPeriod, of receiving the NLME-JOIN.
   confirm primitive, it shall reset

   apsSecurityTimeOutPeriod value is not defined by specification and should be defined in stack
   profile. For ZB_PRO_STACK this value equal to 1.7 seconds on 2.4 GHz.

   See Zigbee specification revision 22 subclause 4.4.11
*/
#define ZB_APS_SECURITY_TIME_OUT_PERIOD ZB_MILLISECONDS_TO_BEACON_INTERVAL(1700)

/**
 APS: Get APS ACK wait time for the device depending on its receiver on when idle
*/
/* WARNING: Generally, it is not according to spec. There are some nwk and APS routines that should
 * correlate to this interval, for these we will use ZB_N_APS_ACK_WAIT_DURATION_FROM_SLEEPY. */
#ifndef ZB_ED_RX_OFF_WHEN_IDLE
#define ZB_N_APS_ACK_WAIT_DURATION(_rx_on_when_idle)  \
  ((_rx_on_when_idle) ?                                                 \
   ZB_N_APS_ACK_WAIT_DURATION_FROM_NON_SLEEPY : ZB_N_APS_ACK_WAIT_DURATION_FROM_SLEEPY)
#else
#define ZB_N_APS_ACK_WAIT_DURATION(_rx_on_when_idle)  \
  (ZB_N_APS_ACK_WAIT_DURATION_FROM_SLEEPY)
#endif

/****************************NWK layer options**************************/

/*
  If defined, NWK and upper layers can access PIB directly.

  Usually, can be defined when using our MAC.
  Undef it to separate MAC and upper layers IB spaces for debug purposes.

  #define ZB_DIRECT_PIB_ACCESS

*/
/**
 *   NWK: size of the long-short address translation table
 *
 *   @note This is a default value of the define. This value can be changed by user.
*/
#ifndef ZB_MEMORY_COMPACT
#define ZB_IEEE_ADDR_TABLE_SIZE 101
#else
#define ZB_IEEE_ADDR_TABLE_SIZE 32
#endif

#ifndef ZB_PANID_TABLE_SIZE
/**
   NWK: size of the long-short Pan ID translation table

   Must be <= (packet buffer size - sizeof(*discovery_confirm)) / sizeof(*network_descriptor)

   That value limits number of Pan IDs visible for device during active scan.
*/
#define ZB_PANID_TABLE_SIZE 16
#endif
/** @cond DOXYGEN_INTERNAL_DOC */
/**
   If defined, add jitter to broadcast transmits etc.

   @note It adds zb_random() and breaks some tests that are supposed to know
   next generated address by setting random generator start.
*/
#define ZB_NWK_USE_SEND_JITTER
/** @endcond */ /* DOXYGEN_INTERNAL_DOC */
/* Some defaults for ZDO startup */

#ifndef ZB_TRACE_LEVEL
/**
   NWK: default energy/active scan duration
*/
#define ZB_DEFAULT_SCAN_DURATION 3


#else  /* ZB_TRACE_LEVEL */

#define ZB_DEFAULT_SCAN_DURATION 5


#endif /*ZB_TRACE_LEVEL*/


#if ZB_ZDO_NWK_SCAN_ATTEMPTS == 1
/**
   Integer value representing the
   * number of scan attempts to make
   * before the NWK layer decides
   * which Zigbee coordinator or
   * router to associate with
   */

#define ZB_NWK_ONE_SCAN_ATTEMPT
#endif /*ZB_ZDO_NWK_SCAN_ATTEMPTS == 1*/


/**
   Number of pending Mgmt_Leave requests allowed
*/
#ifndef ZB_MEMORY_COMPACT
#define ZB_ZDO_PENDING_LEAVE_SIZE 4
#else
#define ZB_ZDO_PENDING_LEAVE_SIZE 2
#endif /*ZB_MEMORY_COMPACT*/
/** @cond DOXYGEN_INTERNAL_DOC */
#ifndef ZB_COORDINATOR_ONLY
/* Enables PAN blacklisting. Note: it was under ifdef
 * ZB_COORDINATOR_ROLE. But ZBOSS usually built with ZR & ZC, so
 * blacklist is useful for ZR. */
/** Enables PAN blacklisting */
#define ZB_NWK_BLACKLIST
#ifndef ZB_NWK_BLACKLIST_SIZE
/** Size of PAN IDs blacklist */
#define ZB_NWK_BLACKLIST_SIZE 16
#endif
#endif

/**
 *   NWK Mesh route: routing table size
 *
 *   @note This is a default value of the define. This value can be changed by user.
*/
#ifndef ZB_MEMORY_COMPACT
#define ZB_NWK_ROUTING_TABLE_SIZE ZB_NEIGHBOR_TABLE_SIZE
#else
#define ZB_NWK_ROUTING_TABLE_SIZE 5
#endif
/** @endcond */ /* DOXYGEN_INTERNAL_DOC */

/** @cond DOXYGEN_MULTIMAC_SECTION */
#ifndef ZB_NWK_MAC_IFACE_TBL_SIZE
/* For now we use only 1 MAC interface (maybe, working in 2.4 os sub-gig) */
/**
   NWK MultiMAC content
*/
#define ZB_NWK_MAC_IFACE_TBL_SIZE 1
#endif  /* ZB_NWK_MAC_IFACE_TBL_SIZE */
/** @endcond */ /* DOXYGEN_MULTIMAC_SECTION */

/**
   Size of channel list structure
*/
#define ZB_CHANNEL_PAGES_NUM 1

/** @cond DOXYGEN_SE_SECTION */
/** Maximum channel pages number according to the Zigbee revision 22 specification */
#define ZB_CHANNEL_PAGES_MAX_NUM 5
/** @endcond */

/*! @cond DOXYGEN_INTERNAL_DOC */
/**
The minimum value of the
backoff exponent (BE) in the
CSMA-CA algorithm. See
7.5.1.4 for a detailed
explanation of the backoff
exponent.
 */
#define ZB_MAC_MIN_BE 3

 /**
The maximum value of the
backoff exponent, BE, in the
CSMA-CA algorithm. See
7.5.1.4 for a detailed
explanation of the backoff
exponent.
 */
#define ZB_MAC_MAX_BE 5
/*! @endcond */ /* DOXYGEN_INTERNAL_DOC */

#ifdef ZB_MAC_POLL_INDICATION_CALLS_REDUCED
#ifndef ZB_MAC_POLL_INDICATION_CALL_MAX_TIMEOUT
/**
   Default maximum value of Poll Indication calls timeout (in sec)
 */
#define ZB_MAC_POLL_INDICATION_CALL_MAX_TIMEOUT 3600
#endif
#endif /* ZB_MAC_POLL_INDICATION_CALLS_REDUCED */

/** @cond DOXYGEN_INTERNAL_DOC */
#ifndef ZB_NWK_NEIGHBOUR_PATH_COST_RSSI_BASED
/**
    Neighbor path cost calculation method.
    By default, calculate neighbor path cost from LQI value as described in ZB
    spec(section 3.6.3.1). But for some special cases (e.g. LCGW) we need to calculate path cost
    from RSSI value.
 */
#define ZB_NWK_NEIGHBOUR_PATH_COST_LQI_BASED
#endif /* ZB_NWK_NEIGHBOUR_PATH_COST_RSSI_BASED */
/** @endcond */ /* DOXYGEN_INTERNAL_DOC */

/***********************************************************************/
/***************************ZBOSS FEATURES SECTION**********************/
/***********************************************************************/

/***************************STACK FEATURES**********************/

/** @cond DOXYGEN_INTERNAL_DOC *//* Current stack profile */
#define ZB_STACK_PROFILE        STACK_PRO

/**
   Protocol version selection: see table 1.1(pro-specification)
*/
#if ZB_STACK_PROFILE == STACK_PRO || ZB_STACK_PROFILE == STACK_2007
#define ZB_PROTOCOL_VERSION 0x02
#else
#define ZB_PROTOCOL_VERSION 0x01
#endif
/** @endcond */ /* DOXYGEN_INTERNAL_DOC ZB_STACK_PROFILE */


#ifndef ZB_BUF_Q_SIZE
/* @note To prevent deadlocks ZB_BUF_Q_SIZE must be < ZB_IOBUF_POOL_SIZE/2 */
/**
   Size of queue for wait for free packet buffer
*/
#define ZB_BUF_Q_SIZE 8
#endif /*ZB_BUF_Q_SIZE*/

/**
   Size, in bytes, of the packet buffer

   @internal Be sure keep it multiple of 4 to exclude alignment problems at ARM
*/
#ifndef ZB_IO_BUF_SIZE
#define ZB_IO_BUF_SIZE 148
#endif  /* ZB_IO_BUF_SIZE */

/**
   Buffer tail portion alignment - 4.

   Note: not sizeof(zb_size_t) because at 64-bit build it is 8 which we don't want.
 */
#define ZB_BUF_ALLOC_ALIGN 4


/**
   Number of fragmented packets which ZBOSS can receive in parallel
 */
#define ZB_APS_MAX_IN_FRAGMENT_TRANSMISSIONS 3

/**
 *   Number of packet buffers. More buffers - more memory. Less buffers - risk to be blocked due to buffer absence.
 *
 *   @note This is a default value of the define. This value can be changed by user.
*/
#ifndef ZB_IOBUF_POOL_SIZE
#if defined ZB_MEMORY_COMPACT && defined ZB_ED_ROLE
#define ZB_IOBUF_POOL_SIZE 20
#else
#define ZB_IOBUF_POOL_SIZE 26
#endif
#endif

/** @cond DOXYGEN_INTERNAL_DOC */
/*
  If enabled, real int24 and uint48 types will work. Else, int24 and uint48 is mapped to
  int32 and uint32.
  See include/zb_uint48.h.
*/
/** Enable support for int24 and uint48 data types */
#define ZB_UINT24_48_SUPPORT
/** @endcond */ /* DOXYGEN_INTERNAL_DOC */

/** Should expire in this number of attempts if failure*/
#define ZB_NWK_ROUTING_FAILURE_ATTEMPTS_NUM 1

/** Calculate routing expiry step interval based on timer initiation value and number of attempts*/
#define ZB_NWK_ROUTING_FAILURE_EXPIRY_STEP (ZB_NWK_ROUTING_TABLE_EXPIRY / ZB_NWK_ROUTING_FAILURE_ATTEMPTS_NUM)

/**
   Number of times device fails to send packet to the parent before rejoin
*/
#define ZB_ZDO_PARENT_LINK_FAILURE_CNT 12

/** @cond DOXYGEN_INTERNAL_DOC */
//#define ZB_ZDO_CHECK_FAILS_NWK_UPDATE_NOTIFY_LIMIT 4
#define ZB_ZDO_CHECK_FAILS_CLEAR_TIMEOUT (30 * ZB_TIME_ONE_SECOND)

#define ZB_PREDEFINED_ROUTER_ADDR 0x3344
#define ZB_PREDEFINED_ED_ADDR     0x3344
/** @endcond */ /* DOXYGEN_INTERNAL_DOC */

#ifdef ZB_LIMIT_VISIBILITY
/**
   Maximum number of addresses in the visibility limit arrays
*/
#define ZB_N_VISIBLE_ADDRESSES 6
#endif

#if defined ZB_LITTLE_ENDIAN && defined ZB_BIG_ENDIAN
#error  Enable only 1 profile support: ZB_LITTLE_ENDIAN or ZB_BIG_ENDIAN
#endif /* defined ZB_LITTLE_ENDIAN && defined ZB_BIG_ENDIAN */

/** @cond DOXYGEN_INTERNAL_DOC */
/* DA: network status with OUT_OF_MEMORY custom value */
#if !defined ZB_CHECK_OOM_STATUS && !defined xZB_CHECK_OOM_STATUS && !defined ZB_MACSPLIT_DEVICE && !defined ZB_LITE_NO_OOM_DETECTION
/** Enable check whether the ZBOSS stack is out of memory */
#define ZB_CHECK_OOM_STATUS
#endif
//#define ZB_SEND_OOM_STATUS

#ifdef ZB_CHECK_OOM_STATUS
/** If bufpool is in out of memory state for a period, specified by ZB_OOM_THRESHOLD,
 * then assertion is triggered. */
#define ZB_OOM_THRESHOLD (120 * ZB_TIME_ONE_SECOND)
#ifdef ZB_SEND_OOM_STATUS
#define ZB_SEND_OOM_DELAY (5 * ZB_TIME_ONE_SECOND)
#endif /* ZB_SEND_OOM_STATUS */
#endif /* ZB_CHECK_OOM_STATUS */
/** @endcond */ /* DOXYGEN_INTERNAL_DOC */

/** @cond DOXYGEN_JOINING_LIST_SECTION */
#define ZB_JOINING_LIST_Q_SIZE 5
#define ZB_JOINING_LIST_RESP_ITEMS_LIMIT 9
#define ZB_JOINING_LIST_DELAY_STEP ZB_TIME_ONE_SECOND
/* default value for mibIeeeExpiryInterval, in minutes */
#define ZB_JOINING_LIST_DEFAULT_EXPIRY_INTERVAL 5
/** @endcond */ /* DOXYGEN_JOINING_LIST_SECTION */

/***************************HA and ZLL FEATURES**********************/
/** @cond DOXYGEN_INTERNAL_DOC */
#if defined ZB_ENABLE_HA || defined ZB_ENABLE_ZLL
#define ZB_ENABLE_ZCL
#endif /* defined ZB_ENABLE_HA || ZB_ENABLE_ZLL */
/** @endcond */ /* DOXYGEN_INTERNAL_DOC */

/* Enable only 1 profile support: ZLL or HA */
/** @cond DOXYGEN_ZLL_SECTION */
#if defined ZB_ENABLE_ZLL || defined DOXYGEN
/** @brief Controls inter-PAN exchange feature */
#define ZB_ENABLE_INTER_PAN_EXCHANGE
/* only ZLL uses AES128 decoding */
#define ZB_NEED_AES128_DEC
#define ZB_ZLL_ENABLE_COMMISSIONING_CLIENT
#define ZB_ZLL_ENABLE_COMMISSIONING_SERVER
#ifdef ZB_ZLL_ENABLE_COMMISSIONING_CLIENT
/* Client device should be address assignment capable */
#define ZB_ZLL_ADDR_ASSIGN_CAPABLE
#endif
#define ZB_ZLL_PRIMARY_CHANNELS {11, 15, 20, 25}
#endif /* defined ZB_ENABLE_ZLL || defined DOXYGEN */
/** @endcond */ /* DOXYGEN_ZLL_SECTION */

/*************************Serial and UDP trace**********************/
#ifndef ZB_PLATFORM_LINUX /* Uncomment for binary logs! */
#if defined ZB_SERIAL_FOR_TRACE || defined ZB_TRACE_OVER_JTAG || defined ZB_NET_TRACE
/* binary trace: optimize traffic size. need special win_com_dump */
#define ZB_BINARY_TRACE
/* #define ZB_TRAFFIC_DUMP_ON */
/* #define ZB_TRAF_DUMP_V2 */
#endif /*ZB_SERIAL_FOR_TRACE || defined ZB_TRACE_OVER_JTAG || defined ZB_NET_TRACE*/
#endif /*ZB_PLATFORM_LINUX*/

/** @cond DOXYGEN_INTERNAL_DOC */
#ifdef ZB_PLATFORM_LINUX

/* NSNG run in the single thread */
#define ZB_THREADS
#ifndef ZB_INIT_HAS_ARGS
#define ZB_INIT_HAS_ARGS
#endif
#ifndef ZB_HAVE_FILE
#define ZB_HAVE_FILE
#endif
#ifndef ZB_TRACE_TO_FILE
#define ZB_TRACE_TO_FILE
#endif
#ifndef ZB_TMP_FILE_PATH_PREFIX
#define ZB_TMP_FILE_PATH_PREFIX ""
#endif
#ifndef ZB_BINARY_FILE_PATH_PREFIX
#define ZB_BINARY_FILE_PATH_PREFIX ""
#endif
#ifndef ZB_USERDATA_FILE_PATH_PREFIX
#define ZB_USERDATA_FILE_PATH_PREFIX ""
#endif
/* #  define ZB_BINARY_TRACE */
/* #  define ZB_NET_TRACE */
/* #  define ZB_TRAFFIC_DUMP_ON */
#endif

#ifndef ZB_MEMTRACE_BUF_SIZE
#define ZB_MEMTRACE_BUF_SIZE 4080
#endif

/* Tune trace portion to fit HTTPS body into single Ethernet frame. Align to 3
 * because base64 packs 3 bytes into 4 chars. */
#ifndef ZB_NET_TRACE_PORTION
#define ZB_NET_TRACE_PORTION 1020
#endif


/**
   If defined, transport calls traffic dump by itself, generic MAC not need to do it
*/
//#define ZB_TRANSPORT_OWN_TRAFFIC_DUMP_ON

//#define ZB_DUMP_TRAF

/**
   Name for trace off switch file

   If file of this name exists in the current directory, switch off both trace
   and traffic dump.
*/
#define ZB_TRACE_SWITCH_OFF_FILE_NAME "trace_off"
/** @endcond */ /* DOXYGEN_INTERNAL_DOC */

/************************Hardware watchdog******************/


#ifndef ZB_WATCHDOG_SCHED_QUANT
/**
 *Check watchdogs once per 20 sec.
 *The hardware watchdog timer is set to 30 seconds,
 *it should be initiated even if there is no job for it
*/
#define ZB_WATCHDOG_SCHED_QUANT (20 * ZB_TIME_ONE_SECOND)
#endif


/** @cond DOXYGEN_INTERNAL_DOC */
/* membuffers debug: */
/* extra buffers trace and debug */
/* WARNING: Disabled by default. It costs too many ROM size! */
#if ZB_TRACE_LEVEL && ZB_TRACE_MASK
/* #define ZB_DEBUG_BUFFERS */
#endif

/* Assert when already freed memory buffer is accessed. Useful for debugging */
#ifdef ZB_DEBUG_BUFFERS
#define ZB_BUF_SHIELD
/* extended membuffers debug*/
#ifdef ZB_DEBUG_BUFFERS_EXT
#define ZB_DEBUG_BUFFERS_EXT_USAGES_COUNT 1
#endif
#endif /*ZB_DEBUG_BUFFERS*/
/** @cond */ /* DOXYGEN_INTERNAL_DOC */

/************************Special modes for testing*******************/

#ifdef DEBUG
#if !defined ZB_LIMITED_FEATURES && !defined ZB_TEST_PROFILE && !defined xZB_TEST_PROFILE
#ifdef ZB_CERTIFICATION_HACKS
/**
 * This define turns on/off test profile
 */
#define ZB_TEST_PROFILE
#endif
#endif /*ZB_LIMITED_FEATURES*/
#endif /*DEBUG*/


/* Testing mode for some pro certification tests */
/** @cond DOXYGEN_INTERNAL_DOC */

#define ZB_PRO_TESTING_MODE
#ifdef ZB_PRO_TESTING_MODE

/* See: certification test TP_PRO_BV-11 */
#ifdef ZB_CERTIFICATION_HACKS
#if !defined ZB_LIMIT_VISIBILITY && !defined ZB_MEMORY_COMPACT
#define ZB_LIMIT_VISIBILITY
#endif /*!defined ZB_LIMIT_VISIBILITY && !defined ZB_MEMORY_COMPACT*/
#endif /* ZB_CERTIFICATION_HACKS */

#if defined ZB_ROUTER_ROLE
/* See: certification test 13.66_Commissioning_the_short_address */
#define ZB_PRO_ADDRESS_ASSIGNMENT_CB
#endif /*defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE*/

/* Compatibility tests (with Ember Zigbee platform ISA3)*/

/* Ember + ZBOSS stack with ZLL Profile*/

/* Ember + ZBOSS Core Pro stack */
//#define ZB_EMBER_TESTS

/* Send messages with empty payload without APS-security */
#define ZB_SEND_EMPTY_DATA_WITHOUT_APS_ENCRYPTION

#endif /* ZB_PRO_TESTING_MODE */


//* Definitions for 802.15.4 certification hacks */

//#define MAC_CERT_TEST_HACKS

//*
//   This define turns on/off channel error mode (set errors while data sending)
//*/
//#define ZB_CHANNEL_ERROR_TEST

//#define TP_PRO_BV_31

//#define TP_PRO_BV_38


/* Put radio into RX mode before transmitting. */
#ifndef ZB_TRANSCEIVER_ON_BEFORE_TX
#define ZB_TRANSCEIVER_ON_BEFORE_TX  1
#endif /* ZB_TRANSCEIVER_ON_BEFORE_TX */

/* Put radio into RX mode before starting Energy Detection. */
#ifndef ZB_TRANSCEIVER_ON_BEFORE_ED
#define ZB_TRANSCEIVER_ON_BEFORE_ED  1
#endif /* ZB_TRANSCEIVER_ON_BEFORE_ED */
/** @endcond */ /* DOXYGEN_INTERNAL_DOC */

/* Certification defines */

#if 0
/**
   Disabling of processing of bind_req and end_device_bind_req for certification purposes
*/
#define ZB_DISABLE_BIND_REQ
#endif


#if 0
/**
   Disabling of processing of end device bind requests
*/
#define ZB_DISABLE_ED_BIND_REQ
#endif

/*PRO STACK COMPATIBILITY, it is better to turn it on only when needed */
/* #AT disable compatible mode with 2007 stack version */
#if 0
#define ZB_PRO_COMPATIBLE
#endif

/**
   Default value for mac frame version subfield
*/
#define MAC_FRAME_VERSION MAC_FRAME_IEEE_802_15_4_2003
/** @cond DOXYGEN_INTERNAL_DOC */
/**
   Block sleepy end devices (devices with rx-on-when-idle == false) from receiving broadcasts (drop it at MAC level)
*/
#define ZB_BLOCK_BROADCASTS_SLEEPY_ED
/** @endcond */ /* DOXYGEN_INTERNAL_DOC */
/* MAC */
/* TODO: add MAC properties here */
/** @cond DOXYGEN_JOINING_LIST_SECTION */
#ifndef MAC_JOINING_LIST_SIZE_LIMIT
#define MAC_JOINING_LIST_SIZE_LIMIT 16
#endif
/** @endcond */ /* DOXYGEN_JOINING_LIST_SECTION */

/***** GREEN POWER *****/

#ifdef ZB_ZGPD_ROLE
#define ZB_MAC_ONLY_STACK
#undef ZB_CHECK_OOM_STATUS
#undef APS_FRAME_SECURITY
#undef ZB_ENABLE_ZCL
#undef ZB_ENABLE_ZLL
#undef ZB_ENABLE_HA
#undef ZB_USE_NVRAM
/* Temporarily disabled for ZGPD MAC library build. */
/* #undef ZB_DIRECT_MODE_WITH_ZGPD */
#define ZB_ENABLE_ZGPD_ATTR_REPORTING
/*
 * The OperationalChannel sub-field can take the following values: 0b0000: channel 11,
 * 0b0001: channel 12, , 0b1111: channel 26.
 * 26-11+1 = 12. Let's use 3 series of 4 channels each.
 * @note that channelsdiapason must be multiple of series.
 */
#define ZB_ZGPD_CH_SERIES       3

#define ZB_GPD_COMMISSIONING_RETRY_INTERVAL ZB_MILLISECONDS_TO_BEACON_INTERVAL(500)
#endif /* ZB_ZGPD_ROLE */

#ifdef ZB_ENABLE_ZGP_DIRECT
/* Old GP GPD sensors (until May, 2014) use
 * 5 ms offset. New sensors use 20 ms offset.*/
//#define ZB_GPD_RX_OFFSET_MS 5
#define ZB_GPD_RX_OFFSET_MS 20

/* Old platforms require gpTxOffset calibration. It put into zb_config_platform.h. At new DSR platforms DSR MAC has a call "tx at specified time" so GP TX delay calibrating is not required.  */
#ifndef ZB_GPD_TX_OFFSET_US
#define ZB_GPD_TX_OFFSET_US                                   \
  (ZB_MILLISECONDS_TO_USEC(ZB_GPD_RX_OFFSET_MS))
#endif

/**
   gpdMinRxWindow
   The gpdMinRxWindow is minimal duration of the reception window of an Rx-capable GPD.
   It has the value of 0.576 ms that corresponds to the Channel Configuration GPDF of 18B.
   GPD vendors shall implement reception window duration that corresponds to the actual GPD frame
   size to be received by this GPD, which may never be shorter than
   gpdMinRxWindow.

   A.1.6.3.2 gpdMinRxWindow
*/
#define ZB_GPD_MIN_RX_WINDOW_MS 50
#endif  /* ZB_ENABLE_ZGP_DIRECT */
/** @} */ /* ZB_CONFIG */
/************************NVRAM SUPPORT*******************/

#if defined ZB_USE_NVRAM
/**
   Storing NWK security counter in NVRAM
*/
#define ZB_STORE_COUNTERS
/**
   Interval in which counter is stored
*/
#define ZB_LAZY_COUNTER_INTERVAL 0x400
#endif /*ZB_USE_NVRAM*/

/**
   Migration datasets when page overflow in NVRAM
*/
#define ZB_NVRAM_DATASET_MIGRATION

/** Save address map in the NVRAM */
#define ZB_STORE_ADDR_MAP

/** Save neighbor table in the NVRAM */
#define ZB_STORE_NEIGHBOR_TBL



#ifndef ZB_LITE_NO_NVRAM_DATASET_VERSIONING
#define ZB_NVRAM_DATASET_OWN_VERSIONING
#endif

/* Check for old GP definitions and produce a error */
#if defined ENABLE_ZGP_TARGET_PLUS || defined ZB_ENABLE_ZGP_EP || defined ZB_DIRECT_MODE_WITH_ZGPD
#error Old ZGP define detected!
#endif

/* ZBOSS Lite defines */

/** Do not allow ZR to join as ZED by default. */
#define ZB_LITE_NO_JOIN_ZR_AS_ZED

/* Since R21 NWK multicast should be turned off by default */
#define ZB_LITE_NO_NWK_MULTICAST

/*
  If defined maximum possible features cut
 */

#ifdef ZB_ZBOSS_LITE_MAX
#ifndef ZB_LITE_NO_SOURCE_ROUTING
/**
   Disable source routing
 */
#define ZB_LITE_NO_SOURCE_ROUTING
#endif

#ifndef ZB_LITE_NO_NLME_ROUTE_DISCOVERY
/**
   Disable route discovery API (but keep implicit route discovery)
 */
#define ZB_LITE_NO_NLME_ROUTE_DISCOVERY
#endif

#ifndef ZB_NO_NWK_MULTICAST
/**
   Disable NWK multicast. Use APS groups and NWK broadcast instead.
 */
#define ZB_NO_NWK_MULTICAST
#endif

#ifndef ZB_LITE_NO_PANID_CONFLICT_DETECTION
/**
   Disable Pan ID conflict detection
 */
#define ZB_LITE_NO_PANID_CONFLICT_DETECTION
#endif

#ifndef ZB_LITE_NO_PANID_CONFLICT_RECEIVING
/**
   Disable Pan ID conflict command receiving
 */
#define ZB_LITE_NO_PANID_CONFLICT_RECEIVING
#endif


#ifdef ZB_LITE_NO_LINK_COST
/**
   Disable nwk link cost based on lqi

   See 3.6.3.1 Routing Cost
 */
///not sure this is good idea #define ZB_LITE_NO_LINK_COST
#endif

#ifndef ZB_LITE_NO_JOIN_ZR_AS_ZED
/**
   Do not attempt to join ZR as ZED if join as ZR failed.
   Do not join as ZED if joining net is prior PRO.
 */
#define ZB_LITE_NO_JOIN_ZR_AS_ZED
#endif

#ifndef ZB_LITE_NO_ORPHAN_SCAN
//#define ZB_LITE_NO_ORPHAN_SCAN
#endif


#ifndef ZB_LITE_NO_APS_DATA_ENCRYPTION
/**
   Do not encrypt data at APS level
 */
#define ZB_LITE_NO_APS_DATA_ENCRYPTION
#endif

#ifndef ZB_LITE_NO_STORE_APS_COUNTERS
/**
  Do not store APS security frame counters in nvram
 */
#define ZB_LITE_NO_STORE_APS_COUNTERS
#endif

/**
   Do not check for APS security counters inconsistence at unsecure
 */
#ifndef ZB_NO_CHECK_INCOMING_SECURE_APS_FRAME_COUNTERS
#define ZB_NO_CHECK_INCOMING_SECURE_APS_FRAME_COUNTERS
#endif

#ifdef ZB_DISTRIBUTED_SECURITY_ON
#undef ZB_DISTRIBUTED_SECURITY_ON
#endif

#ifdef ZB_SECURITY_INSTALLCODES
#undef ZB_SECURITY_INSTALLCODES
#endif

#ifdef ZB_CERTIFICATION_HACKS
#undef ZB_CERTIFICATION_HACKS
#endif


#ifndef ZB_LITE_NO_UNICAST_SWITCH_KEY
/**
   Do not send switch key unicast
 */
#define ZB_LITE_NO_UNICAST_SWITCH_KEY
#endif


#ifndef ZB_LITE_NO_GLOBAL_VS_UNIQUE_KEYS
/**
   Treat all APS keys as Unique.
(Unique vs Global is brain-damaging flag to define encryption/non encryption at
APS of some APS commands, like Update Device. It introduced in r20 probably for
compatibility with some mammoth shit */
//#define ZB_LITE_NO_GLOBAL_VS_UNIQUE_KEYS
#endif


#ifndef ZB_LITE_NO_TRUST_CENTER_REQUIRE_KEY_EXCHANGE
/**
   Never require unique keys exchange on join
 */
#define ZB_LITE_NO_TRUST_CENTER_REQUIRE_KEY_EXCHANGE
#endif

#ifndef ZB_LITE_NO_ZDO_SYSTEM_SERVER_DISCOVERY
/**
   Exclude ZDO System Server Discovery
 */
#define ZB_LITE_NO_ZDO_SYSTEM_SERVER_DISCOVERY
#endif

#ifndef ZB_LITE_NO_ZDO_MGMT_RTG
#define ZB_LITE_NO_ZDO_MGMT_RTG
#endif

#ifndef ZB_LITE_NO_FULL_FUNCLIONAL_MGMT_NWK_UPDATE
#define ZB_LITE_NO_FULL_FUNCLIONAL_MGMT_NWK_UPDATE
#endif

/**
   Do not try to inject LEAVE on data request from ZED which we already timed out.

   That define removes stupid r21 feature which requires from us to always set
   pending bit in any our real devices (just because we are a) not so fast to check
   _absence_ of device in the neighbor table and replay with MAC ACK and b) want
   to use auto-ack feature of radio which support it - like TI devices).
 */
#ifndef ZB_LITE_NO_LEAVE_INJECTION
#define ZB_LITE_NO_LEAVE_INJECTION
#endif
#ifdef ZB_LITE_NO_LEAVE_INJECTION
#ifdef ZB_MAC_STICKY_PENDING_BIT
#undef ZB_MAC_STICKY_PENDING_BIT
#endif
#endif

#ifndef ZB_LITE_NO_CONFIGURABLE_LINK_STATUS
#define ZB_LITE_NO_CONFIGURABLE_LINK_STATUS
#endif

#ifdef ZB_COORDINATOR_ONLY
#ifdef ZB_DISTRIBUTED_SECURITY_ON
#undef ZB_DISTRIBUTED_SECURITY_ON
#endif
#endif

#ifndef ZB_LITE_NO_INDIRECT_MGMT_LEAVE
/**
   Disable mgmt leave with requires sending leave to third device.
 */
#define ZB_LITE_NO_INDIRECT_MGMT_LEAVE
/* Need to store only one entry - about leave myself */
#ifdef ZB_ZDO_PENDING_LEAVE_SIZE
#undef ZB_ZDO_PENDING_LEAVE_SIZE
#define ZB_ZDO_PENDING_LEAVE_SIZE 1
#endif
#endif

#ifndef ZB_LITE_APS_DONT_TX_PACKET_TO_MYSELF
/**
   Don't pass from APS up packet which is sent to myself - either directly or
   via group.
 */
#define ZB_LITE_APS_DONT_TX_PACKET_TO_MYSELF
#endif


#ifndef ZB_LITE_BDB_ONLY_COMMISSIONING
/**
   Allow only BDB commissioning and disallow old ZBOSS start sequence
 */
#define ZB_LITE_BDB_ONLY_COMMISSIONING
#endif
#if defined ZB_LITE_BDB_ONLY_COMMISSIONING && !defined ZB_BDB_MODE
#define ZB_BDB_MODE
#endif


#ifndef ZB_LITE_LIMIT_PIB_ACCESS
/**
   Limit access to PIB to values really used by the stack
 */
#define ZB_LITE_LIMIT_PIB_ACCESS
#endif

#ifndef ZB_LITE_ALWAYS_SECURE
/**
   No ability to switch off security
 */
#define ZB_LITE_ALWAYS_SECURE
#endif

#ifndef ZB_LITE_DONT_STORE_RSSI
/**
   Do not store rssi in the neighbor
 */
//#define ZB_LITE_DONT_STORE_RSSI
//#define ZB_LITE_DONT_STORE_RSSI
#endif

/**
   Disable end device bind
 */
#ifndef ZB_LITE_NO_END_DEVICE_BIND
#define ZB_LITE_NO_END_DEVICE_BIND
#endif

#ifdef ZB_TEST_PROFILE
#undef ZB_TEST_PROFILE
#endif

#endif  /* ZB_ZBOSS_LITE_MAX */


#ifdef ZB_COORDINATOR_ONLY
#undef ZB_DISTRIBUTED_SECURITY_ON
#endif

#if defined ZB_TRAFFIC_DUMP_OFF && defined ZB_TRAFFIC_DUMP_ON
#undef ZB_TRAFFIC_DUMP_ON
#endif


#ifndef ZB_NO_NWK_MULTICAST
/**
   Disable NWK multicast. Deprecated in R21.
   Use APS groups and NWK broadcast instead.
 */
#define ZB_NO_NWK_MULTICAST
#endif


#ifdef ZB_CONFIGURABLE_MEM

/*
  Huge feature if configuring RAM usage without ZBOSS library recompiling.
  Instead of normal static declarations use pointers.
  At init time set pointers to be used instead of static arrays used normally.
  Keep possibility to compile in a normal way; also keep code mergeable.
  Undefine here array size constants and define it to be variables. See zb_g_context.h for details.
*/
#ifdef ZB_IOBUF_POOL_SIZE
#undef ZB_IOBUF_POOL_SIZE
#endif
#ifdef ZB_SCHEDULER_Q_SIZE
#undef ZB_SCHEDULER_Q_SIZE
#endif
#ifdef ZB_N_APS_KEY_PAIR_ARR_MAX_SIZE
#undef ZB_N_APS_KEY_PAIR_ARR_MAX_SIZE
#endif
#ifdef ZB_IEEE_ADDR_TABLE_SIZE
#undef ZB_IEEE_ADDR_TABLE_SIZE
#endif
#ifdef ZB_NEIGHBOR_TABLE_SIZE
#undef ZB_NEIGHBOR_TABLE_SIZE
#endif
#ifdef ZB_NWK_ROUTING_TABLE_SIZE
#undef ZB_NWK_ROUTING_TABLE_SIZE
#endif
#ifdef ZB_APS_DUPS_TABLE_SIZE
#undef ZB_APS_DUPS_TABLE_SIZE
#endif
#ifdef ZB_APS_SRC_BINDING_TABLE_SIZE
#undef ZB_APS_SRC_BINDING_TABLE_SIZE
#endif
#ifdef ZB_APS_DST_BINDING_TABLE_SIZE
#undef ZB_APS_DST_BINDING_TABLE_SIZE
#endif
#ifdef ZB_APS_BIND_TRANS_TABLE_SIZE
#undef ZB_APS_BIND_TRANS_TABLE_SIZE
#endif
#ifdef ZB_SINGLE_TRANS_INDEX_SIZE
#undef ZB_SINGLE_TRANS_INDEX_SIZE
#endif
/* 10/21/2019 EE CR:MINOR There was #undef ZB_SCHEDULER_Q_SIZE_PROTECTED_STACK_POOL. Was its removal intended? */
#ifdef ZB_NWK_MAX_SRC_ROUTES
#undef ZB_NWK_MAX_SRC_ROUTES
#endif

#else
/**
   The purpose of the define. Ret code handling implementation on the application side
   (via ZB_SCHEDULE_USER_APP_ALARM and ZB_SCHEDULE_USER_APP_CALLBACK) implies that we have some part
   of the callback and alarm queues which can not be used from the user app and always should be reserved
   for stack schedule purposes. So, let's define this part as 10 (for both immediate callbacks and alarms)
   for all configurations.
 */

#define ZB_SCHEDULER_Q_SIZE_PROTECTED_STACK_POOL 10
#if (ZB_SCHEDULER_Q_SIZE - ZB_SCHEDULER_Q_SIZE_PROTECTED_STACK_POOL) < 6
#error The size of application scheduler queue is very small! Please, change ZB_SCHEDULER_Q_SIZE_PROTECTED_STACK_POOL, ZB_SCHEDULER_Q_SIZE to set it at least 6
#endif
#endif  /* ZB_CONFIGURABLE_MEMORY */

/* pending bit source matching intended for ZB_ROUTER_ROLE only */
#ifndef ZB_ROUTER_ROLE
#ifdef ZB_MAC_PENDING_BIT_SOURCE_MATCHING
#undef ZB_MAC_PENDING_BIT_SOURCE_MATCHING
#endif
#ifdef ZB_MAC_SOFTWARE_PB_MATCHING
#undef ZB_MAC_SOFTWARE_PB_MATCHING
#endif
#endif

#ifndef ZB_NO_SYSTEST_SUPPORT
/* That is legacy for some tests. Not used any more */
#define ZB_NO_SYSTEST_SUPPORT
#endif

#ifdef ZB_COORDINATOR_ONLY
/* ZLL/Touchlink can be only ZR and ZED */
#ifdef ZB_BDB_TOUCHLINK
#undef ZB_BDB_TOUCHLINK
#endif
#ifdef ZB_ENABLE_INTER_PAN_EXCHANGE
#undef ZB_ENABLE_INTER_PAN_EXCHANGE
#endif
#ifdef ZB_REJOIN_BACKOFF
#undef ZB_REJOIN_BACKOFF
#endif
#ifdef ZB_DISTRIBUTED_SECURITY_ON
#undef ZB_DISTRIBUTED_SECURITY_ON
#endif
#endif

#ifdef ZB_ED_ROLE
#define ZB_USE_SLEEP
#endif

/*! \addtogroup ZB_CONFIG */
/*! @{ */
/** Maximum buffer index. Buffer indexing starts from one. */
#define ZB_N_BUF_IDS (ZB_IOBUF_POOL_SIZE + 1)

#ifdef ZB_PRODUCTION_CONFIG
#define ZB_PRODUCTION_CONFIG_MAX_SIZE 128

typedef enum zb_production_config_version_e
{
  ZB_PRODUCTION_CONFIG_VERSION_1_0 = 0x01,
  ZB_PRODUCTION_CONFIG_VERSION_2_0 = 0x02,
} zb_production_config_version_t;

#define ZB_PRODUCTION_CONFIG_CURRENT_VERSION   ZB_PRODUCTION_CONFIG_VERSION_2_0
#endif /*ZB_PRODUCTION_CONFIG*/
/*! @} */

#if defined USE_ZB_WATCHDOG || defined ZB_MACSPLIT_DEVICE
#define ZB_NEVER_STOP_TIMER
#endif

#endif /* ZB_CONFIG_H */
 
