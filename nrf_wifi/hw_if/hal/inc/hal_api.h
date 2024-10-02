/**
 * @file hal_api.h
 *
 * @brief Header containing API declarations for the HAL Layer of the Wi-Fi driver.
 */

#ifndef __HAL_API_H__
#define __HAL_API_H__

#include "osal_api.h"
#include "rpu_if.h"
#include "bal_api.h"
#include "hal_structs.h"
#include "hal_mem.h"
#include "hal_reg.h"
#include "hal_fw_patch_loader.h"

#define NRF_WIFI_ADDR_REG_NAME_LEN		16

/**
 * @brief Structure containing information about the firmware address.
 */
struct nrf70_fw_addr_info {
	/** RPU process type. */
	enum RPU_PROC_TYPE rpu_proc;
	/** Name of the address register. */
	char name[NRF_WIFI_ADDR_REG_NAME_LEN];
	/** Destination address. */
	unsigned int dest_addr;
};

extern const struct nrf70_fw_addr_info nrf70_fw_addr_info[];

/**
 * @brief Initialize the HAL layer.
 *
 * @param cfg_params Parameters needed to configure the HAL for WLAN operation.
 * @param intr_callbk_fn Pointer to the callback function which the user of this
 *                       layer needs to implement to handle events from the RPU.
 * @param rpu_recovery_callbk_fn Pointer to the callback function which the user of
 *                               this layer needs to implement to handle RPU recovery.
 *
 * This API is used to initialize the HAL layer and is expected to be called
 * before using the HAL layer. This API returns a pointer to the HAL context
 * which might need to be passed to further API calls.
 *
 * @return Pointer to instance of HAL layer context.
 */
struct nrf_wifi_hal_priv *
nrf_wifi_hal_init(struct nrf_wifi_hal_cfg_params *cfg_params,
		  enum nrf_wifi_status (*intr_callbk_fn)(void *mac_ctx,
							 void *event_data,
							 unsigned int len),
		  enum nrf_wifi_status (*rpu_recovery_callbk_fn)(void *mac_ctx,
							     void *event_data,
							     unsigned int len));

/**
 * @brief Deinitialize the HAL layer.
 *
 * @param hpriv Pointer to the HAL context returned by the nrf_wifi_hal_init API.
 *
 * This API is used to deinitialize the HAL layer and is expected to be called
 * after done using the HAL layer.
 *
 * @return None.
 */
void nrf_wifi_hal_deinit(struct nrf_wifi_hal_priv *hpriv);

struct nrf_wifi_hal_dev_ctx *nrf_wifi_hal_dev_add(struct nrf_wifi_hal_priv *hpriv,
						  void *mac_dev_ctx);


void nrf_wifi_hal_dev_rem(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx);


enum nrf_wifi_status nrf_wifi_hal_dev_init(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx);


void nrf_wifi_hal_dev_deinit(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx);

void nrf_wifi_hal_lock_rx(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx);

void nrf_wifi_hal_unlock_rx(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx);

void nrf_wifi_hal_enable(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx);
void nrf_wifi_hal_disable(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx);
enum NRF_WIFI_HAL_STATUS nrf_wifi_hal_status_unlocked(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx);



/**
 * @brief Send a control command to the RPU.
 *
 * @param hal_ctx Pointer to HAL context.
 * @param cmd Pointer to command data.
 * @param cmd_size Size of the command data pointed to by @cmd.
 *
 * This function takes care of sending a command to the RPU. It does the
 * following:
 *
 *     - Fragments a command into smaller chunks if the size of a command is
 *       greater than %MAX_CMD_SIZE.
 *     - Queues up the command(s) to the HAL command queue.
 *     - Calls a function to further process the commands queued up in the HAL
 *       command queue which handles it by:
 *
 *           - Waiting for the RPU to be ready to handle a command.
 *           - Copies the command to the GRAM memory and indicates to the RPU
 *             that a command has been posted
 *
 * @return The status of the operation.
 */
enum nrf_wifi_status nrf_wifi_hal_ctrl_cmd_send(struct nrf_wifi_hal_dev_ctx *hal_ctx,
						void *cmd,
						unsigned int cmd_size);


