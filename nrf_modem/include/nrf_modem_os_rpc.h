/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file nrf_modem_os_rpc.h
 *
 * @brief RPC transport glue.
 *
 * @defgroup nrf_modem_os_rpc Modem Library RPC transport glue
 * @{
 */
#ifndef NRF_MODEM_OS_RPC_H__
#define NRF_MODEM_OS_RPC_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*nrf_modem_os_rpc_signal_cb_t)(uint32_t ch, void *priv);

/**
 * @brief Parameters for configuring an nrf_modem rpc instance.
 *
 * Passed when invoking @c nrf_modem_os_rpc_open.
 */
struct nrf_modem_os_rpc_config {
	struct {
		/** Remote signal device. */
		uintptr_t sigdev;
		/** TX channel number. */
		uint32_t ch;
		/** TX shared memory start address. */
		uintptr_t addr;
		/** TX shared memory size. */
		size_t size;
	} tx;
	struct {
		/** Local signal device. */
		uintptr_t sigdev;
		/** RX channel number. */
		uint32_t ch;
		/** RX shared memory start address. */
		uintptr_t addr;
		/** RX shared memory size. */
		size_t size;
	} rx;
	struct {
		/** Pointer to private context passed as an argument to callbacks. */
		void *priv;
		/**
		 * @brief Bind was successful.
		 *
		 * Invoked when the local and remote rpc instances
		 * have bound successfully.
		 *
		 * @param priv Private user data.
		 */
		void (*bound)(void *priv);
		/**
		 * @brief New message has arrived.
		 *
		 * Invoked when new data is received.
		 *
		 * @param data Pointer to data buffer.
		 * @param len Length of @a data.
		 * @param priv Private user data.
		 */
		void (*received)(const void *data, size_t len, void *priv);
	} cb;
};

/**
 * @brief Parameters for configuring an nrf_modem signaling instance.
 *
 * Passed when invoking @c nrf_modem_os_rpc_signal_init.
 */
struct nrf_modem_os_rpc_signal_config {
	/** Signal device. */
	uintptr_t sigdev;
	/** Channel number. */
	uint32_t ch;
	/** Pointer to private context passed as an argument to callbacks. */
	void *priv;
	/** Invoked when signal with channel num @a ch is raised.
	 *  See @c nrf_modem_os_rpc_signal_cb_t.
	 */
	nrf_modem_os_rpc_signal_cb_t recv;
};

/**
 * @brief RPC instance declaration used internally in nrf_modem.
 *
 * This structure must be defined in the glue.
 */
struct nrf_modem_os_rpc;

/**
 * @brief Signaling instance declaration used internally in nrf_modem.
 *
 * This structure must be defined in the glue.
 */
struct nrf_modem_os_rpc_signal;

/**
 * @brief RPC instances used by nrf_modem and defined externally.
 */
extern struct nrf_modem_os_rpc inst_ctrl;
extern struct nrf_modem_os_rpc inst_data;

/**
 * @brief Signaling instances used by nrf_modem and defined externally.
 */
extern struct nrf_modem_os_rpc_signal inst_app_fault;
extern struct nrf_modem_os_rpc_signal inst_modem_fault;
extern struct nrf_modem_os_rpc_signal inst_modem_sysoff;

/**
 * @brief Get address of the application signal device.
 *
 * @return Address of application signal device.
 */
uintptr_t nrf_modem_os_rpc_sigdev_app_get(void);

/**
 * @brief Get address of the modem signal device.
 *
 * @return Address of modem signal device.
 */
uintptr_t nrf_modem_os_rpc_sigdev_modem_get(void);

/**
 * @brief Open an RPC instance.
 *
 * @param instance Pointer to RPC instance.
 * @param conf Parameters for configuring the instance before it is opened.
 *
 * @return 0 on success, a negative errno otherwise.
 */
int nrf_modem_os_rpc_open(struct nrf_modem_os_rpc *instance,
			  const struct nrf_modem_os_rpc_config *conf);

/**
 * @brief Send a message from the local RPC instance to the remote instance.
 *
 * @param instance Pointer to RPC instance.
 * @param msg Pointer to a buffer containing data to send.
 * @param len Size of data in the @p msg buffer.
 *
 * @retval 0 on success.
 * @retval -NRF_EBUSY when the instance is closed or handshaking.
 * @retval -NRF_ENOMEM when there are not enough memory in circular buffer.
 * @retval other errno codes from dependent modules.
 */
int nrf_modem_os_rpc_send(struct nrf_modem_os_rpc *instance, const void *msg, size_t len);

/**
 * @brief Close an RPC instance.
 *
 * @param instance pointer to RPC instance.
 *
 * @return 0 on success, a negative errno otherwise.
 */
int nrf_modem_os_rpc_close(struct nrf_modem_os_rpc *instance);

/**
 * @brief Suspend processing of incoming messages on the RPC instance.
 *
 * @param instance Pointer to RPC instance.
 *
 * @return 0 on success, a negative errno otherwise.
 */
int nrf_modem_os_rpc_rx_suspend(struct nrf_modem_os_rpc *instance);

/**
 * @brief Resume processing of incoming messages on the RPC instance.
 *
 * @param instance Pointer to RPC instance.
 *
 * @return 0 on success, a negative errno otherwise.
 */
int nrf_modem_os_rpc_rx_resume(struct nrf_modem_os_rpc *instance);

/**
 * @brief Configure and enable the signaling instance.
 *
 * @param instance Signaling instance.
 * @param conf Parameters for configuring the instance before it is enabled.
 *
 * @return 0 on success, a negative errno otherwise.
 */
int nrf_modem_os_rpc_signal_init(struct nrf_modem_os_rpc_signal *instance,
				 struct nrf_modem_os_rpc_signal_config *conf);

/**
 * @brief Signal with the signaling instance.
 *
 * @param instance Signaling instance.
 *
 * @return 0 on success, a negative errno otherwise.
 */
int nrf_modem_os_rpc_signal_send(struct nrf_modem_os_rpc_signal *instance);

/**
 * @brief Disable the signaling instance.
 *
 * @param instance Signaling instance.
 *
 * @return 0 on success, a negative errno otherwise.
 */
int nrf_modem_os_rpc_signal_deinit(struct nrf_modem_os_rpc_signal *instance);

/**
 * @brief Flush address range in cache.
 *
 * @param addr Starting address to flush.
 * @param size Range size.
 *
 * @return 0 on success, a negative errno otherwise.
 */
int nrf_modem_os_rpc_cache_data_flush(void *addr, size_t size);

/**
 * @brief Invalidate address range in cache.
 *
 * @param addr Starting address to invalidate.
 * @param size Range size.
 *
 * @return 0 on success, a negative errno otherwise.
 */
int nrf_modem_os_rpc_cache_data_invalidate(void *addr, size_t size);



#ifdef __cplusplus
}
#endif

#endif /* NRF_MODEM_OS_RPC_H__ */
/** @} */
