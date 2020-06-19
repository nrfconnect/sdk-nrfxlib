/*
 * Copyright (c) 2020 Nordic Semiconductor
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 *
 * This file implements mappings of Kconfig options enabling and configuring ZBOSS features.
 */

#ifndef ZB_VENDOR_H__
#define ZB_VENDOR_H__

/**
 * Include additional configuration file, which usually contains the set of
 * symbols defined by ZBOSS source code Kconfig, which can be found in NCS
 * platform repository.
 */
#ifdef LIBZBOSS_CONFIG_FILE
#include LIBZBOSS_CONFIG_FILE
#endif

#include "zb_config_platform.h"

#ifdef CONFIG_ZB_NCP_DEBUG
#define ZBNCP_DEBUG
#define ZBNCP_USE_ZBOSS_TRACE
#endif

#ifdef CONFIG_ZB_NCP_TRANSPORT_TYPE_SERIAL
#define ZB_NCP_TRANSPORT_TYPE_SERIAL
#endif

#ifdef CONFIG_ZB_NCP_TRANSPORT_TYPE_SPI
#define ZB_NCP_TRANSPORT_TYPE_SPI
#endif

#ifdef CONFIG_ZB_NCP_TRANSPORT_TYPE_NSNG
#define ZB_NCP_TRANSPORT_TYPE_NSNG
#endif

#ifdef CONFIG_ZB_TRACE_LOG_LEVEL
#define ZB_TRACE_LEVEL CONFIG_ZB_TRACE_LOG_LEVEL
#endif

#ifdef CONFIG_ZB_TRACE_MASK
#define ZB_TRACE_MASK CONFIG_ZB_TRACE_MASK
#endif

#ifdef CONFIG_ZB_NVRAM_ENABLE_VER_MIGRATION
#define ZB_NVRAM_ENABLE_VER_MIGRATION
#else
#define ZB_NO_NVRAM_VER_MIGRATION
#endif

#ifdef CONFIG_ZB_BDB_ENABLE_FINDING_BINDING
#define ZB_BDB_ENABLE_FINDING_BINDING
#endif

#ifdef CONFIG_ZB_DISTRIBUTED_SECURITY_ON
#define ZB_DISTRIBUTED_SECURITY_ON
#endif

#ifdef CONFIG_ZB_SECURITY_INSTALLCODES
#define ZB_SECURITY_INSTALLCODES
#endif

#ifdef CONFIG_ZB_PRODUCTION_CONFIG
#define ZB_PRODUCTION_CONFIG
#endif

#ifdef CONFIG_ZB_APS_USER_PAYLOAD
#define ZB_APS_USER_PAYLOAD
#endif

#ifdef CONFIG_ZB_LOW_SECURITY_MODE
#define ZB_LOW_SECURITY_MODE
#endif

#ifdef CONFIG_ZB_APS_FRAGMENTATION
#define APS_FRAGMENTATION
#endif

#ifdef CONFIG_ZB_NWK_BLACKLIST
#define ZB_NWK_BLACKLIST

#ifdef CONFIG_ZB_NWK_BLACKLIST_SIZE
#define ZB_NWK_BLACKLIST_SIZE CONFIG_ZB_NWK_BLACKLIST_SIZE
#endif

#endif /* CONFIG_ZB_NWK_BLACKLIST */

#ifdef CONFIG_ZB_ZCL_SUPPORT_CLUSTER_WWAH
#define ZB_R22_MULTIMAC
#define ZB_ZCL_SUPPORT_CLUSTER_WWAH

#ifdef CONFIG_ZB_ZCL_ENABLE_WWAH_CLIENT
#define ZB_ZCL_ENABLE_WWAH_CLIENT
#endif

#ifdef CONFIG_ZB_ZCL_ENABLE_WWAH_SERVER
#define ZB_ZCL_ENABLE_WWAH_SERVER

#ifdef CONFIG_ZB_NWK_RETRY_COUNT
#define ZB_NWK_RETRY_COUNT
#endif

#ifdef CONFIG_ZB_BEACON_SURVEY
#define ZB_BEACON_SURVEY
#endif