/**
 * @brief Send a data command to the RPU.
 *
 * @param hal_ctx Pointer to HAL context.
 * @param cmd_type Type of the data command to send to the RPU.
 * @param data_cmd The data command to be sent to the RPU.
 * @param data_cmd_size Size of the data command to be sent to the RPU.
 * @param desc_id Descriptor ID of the buffer being submitted to RPU.
 * @param pool_id Pool ID to which the buffer being submitted to RPU belongs.
 *
 * This function programs the relevant information about a data command,
 * to the RPU. These buffers are needed by the RPU to receive data and
 * management frames as well as to transmit data frames.
 *
 * @return The status of the operation.
 */
enum nrf_wifi_status nrf_wifi_hal_data_cmd_send(struct nrf_wifi_hal_dev_ctx *hal_ctx,
						enum NRF_WIFI_HAL_MSG_TYPE cmd_type,
						void *data_cmd,
						unsigned int data_cmd_size,
						unsigned int desc_id,
						unsigned int pool_id);

/**
 * @brief Process events from the RPU.
 *
 * @param hpriv Pointer to HAL context.
 *
 * This function processes the events which have been queued into the event
 * queue by an ISR. It does the following:
 *
 *     - Dequeues an event from the event queue.
 *     - Calls hal_event_process to further process the event.
 *
 * @return The status of the operation.
 */
enum nrf_wifi_status hal_rpu_eventq_process(struct nrf_wifi_hal_dev_ctx *hal_ctx);


/**
 * @brief Map a receive buffer for the Wi-Fi HAL.
 *
 * This function maps a receive buffer to the Wi-Fi HAL device context.
 *
 * @param hal_ctx   Pointer to the Wi-Fi HAL device context.
 * @param buf       The buffer to be mapped.
 * @param buf_len   The length of the buffer.
 * @param pool_id   The pool ID of the buffer.
 * @param buf_id    The buffer ID.
 *
 * @return The status of the operation.
 */
unsigned long nrf_wifi_hal_buf_map_rx(struct nrf_wifi_hal_dev_ctx *hal_ctx,
									  unsigned long buf,
									  unsigned int buf_len,
									  unsigned int pool_id,
									  unsigned int buf_id);

/**
 * @brief Unmap a receive buffer from the Wi-Fi HAL.
 *
 * This function unmaps a receive buffer from the Wi-Fi HAL device context.
 *
 * @param hal_ctx     Pointer to the Wi-Fi HAL device context.
 * @param data_len    The length of the data.
 * @param pool_id     The pool ID of the buffer.
 * @param buf_id      The buffer ID.
 *
 * @return The status of the operation.
 */
unsigned long nrf_wifi_hal_buf_unmap_rx(struct nrf_wifi_hal_dev_ctx *hal_ctx,
	unsigned int data_len,
	unsigned int pool_id,
	unsigned int buf_id);

/**
 * @brief Map a transmit buffer for the Wi-Fi HAL.
 *
 * This function maps a transmit buffer to the Wi-Fi HAL device context.
 *
 * @param hal_ctx     Pointer to the Wi-Fi HAL device context.
 * @param buf         The buffer to be mapped.
 * @param buf_len     The length of the buffer.
 * @param desc_id     The descriptor ID.
 * @param token       The token.
 * @param buf_indx    The buffer index.
 *
 * @return The status of the operation.
 */
unsigned long nrf_wifi_hal_buf_map_tx(struct nrf_wifi_hal_dev_ctx *hal_ctx,
									  unsigned long buf,
									  unsigned int buf_len,
									  unsigned int desc_id,
									  unsigned int token,
									  unsigned int buf_indx);

/**
 * @brief Unmap a transmit buffer from the Wi-Fi HAL.
 *
 * This function unmaps a transmit buffer from the Wi-Fi HAL device context.
 *
 * @param hal_ctx     Pointer to the Wi-Fi HAL device context.
 * @param desc_id     The descriptor ID.
 *
 * @return The status of the operation.
 */
unsigned long nrf_wifi_hal_buf_unmap_tx(struct nrf_wifi_hal_dev_ctx *hal_ctx,
			unsigned int desc_id);

