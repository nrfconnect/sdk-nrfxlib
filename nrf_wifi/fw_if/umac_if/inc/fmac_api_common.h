/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/** @file
 *
 * @addtogroup nrf_wifi_api_common FMAC API common
 * @{
 *
 * @brief Header containing API declarations for the
 * FMAC IF Layer of the Wi-Fi driver.
 */
#ifndef __FMAC_API_COMMON_H__
#define __FMAC_API_COMMON_H__

#include "osal_api.h"
#include "hal_api.h"
#include "host_rpu_umac_if.h"
#include "host_rpu_data_if.h"
#include "host_rpu_sys_if.h"

#include "fmac_structs.h"
#include "fmac_cmd.h"
#include "fmac_event.h"
#include "fmac_vif.h"
#include "fmac_bb.h"

#include <patch_info.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/**
 * @brief Adds a RPU instance.
 * @param fpriv Pointer to the context of the UMAC IF layer.
 * @param os_dev_ctx Pointer to the OS specific context of the RPU instance.
 *
 * This function adds an RPU instance. This function will return the
 *	    pointer to the context of the RPU instance. This pointer will need to be
 *	    supplied while invoking further device specific APIs,
 *	    for example, nrf_wifi_fmac_scan() etc.
 *
 * @return Pointer to the context of the RPU instance.
 */
struct nrf_wifi_fmac_dev_ctx *nrf_wifi_fmac_dev_add(struct nrf_wifi_fmac_priv *fpriv,
						    void *os_dev_ctx);

/**
 * @brief Issue a request to get stats from the RPU.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param op_mode RPU operation mode.
 * @param stats Pointer to memory where the stats are to be copied.
 *
 * This function is used to send a command to
 *	    instruct the firmware to return the current RPU statistics. The RPU will
 *	    send the event with the current statistics.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_stats_get(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					     enum rpu_op_mode op_mode,
					     struct rpu_op_stats *stats);


/**
 * @brief Validate the firmware header.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param info Pointer to the address where the firmware information needs to be copied.
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_validate_fw_header(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
						 struct nrf70_fw_image_info *info);

/**
 * @brief Parse the Firmware(s) to be loaded to the RPU WLAN device.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device,
 *	which was passed as \p fmac_dev_ctx parameter via the
 *	\p add_dev_callbk_fn() callback function.
 * @param fw_data Pointer to the address where the firmware data is available.
 * @param fw_size Size of the firmware data.
 * @param fw_info Pointer to the address where the firmware information needs to be copied.
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_fw_parse(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					   const void *fw_data,
					   unsigned int fw_size,
					   struct nrf_wifi_fmac_fw_info *fw_info);


/**
 * @brief Reset the processors in the RPU WLAN device.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device,
 *      which was passed as \p fmac_dev_ctx parameter via the
 *      \p add_dev_callbk_fn() callback function.
 *
 * This function resets the processors in the RPU WLAN device.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_fw_reset(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx);

/**
 * @brief Boot the Firmware(s) to the RPU WLAN device.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device,
 *      which was passed as \p fmac_dev_ctx parameter via the
 *      \p add_dev_callbk_fn() callback function.
 *
 * This function boots the FullMAC firmware(s) to the RPU WLAN device and waits
 * for the boot to complete and validate the boot signature.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_fw_boot(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx);

/**
 * @brief Load a chunk of the Firmware to the RPU WLAN device.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device,
 *      which was passed as \p fmac_dev_ctx parameter via the
 *      \p add_dev_callbk_fn() callback function.
 * @param rpu_proc RPU processor type.
 * @param fw_chunk Pointer to the address where the firmware chunk information is available.
 *
 * This function loads a chunk of the firmware to the RPU WLAN device.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_fw_chunk_load(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					   enum RPU_PROC_TYPE rpu_proc,
					   struct nrf_wifi_fmac_fw_chunk_info *fw_chunk);

/**
 * @brief Loads the Firmware(s) to the RPU WLAN device.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device,
 *      which was passed as \p fmac_dev_ctx parameter via the
 *      \p add_dev_callbk_fn() callback function.
 * @param fmac_fw Information about the FullMAC firmware(s) to be loaded.
 *
 * This function loads the FullMAC firmware(s) to the RPU WLAN device.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_fw_load(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					   struct nrf_wifi_fmac_fw_info *fmac_fw);


/**
 * @brief Get FW versions from the RPU.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param fw_ver Pointer to the address where the FW versions needs to be copied.
 *
 * This function is used to get Firmware versions from the RPU.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_ver_get(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					  unsigned int *fw_ver);

/**
 * @brief Configure HE LTF and GI parameters.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param he_ltf HE LTF parameter which will be configured in RPU.
 * @param he_gi HE GI parameter which will be configured in RPU.
 * @param enabled enable/disable HE LTF and GI parameter configured.
 *
 * This function is used to send a command to RPU
 *	    to configure HE LTF and GI parameters in RPU.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_conf_ltf_gi(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					       unsigned char he_ltf,
					       unsigned char he_gi,
					       unsigned char enabled);

#if defined(CONFIG_NRF700X_UTIL) || defined(__DOXYGEN__)
enum nrf_wifi_status nrf_wifi_fmac_set_tx_rate(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					       unsigned char rate_flag,
					       int data_rate);
#if defined(CONFIG_NRF_WIFI_LOW_POWER) || defined(__DOXYGEN__)
/**
 * @brief Get the RPU power save status from host perspective.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param rpu_ps_ctrl_state Pointer to the address where the current RPU power save state
 *			    from host perspective needs to be copied.
 *
 * This function is used to fetch the RPU power save status
 *	    from host perspective.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_get_host_rpu_ps_ctrl_state(void *fmac_dev_ctx,
							      int *rpu_ps_ctrl_state);
#endif /* CONFIG_NRF_WIFI_LOW_POWER */
#endif /* CONFIG_NRF700X_UTIL */
/**
 * @brief Configure SR coexistence parameters in RPU.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param cmd SR coexistence parameters which will be configured in RPU.
 * @param cmd_len Command length.
 *
 * This function is used to send a command to RPU to configure
 *	    SR coexistence parameters in RPU.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_conf_srcoex(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					       void *cmd, unsigned int cmd_len);


/**
 * @brief Set the Multicast filter address.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param if_idx Index of the interface whose state needs to be changed.
 * @param mcast_info Multicast information to be set.
 *
 * This function is used to send a command (%NRF_WIFI_UMAC_CMD_MCAST_FILTER) to
 *	    instruct the firmware to set the multicast filter address to an interface
 *	    with index \p if_idx and parameters specified by \p mcast_info.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_set_mcast_addr(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
						  unsigned char if_idx,
						  struct nrf_wifi_umac_mcast_cfg *mcast_info);


/**
 * @brief Fetch MAC address from OTP.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param vif_idx Interface index for which the MAC address is to be fetched.
 * @param mac_addr Pointer to the address where the MAC address needs to be copied.
 *
 * This function is used to fetch MAC address from the OTP.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_otp_mac_addr_get(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
						    unsigned char vif_idx,
						    unsigned char *mac_addr);


/**
 * @brief Get the RF parameters to be programmed to the RPU.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param rf_params Pointer to the address where the RF params information needs to be copied.
 *
 * This function is used to fetch RF parameters information from the RPU and
 *	    update the default RF parameter with the OTP values. The updated RF
 *	    parameters are then returned in the \p f_params.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_rf_params_get(
					struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					struct nrf_wifi_phy_rf_params *rf_params);
/**
 * @brief Set regulatory domain in RPU.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param reg_info Pointer to the address where the regulatory domain information.
 *		   needs to be copied.
 * This function is used to set regulatory domain in the RPU.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_set_reg(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					   struct nrf_wifi_fmac_reg_info *reg_info);

/**
 * @brief Get regulatory domain from RPU.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param reg_info Pointer to the address where the regulatory domain information.
 *		   needs to be copied.
 * This function is used to get regulatory domain from the RPU.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_get_reg(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					   struct nrf_wifi_fmac_reg_info *reg_info);

/**
 * @brief Get the current power save information.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param if_idx Index of the interface on which power management is to be set.
 *
 * This function is used to send a command
 *	    to RPU to Enable/Disable WLAN Power management.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_get_power_save_info(void *fmac_dev_ctx,
						       unsigned char if_idx);

/**
 * @brief Initialize RF parameters.
 * @param opriv Pointer to the OSAL context.
 * @param prf Pointer to the RF parameter structure.
 * @param package_info Package information, QFN, CSP etc.
 * @param str String of RF params
 *
 * This function is used to initialize the RF parameter structure
 * with the XO, power ceiling info, voltage and temperature based
 * backoffs etc.
 */