#ifdef CONFIG_ZB_PARENT_CLASSIFICATION
#define ZB_PARENT_CLASSIFICATION
#endif

#elif defined(CONFIG_ZB_NWK_RETRY_COUNT) || defined(CONFIG_ZB_BEACON_SURVEY) || defined(CONFIG_ZB_PARENT_CLASSIFICATION)
#error ZBOSS configuration mismatch: You have to enable WWAHu cluster server implementation to use additional WWAHu features.

#endif /* ZB_ZCL_ENABLE_WWAH_SERVER */

#endif /* CONFIG_ZB_ZCL_SUPPORT_CLUSTER_WWAH */

#ifdef CONFIG_ZB_CHECK_OOM_STATUS
#define ZB_CHECK_OOM_STATUS
#endif

#ifdef CONFIG_ZB_SEND_OOM_STATUS
#define ZB_SEND_OOM_STATUS
#endif

#ifdef CONFIG_ZB_PROMISCUOUS_MODE
#define ZB_PROMISCUOUS_MODE
#endif

#ifdef CONFIG_ZB_CERTIFICATION_HACKS
#define ZB_CERTIFICATION_HACKS
#endif

#ifdef CONFIG_ZB_TEST_PROFILE
#define ZB_TEST_PROFILE
#endif

#ifdef CONFIG_ZB_MULTITEST_CONSOLE_SLEEP_TIMEOUT
#define ZB_MULTITEST_CONSOLE_SLEEP_TIMEOUT CONFIG_ZB_MULTITEST_CONSOLE_SLEEP_TIMEOUT
#endif

#ifdef CONFIG_ZB_DONT_NEED_TRACE_FILE_ID
#define ZB_DONT_NEED_TRACE_FILE_ID
#endif

#ifdef CONFIG_ZB_MAC_AUTO_DELAY_IN_MAC_GP_SEND
#define MAC_AUTO_DELAY_IN_MAC_GP_SEND
#endif

#ifdef CONFIG_ZB_USEALIAS
#define ZB_USEALIAS
#endif

#ifdef CONFIG_ZB_LIMIT_VISIBILITY
#define ZB_LIMIT_VISIBILITY
#endif

#ifdef CONFIG_ZB_BDB_TOUCHLINK
#define ZB_ENABLE_INTER_PAN_EXCHANGE
#define ZB_ENABLE_ZLL
#define ZB_BDB_TOUCHLINK
#endif

#ifdef CONFIG_ZB_CONFIGURABLE_MEM
#define ZB_CONFIGURABLE_MEM
#endif

/* Default memory storage configuration, used while dynamic memory configuration features is enabled. */
#ifdef ZB_CONFIG_DEFAULT_KERNEL_DEFINITION

/* Select memory configuration according to the implemented Zigbee role. */
#ifndef ZB_ED_ROLE
#define ZB_CONFIG_ROLE_ZC
#else
#define ZB_CONFIG_ROLE_ZED
#endif

#ifdef CONFIG_ZB_CONFIG_OVERALL_NETWORK_SIZE
#define ZB_CONFIG_OVERALL_NETWORK_SIZE CONFIG_ZB_CONFIG_OVERALL_NETWORK_SIZE
#endif

#ifdef CONFIG_ZB_CONFIG_HIGH_TRAFFIC
#define ZB_CONFIG_HIGH_TRAFFIC
#endif

#ifdef CONFIG_ZB_CONFIG_MODERATE_TRAFFIC
#define ZB_CONFIG_MODERATE_TRAFFIC
#endif

#ifdef CONFIG_ZB_CONFIG_LIGHT_TRAFFIC
#define ZB_CONFIG_LIGHT_TRAFFIC
#endif

#ifdef CONFIG_ZB_CONFIG_APPLICATION_COMPLEX
#define ZB_CONFIG_APPLICATION_COMPLEX
#endif

#ifdef CONFIG_ZB_CONFIG_APPLICATION_MODERATE
#define ZB_CONFIG_APPLICATION_MODERATE
#endif

#ifdef CONFIG_ZB_CONFIG_APPLICATION_SIMPLE
#define ZB_CONFIG_APPLICATION_SIMPLE
#endif