/**
 * @brief Set the processing context for the Wi-Fi HAL.
 *
 * This function sets the processing context for the Wi-Fi HAL device context.
 *
 * @param hal_ctx     Pointer to the Wi-Fi HAL device context.
 * @param proc        The processing type.
 */
void nrf_wifi_hal_proc_ctx_set(struct nrf_wifi_hal_dev_ctx *hal_ctx,
							   enum RPU_PROC_TYPE proc);

/**
 * @brief Reset the processing context for the Wi-Fi HAL.
 *
 * This function resets the processing context for the Wi-Fi HAL device context.
 *
 * @param hal_ctx     Pointer to the Wi-Fi HAL device context.
 * @param rpu_proc    The RPU processing type.
 *
 * @return The status of the operation.
 */
enum nrf_wifi_status nrf_wifi_hal_proc_reset(struct nrf_wifi_hal_dev_ctx *hal_ctx,
			enum RPU_PROC_TYPE rpu_proc);

/**
 * @brief Check the boot status of the firmware for the Wi-Fi HAL.
 *
 * This function checks the boot status of the firmware for the Wi-Fi HAL device context.
 *
 * @param hal_ctx     Pointer to the Wi-Fi HAL device context.
 * @param rpu_proc    The RPU processing type.
 *
 * @return The status of the operation.
 */
enum nrf_wifi_status nrf_wifi_hal_fw_chk_boot(struct nrf_wifi_hal_dev_ctx *hal_ctx,
			enum RPU_PROC_TYPE rpu_proc);

#if defined(CONFIG_NRF_WIFI_LOW_POWER) || defined(__DOXYGEN__)
/**
 * @brief Wake up the RPU power save mode for the Wi-Fi HAL.
 *
 * This function wakes up the RPU power save mode for the Wi-Fi HAL device context.
 *
 * @param hal_dev_ctx     Pointer to the Wi-Fi HAL device context.
 *
 * @return The status of the operation.
 */
enum nrf_wifi_status hal_rpu_ps_wake(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx);

/**
 * @brief Get the RPU power save state for the Wi-Fi HAL.
 *
 * This function gets the RPU power save state for the Wi-Fi HAL device context.
 *
 * @param hal_dev_ctx         Pointer to the Wi-Fi HAL device context.
 * @param rpu_ps_ctrl_state   Pointer to the RPU power save control state.
 *
 * @return The status of the operation.
 */
enum nrf_wifi_status nrf_wifi_hal_get_rpu_ps_state(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx,
			int *rpu_ps_ctrl_state);
#endif /* CONFIG_NRF_WIFI_LOW_POWER */

/**
 * @brief Get the OTP information for the Wi-Fi HAL.
 *
 * This function gets the OTP (One-Time Programmable) information for the Wi-Fi HAL device context.
 *
 * @param hal_dev_ctx     Pointer to the Wi-Fi HAL device context.
 * @param otp_info        Pointer to the OTP information structure.
 * @param otp_flags       Pointer to the OTP flags.
 *
 * @return The status of the operation.
 */
enum nrf_wifi_status nrf_wifi_hal_otp_info_get(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx,
			struct host_rpu_umac_info *otp_info,
			unsigned int *otp_flags);

/**
 * @brief Get the OTP firmware programming version for the Wi-Fi HAL.
 *
 * This function gets the OTP firmware programming version for the Wi-Fi HAL device context.
 *
 * @param hal_dev_ctx     Pointer to the Wi-Fi HAL device context.
 * @param ft_prog_ver     Pointer to the firmware programming version.
 *
 * @return The status of the operation.
 */
enum nrf_wifi_status nrf_wifi_hal_otp_ft_prog_ver_get(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx,
			unsigned int *ft_prog_ver);

/**
 * @brief Get the OTP package information for the Wi-Fi HAL.
 *
 * This function gets the OTP package information for the Wi-Fi HAL device context.
 *
 * @param hal_dev_ctx     Pointer to the Wi-Fi HAL device context.
 * @param package_info    Pointer to the package information.
 *
 * @return The status of the operation.
 */
enum nrf_wifi_status nrf_wifi_hal_otp_pack_info_get(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx,
			unsigned int *package_info);

#endif /* __HAL_API_H__ */