int nrf_wifi_phy_rf_params_init(struct nrf_wifi_osal_priv *opriv,
				struct nrf_wifi_phy_rf_params *prf,
				unsigned int package_info,
				unsigned char *str);

#ifdef CONFIG_NRF700X_SYSTEM_WITH_RAW_MODES
/**
 * @brief Set the current mode of operation
 * @param dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param if_idx Index of the interface on which mode is to be set.
 * @param mode Value to be set for the interface.
 *
 * This function is used to send a command
 *         to RPU to set mode of operation
 *
 * @retval      WIFI_NRF_STATUS_SUCCESS On success
 * @retval      WIFI_NRF_STATUS_FAIL On failure
 */
enum nrf_wifi_status nrf_wifi_fmac_set_mode(void *dev_ctx,
					    unsigned char if_idx,
					    unsigned char mode);
#endif

#if defined(CONFIG_NRF700X_RAW_DATA_TX) || defined(CONFIG_NRF700X_RAW_DATA_RX)
/**
 * @brief Set the current channel
 * @param dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param if_idx Index of the interface on which mode is to be set.
 * @param channel Value to be set for the interface.
 *
 * This function is used to send a command
 *         to RPU to set current channel of operation
 *
 * @retval      WIFI_NRF_STATUS_SUCCESS On success
 * @retval      WIFI_NRF_STATUS_FAIL On failure
 */
enum nrf_wifi_status nrf_wifi_fmac_set_channel(void *dev_ctx,
					       unsigned char if_idx,
					       unsigned int channel);

#endif /* CONFIG_NRF700X_RAW_DATA_TX || CONFIG_NRF700X_RAW_DATA_RX */

#if defined(CONFIG_NRF700X_RAW_DATA_RX) || defined(CONFIG_NRF700X_PROMISC_DATA_RX)
/**
 * @brief Set packet filter settings
 * @param dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param filter Value to be set for the interface.
 * @param if_idx Index of the interface on which mode is to be set.
 * @param buffer_size Size of packet capture length.
 *
 * This function is used to send a command
 *         to RPU to set filter setting for sniffer operation
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_set_packet_filter(void *dev_ctx, unsigned char filter,
						     unsigned char if_idx,
						     unsigned short buffer_size);
#endif /* CONFIG_NRF700X_RAW_DATA_RX || CONFIG_NRF700X_PROMISC_DATA_RX */

/**
 * @}
 */
#endif /* __FMAC_API_COMMON_H__ */