#endif /* ZB_CONFIG_DEFAULT_KERNEL_DEFINITION */

/* Compile-time parameters, affecting memory usage in all configurations. */
#define ZB_PANID_TABLE_SIZE              CONFIG_ZB_PANID_TABLE_SIZE
#define ZB_DEV_MANUFACTORER_TABLE_SIZE   CONFIG_ZB_DEV_MANUFACTURER_TABLE_SIZE
#define ZB_BUF_Q_SIZE                    CONFIG_ZB_BUF_Q_SIZE
#define ZDO_TRAN_TABLE_SIZE              CONFIG_ZB_ZDO_TRAN_TABLE_SIZE
#define ZB_APS_ENDPOINTS_IN_GROUP_TABLE  CONFIG_ZB_APS_ENDPOINTS_IN_GROUP_TABLE
#define ZB_NWK_BTR_TABLE_SIZE            CONFIG_ZB_NWK_BTR_TABLE_SIZE
#define ZB_NWK_BRR_TABLE_SIZE            CONFIG_ZB_NWK_BRR_TABLE_SIZE
#define ZB_MAX_EP_NUMBER                 CONFIG_ZB_MAX_EP_NUMBER
#define ZB_APS_GROUP_TABLE_SIZE          CONFIG_ZB_APS_GROUP_TABLE_SIZE
#define ZB_ZGP_SINK_TBL_SIZE             CONFIG_ZB_ZGP_SINK_TBL_SIZE
#define ZB_ZGP_PROXY_TBL_SIZE            CONFIG_ZB_ZGP_PROXY_TBL_SIZE

#ifdef CONFIG_ZB_APS_SRC_BINDING_TABLE_SIZE
#define ZB_APS_SRC_BINDING_TABLE_SIZE CONFIG_ZB_APS_SRC_BINDING_TABLE_SIZE
#endif

#ifdef CONFIG_ZB_APS_DST_BINDING_TABLE_SIZE
#define ZB_APS_DST_BINDING_TABLE_SIZE CONFIG_ZB_APS_DST_BINDING_TABLE_SIZE
#endif

#ifdef CONFIG_ZB_IOBUF_POOL_SIZE
#define ZB_IOBUF_POOL_SIZE CONFIG_ZB_IOBUF_POOL_SIZE
#endif

#ifdef CONFIG_ZB_SCHEDULER_Q_SIZE
#define ZB_SCHEDULER_Q_SIZE CONFIG_ZB_SCHEDULER_Q_SIZE
#endif

#ifndef ZB_CONFIGURABLE_MEM
#define ZB_CHILD_HASH_TABLE_SIZE (((ZB_IEEE_ADDR_TABLE_SIZE + ZB_IEEE_ADDR_TABLE_SIZE / 3) + 31) / 32 * 32)
#endif /* ZB_CONFIGURABLE_MEM */


/* ZBOSS stack options non-configurable via Kconfig. */

/* Include Zigbee BDB implementation (always defined, except macsplit). */
#define ZB_BDB_MODE

/* Include Zigbee HA clusters implementation (always defined, except macsplit). */
#define ZB_ENABLE_HA

/* Include all HA device definitions. For a better granularity, please take a look at the list in zb_ha_device_config.h file (always defined, except macsplit). */
#define ZB_ALL_DEVICE_SUPPORT

/* Include rejoin backoff procedure (always included, except macsplit). */
#define ZB_REJOIN_BACKOFF

/* Enable ZGP profile  (always defined, except macsplit). */
#define ZB_ENABLE_ZGP

/* Enable sleepy behaviour on all devices. The sleep signal will allow other, lower-priority task to take over. */
#ifndef ZB_USE_SLEEP
#define ZB_USE_SLEEP
#endif

/* Enter TX mode directly from radio sleep sate. */
#define ZB_TRANSCEIVER_ON_BEFORE_TX  0

/* Start Energy Detection from radio sleep state. */
#define ZB_TRANSCEIVER_ON_BEFORE_ED  0

#endif /* ZB_VENDOR_H__ */
