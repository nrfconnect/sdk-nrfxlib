/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @brief File containing declarations for the
 * OSAL Layer of the Wi-Fi driver.
 */

#ifndef __OSAL_API_H__
#define __OSAL_API_H__

#include "osal_structs.h"

/* Have to match zephyr/include/zephyr/logging/log_core.h */
#define NRF_WIFI_LOG_LEVEL_ERR 1U
#define NRF_WIFI_LOG_LEVEL_INF 3U
#define NRF_WIFI_LOG_LEVEL_DBG 4U

#ifndef CONFIG_NRF700X_LOG_VERBOSE
#define __func__ "<snipped>"
#endif /* CONFIG_NRF700X_LOG_VERBOSE */

/**
 * @brief the OSAL layer.
 *
 * Initialize the OSAL layer and is expected to be called
 * before using the OSAL layer. Returns a pointer to the OSAL context
 * which might need to be passed to further API calls.
 *
 * @return Pointer to instance of OSAL context.
 */
struct nrf_wifi_osal_priv *nrf_wifi_osal_init(void);

/**
 * @brief Deinitialize the OSAL layer.
 * @param opriv: Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 *
 * Deinitialize the OSAL layer and is expected to be called after done using
 * the OSAL layer.
 *
 * @return None.
 */
void nrf_wifi_osal_deinit(struct nrf_wifi_osal_priv *opriv);

/**
 * @brief Allocate memory.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param size Size of the memory to be allocated in bytes.
 * @return Pointer to start of allocated memory on success, NULL on error.
 */
void *nrf_wifi_osal_mem_alloc(struct nrf_wifi_osal_priv *opriv,
							  size_t size);

/**
 * @brief Allocated zero-initialized memory.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param size Size of the memory to be allocated in bytes.
 *
 * @return Pointer to start of allocated memory on success, NULL on error.
 */
void *nrf_wifi_osal_mem_zalloc(struct nrf_wifi_osal_priv *opriv,
							   size_t size);

/**
 * @brief Free previously allocated memory.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param buf Pointer to the memory to be freed.
 *
 * Free up memory which has been allocated using  nrf_wifi_osal_mem_alloc or
 *  nrf_wifi_osal_mem_zalloc.
 *
 * @return None.
 */
void nrf_wifi_osal_mem_free(struct nrf_wifi_osal_priv *opriv,
							void *buf);

/**
 * @brief Copy contents from one memory location to another.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param dest Pointer to the memory location where contents are to be copied.
 * @param src Pointer to the memory location from where contents are to be copied.
 * @param count Number of bytes to be copied.
 * @return Pointer to destination memory if successful, NULL otherwise.
 */
void *nrf_wifi_osal_mem_cpy(struct nrf_wifi_osal_priv *opriv,
							void *dest,
							const void *src,
							size_t count);

/**
 * @brief Fill a block of memory with a particular value.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param start Pointer to the memory location whose contents are to be set.
 * @param val Value to be set.
 * @param size Number of bytes to be set.
 * @return Pointer to memory location which was set on success, NULL on error.
 */
void *nrf_wifi_osal_mem_set(struct nrf_wifi_osal_priv *opriv,
							void *start,
							int val,
							size_t size);

/**
 * @brief Memory map IO memory into CPU space.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param addr Address of the IO memory to be mapped.
 * @param size Size of the IO memory in bytes.
 * @return Pointer to the mapped IO memory on success, NULL on error.
 */
void *nrf_wifi_osal_iomem_mmap(struct nrf_wifi_osal_priv *opriv,
							   unsigned long addr,
							   unsigned long size);

/**
 * @brief Unmap previously mapped IO memory from CPU space.
 *
 * @param opriv Pointer to the OSAL context returned by the nrf_wifi_osal_init API.
 * @param addr Pointer to mapped IO memory to be unmapped.
 *
 * Unmaps IO memory from CPU space that was mapped using nrf_wifi_osal_iomem_mmap.
 */
void nrf_wifi_osal_iomem_unmap(struct nrf_wifi_osal_priv *opriv,
							   volatile void *addr);

/**
 * @brief Read value from a 32 bit IO memory mapped register.
 *
 * @param opriv Pointer to the OSAL context returned by the nrf_wifi_osal_init API.
 * @param addr Pointer to the IO memory mapped register address.
 *
 * @return 32 bit value read from register.
 */
unsigned int nrf_wifi_osal_iomem_read_reg32(struct nrf_wifi_osal_priv *opriv,
				const volatile void *addr);

/**
 * @brief Write a 32 bit value to a IO memory mapped register.
 *
 * @param opriv Pointer to the OSAL context returned by the nrf_wifi_osal_init API.
 * @param addr Pointer to the IO memory mapped register address.
 * @param val Value to be written to the register.
 *
 * Writes a 32 bit value (val) to a 32 bit device register using a memory
 * mapped address (addr).
 */
void nrf_wifi_osal_iomem_write_reg32(struct nrf_wifi_osal_priv *opriv,
									 volatile void *addr,
									 unsigned int val);

/**
 * @brief Copy data from the memory of a memory mapped IO device to host memory.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param dest Pointer to the host memory where data is to be copied.
 * @param src Pointer to the memory of the memory mapped IO device from where data is to be copied.
 * @param count The size of the data to be copied in bytes.
 *
 * Copies a block of data of size  count bytes from memory mapped device memory(src)
 * to host memory(dest).
 *
 * @return None.
 */
void nrf_wifi_osal_iomem_cpy_from(struct nrf_wifi_osal_priv *opriv,
								  void *dest,
								  const volatile void *src,
								  size_t count);

/**
 * @brief Copy data to the memory of a memory mapped IO device from host memory.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param dest Pointer to the memory of the memory mapped IO device where data is to be copied.
 * @param src Pointer to the host memory from where data is to be copied.
 * @param count The size of the data to be copied in bytes.
 *
 * Copies a block of data of size  count bytes from host memory (src) to memory mapped
 * device memory(dest).
 *
 * @return None.
 */
void nrf_wifi_osal_iomem_cpy_to(struct nrf_wifi_osal_priv *opriv,
								volatile void *dest,
								const void *src,
								size_t count);

/**
 * @brief Allocate a busy lock.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 *
 * Allocates a busy lock.
 *
 * @return Pointer to the busy lock instance on success, NULL on error.
 */
void *nrf_wifi_osal_spinlock_alloc(struct nrf_wifi_osal_priv *opriv);

/**
 * @brief Free a busy lock.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param lock Pointer to a busy lock instance.
 *
 * Frees a busy lock (lock) allocated by  nrf_wifi_osal_spinlock_alloc.
 *
 * @return None.
 */
void nrf_wifi_osal_spinlock_free(struct nrf_wifi_osal_priv *opriv,
								 void *lock);

/**
 * @brief Initialize a busy lock.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param lock Pointer to a busy lock instance.
 *
 * Initialize a busy lock (lock) allocated by  nrf_wifi_osal_spinlock_alloc.
 *
 * @return None.
 */
void nrf_wifi_osal_spinlock_init(struct nrf_wifi_osal_priv *opriv,
								 void *lock);

/**
 * @brief Acquire a busy lock.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param lock Pointer to a busy lock instance.
 *
 * Acquires a busy lock (lock) allocated by  nrf_wifi_osal_spinlock_alloc.
 *
 * @return None.
 */
void nrf_wifi_osal_spinlock_take(struct nrf_wifi_osal_priv *opriv,
								 void *lock);

/**
 * @brief Releases a busy lock.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param lock Pointer to a busy lock instance.
 *
 * Releases a busy lock (lock) acquired by  nrf_wifi_osal_spinlock_take.
 *
 * @return None.
 */
void nrf_wifi_osal_spinlock_rel(struct nrf_wifi_osal_priv *opriv,
								void *lock);

/**
 * @brief Acquire a busy lock and disable interrupts.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param lock Pointer to a busy lock instance.
 * @param flags Interrupt state flags.
 *
 * Saves interrupt states ( flags), disable interrupts and take a
 * busy lock (lock).
 *
 * @return None.
 */
void nrf_wifi_osal_spinlock_irq_take(struct nrf_wifi_osal_priv *opriv,
									 void *lock,
									 unsigned long *flags);

/**
 * @brief Release a busy lock and enable interrupts.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param lock Pointer to a busy lock instance.
 * @param flags Interrupt state flags.
 *
 * Restores interrupt states ( flags) and releases busy lock (lock) acquired
 * using  nrf_wifi_osal_spinlock_irq_take.
 *
 * @return None.
 */
void nrf_wifi_osal_spinlock_irq_rel(struct nrf_wifi_osal_priv *opriv,
									void *lock,
									unsigned long *flags);

#if CONFIG_WIFI_NRF700X_LOG_LEVEL >= NRF_WIFI_LOG_LEVEL_DBG
/**
 * @brief Log a debug message.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param fmt Format string.
 * @param ... Variable arguments.
 *
 * Logs a debug message.
 *
 * @return Number of characters of the message logged.
 */
int nrf_wifi_osal_log_dbg(struct nrf_wifi_osal_priv *opriv,
						  const char *fmt, ...);
#else
#define nrf_wifi_osal_log_dbg(level, fmt, ...)
#endif

#if CONFIG_WIFI_NRF700X_LOG_LEVEL >= NRF_WIFI_LOG_LEVEL_INF
/**
 * @brief Logs an informational message.
 *
 * @param opriv Pointer to the OSAL context returned by the nrf_wifi_osal_init API.
 * @param fmt Format string.
 * @param ... Variable arguments.
 * @return Number of characters of the message logged.
 */
int nrf_wifi_osal_log_info(struct nrf_wifi_osal_priv *opriv,
						   const char *fmt, ...);
#else
#define nrf_wifi_osal_log_info(level, fmt, ...)
#endif

#if CONFIG_WIFI_NRF700X_LOG_LEVEL >= NRF_WIFI_LOG_LEVEL_ERR
/**
 * @brief Logs an error message.
 *
 * @param opriv Pointer to the OSAL context returned by the nrf_wifi_osal_init API.
 * @param fmt Format string.
 * @param ... Variable arguments.
 * @return Number of characters of the message logged.
 */
int nrf_wifi_osal_log_err(struct nrf_wifi_osal_priv *opriv,
						  const char *fmt, ...);
#else
#define nrf_wifi_osal_log_err(level, fmt, ...)
#endif

/**
 * @brief Allocate a linked list node.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 *
 * @return Pointer to the linked list node allocated.
 *         NULL if there is an error.
 */
void *nrf_wifi_osal_llist_node_alloc(struct nrf_wifi_osal_priv *opriv);

/**
 * @brief Free a linked list node.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param node Pointer to a linked list node.
 *
 * Frees a linked list node(node) which was allocated by  nrf_wifi_osal_llist_node_alloc.
 */
void nrf_wifi_osal_llist_node_free(struct nrf_wifi_osal_priv *opriv, void *node);

/**
 * @brief Get data stored in a linked list node.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param node Pointer to a linked list node.
 *
 * @return Pointer to the data stored in the linked list node.
 *         NULL if there is an error.
 */
void *nrf_wifi_osal_llist_node_data_get(struct nrf_wifi_osal_priv *opriv, void *node);

/**
 * @brief Set data in a linked list node.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param node Pointer to a linked list node.
 * @param data Pointer to the data to be stored in the linked list node.
 *
 * Stores the pointer to the data(data) in a linked list node(node).
 */
void nrf_wifi_osal_llist_node_data_set(struct nrf_wifi_osal_priv *opriv, void *node, void *data);

/**
 * @brief Allocate a linked list.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 *
 * @return Pointer to the allocated linked list on success, NULL on error.
 */
void *nrf_wifi_osal_llist_alloc(struct nrf_wifi_osal_priv *opriv);

/**
 * @brief Free a linked list.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param llist Pointer to a linked list.
 *
 * Frees a linked list (llist) allocated by  nrf_wifi_osal_llist_alloc.
 */
void nrf_wifi_osal_llist_free(struct nrf_wifi_osal_priv *opriv,
							  void *llist);

/**
 * @brief Initialize a linked list.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param llist Pointer to a linked list.
 *
 * Initialize a linked list (llist) allocated by  nrf_wifi_osal_llist_alloc.
 */
void nrf_wifi_osal_llist_init(struct nrf_wifi_osal_priv *opriv,
							  void *llist);

/**
 * @brief Add a node to the tail of a linked list.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param llist Pointer to a linked list.
 * @param llist_node Pointer to a linked list node.
 *
 * Adds a linked list node ( llist_node) allocated by  nrf_wifi_osal_llist_node_alloc
 * to the tail of a linked list (llist) allocated by  nrf_wifi_osal_llist_alloc.
 */
void nrf_wifi_osal_llist_add_node_tail(struct nrf_wifi_osal_priv *opriv,
									   void *llist,
									   void *llist_node);

/**
 * @brief Add a node to the head of a linked list.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param llist Pointer to a linked list.
 * @param llist_node Pointer to a linked list node.
 *
 * Adds a linked list node ( llist_node) allocated by  nrf_wifi_osal_llist_node_alloc
 * to the head of a linked list (llist) allocated by  nrf_wifi_osal_llist_alloc.
 */
void nrf_wifi_osal_llist_add_node_head(struct nrf_wifi_osal_priv *opriv,
									   void *llist,
									   void *llist_node);

/**
 * @brief Get the head of a linked list.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param llist Pointer to a linked list.
 *
 * Returns the head node from a linked list (llist) while still leaving the
 * node as part of the linked list. If the linked list is empty, returns NULL.
 *
 * @return Pointer to the head of the linked list on success, NULL on error.
 */
void *nrf_wifi_osal_llist_get_node_head(struct nrf_wifi_osal_priv *opriv,
										void *llist);

/**
 * @brief Get the next node in a linked list.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param llist Pointer to a linked list.
 * @param llist_node Pointer to a linked list node.
 * @return Pointer to the next node in the linked list if successful, NULL otherwise.
 */
void *nrf_wifi_osal_llist_get_node_nxt(struct nrf_wifi_osal_priv *opriv,
									   void *llist,
									   void *llist_node);

/**
 * @brief Delete node from a linked list.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param llist Pointer to a linked list.
 * @param llist_node Pointer to a linked list node.
 * @return None.
 */
void nrf_wifi_osal_llist_del_node(struct nrf_wifi_osal_priv *opriv,
								  void *llist,
								  void *llist_node);

/**
 * @brief Get length of a linked list.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param llist Pointer to a linked list.
 * @return Linked list length in bytes.
 */
unsigned int nrf_wifi_osal_llist_len(struct nrf_wifi_osal_priv *opriv,
									 void *llist);

/**
 * @brief Allocate a network buffer.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param size Size in bytes of the network buffer to be allocated.
 * @return Pointer to the allocated network buffer if successful, NULL otherwise.
 */
void *nrf_wifi_osal_nbuf_alloc(struct nrf_wifi_osal_priv *opriv,
							   unsigned int size);

/**
 * @brief Free a network buffer.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param nbuf Pointer to a network buffer.
 * @return None.
 */
void nrf_wifi_osal_nbuf_free(struct nrf_wifi_osal_priv *opriv,
							 void *nbuf);

/**
 * @brief Reserve headroom space in a network buffer.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param nbuf Pointer to a network buffer.
 * @param size Size in bytes of the headroom to be reserved.
 * @return None.
 */
void nrf_wifi_osal_nbuf_headroom_res(struct nrf_wifi_osal_priv *opriv,
									 void *nbuf,
									 unsigned int size);

/**
 * @brief Get the size of the headroom in a network buffer.
 *
 * @param opriv Pointer to the OSAL context returned by the nrf_wifi_osal_init API.
 * @param nbuf Pointer to a network buffer.
 * @return Size of the network buffer data headroom in bytes.
 */
unsigned int nrf_wifi_osal_nbuf_headroom_get(struct nrf_wifi_osal_priv *opriv,
			void *nbuf);

/**
 * @brief Get the size of data in a network buffer.
 *
 * @param opriv Pointer to the OSAL context returned by the nrf_wifi_osal_init API.
 * @param nbuf Pointer to a network buffer.
 * @return Size of the network buffer data in bytes.
 */
unsigned int nrf_wifi_osal_nbuf_data_size(struct nrf_wifi_osal_priv *opriv,
										  void *nbuf);

/**
 * @brief Get a handle to the data in a network buffer.
 *
 * @param opriv Pointer to the OSAL context returned by the nrf_wifi_osal_init API.
 * @param nbuf Pointer to a network buffer.
 * @return Pointer to the data in the network buffer if successful, otherwise NULL.
 */
void *nrf_wifi_osal_nbuf_data_get(struct nrf_wifi_osal_priv *opriv,
								  void *nbuf);

/**
 * @brief Extend the tail portion of the data in a network buffer.
 *
 * @param opriv Pointer to the OSAL context returned by the nrf_wifi_osal_init API.
 * @param nbuf Pointer to a network buffer.
 * @param size Size in bytes of the extension.
 * @return Updated pointer to the data in the network buffer if successful, otherwise NULL.
 */
void *nrf_wifi_osal_nbuf_data_put(struct nrf_wifi_osal_priv *opriv,
								  void *nbuf,
								  unsigned int size);

/**
 * @brief Extend the head portion of the data in a network buffer.
 *
 * @param opriv Pointer to the OSAL context returned by the nrf_wifi_osal_init API.
 * @param nbuf Pointer to a network buffer.
 * @param size Size in bytes, of the extension.
 * @return Updated pointer to the data in the network buffer if successful, NULL otherwise.
 */
void *nrf_wifi_osal_nbuf_data_push(struct nrf_wifi_osal_priv *opriv,
								   void *nbuf,
								   unsigned int size);

/**
 * @brief Reduce the head portion of the data in a network buffer.
 *
 * @param opriv Pointer to the OSAL context returned by the nrf_wifi_osal_init API.
 * @param nbuf Pointer to a network buffer.
 * @param size Size in bytes, of the reduction.
 * @return Updated pointer to the data in the network buffer if successful, NULL otherwise.
 */
void *nrf_wifi_osal_nbuf_data_pull(struct nrf_wifi_osal_priv *opriv,
								   void *nbuf,
								   unsigned int size);

/**
 * @brief Get the priority of a network buffer.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param nbuf Pointer to a network buffer.
 *
 * @return Priority of the network buffer.
 */
unsigned char nrf_wifi_osal_nbuf_get_priority(struct nrf_wifi_osal_priv *opriv,
			void *nbuf);

/**
 * @brief Get the checksum status of a network buffer.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param nbuf Pointer to a network buffer.
 *
 * @return Checksum status of the network buffer.
 */
unsigned char nrf_wifi_osal_nbuf_get_chksum_done(struct nrf_wifi_osal_priv *opriv,
			void *nbuf);

/**
 * @brief Set the checksum status of a network buffer.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param nbuf Pointer to a network buffer.
 * @param chksum_done Checksum status of the network buffer.
 *
 * @return None
 */
void nrf_wifi_osal_nbuf_set_chksum_done(struct nrf_wifi_osal_priv *opriv,
			void *nbuf, unsigned char chksum_done);
/**
 * @brief Allocate a tasklet.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param type Type of tasklet.
 * @return Pointer to the tasklet instance allocated if successful, otherwise NULL.
 */
void *nrf_wifi_osal_tasklet_alloc(struct nrf_wifi_osal_priv *opriv, int type);

/**
 * @brief Free a tasklet.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param tasklet Pointer to a tasklet.
 * @return None.
 */
void nrf_wifi_osal_tasklet_free(struct nrf_wifi_osal_priv *opriv, void *tasklet);

/**
 * @brief Initialize a tasklet.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param tasklet Pointer to a tasklet.
 * @param callbk_fn Callback function to be invoked by the tasklet.
 * @param data Data to be passed to the callback function when the tasklet invokes it.
 * @return None.
 */
void nrf_wifi_osal_tasklet_init(struct nrf_wifi_osal_priv *opriv, void *tasklet,
		void (*callbk_fn)(unsigned long), unsigned long data);

/**
 * @brief Schedule a tasklet.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param tasklet Pointer to a tasklet.
 *
 * Schedules a tasklet that had been allocated using
 *  nrf_wifi_osal_tasklet_alloc and initialized using
 *  nrf_wifi_osal_tasklet_init.
 *
 * @return None.
 */
void nrf_wifi_osal_tasklet_schedule(struct nrf_wifi_osal_priv *opriv,
									void *tasklet);

/**
 * @brief Terminate a tasklet.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param tasklet Pointer to a tasklet.
 *
 * Terminates a tasklet(tasklet) that had been scheduled by
 *  nrf_wifi_osal_tasklet_schedule.
 *
 * @return None.
 */
void nrf_wifi_osal_tasklet_kill(struct nrf_wifi_osal_priv *opriv,
								void *tasklet);

/**
 * @brief Sleep for a specified duration in milliseconds.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param msecs Sleep duration in milliseconds.
 *
 * Puts the calling thread to sleep for at least msecs milliseconds.
 *
 * @return None.
 */
void nrf_wifi_osal_sleep_ms(struct nrf_wifi_osal_priv *opriv,
							unsigned int msecs);

/**
 * @brief Delay for a specified duration in microseconds.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param usecs Delay duration in microseconds.
 *
 * Delays execution of calling thread for usecs microseconds. This is
 * busy-waiting and won't allow other threads to execute during
 * the time lapse.
 *
 * @return None.
 */
void nrf_wifi_osal_delay_us(struct nrf_wifi_osal_priv *opriv,
							unsigned long usecs);

/**
 * @brief Get current system uptime in microseconds.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 *
 * Get the current system uptime in microseconds.
 *
 * @return System uptime in microseconds.
 */
unsigned long nrf_wifi_osal_time_get_curr_us(struct nrf_wifi_osal_priv *opriv);

/**
 * @brief Get elapsed time in microseconds.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param start_time_us The timestamp in microseconds from which elapsed time is to be measured.
 *
 * @return Elapsed time in microseconds.
 */
unsigned int nrf_wifi_osal_time_elapsed_us(struct nrf_wifi_osal_priv *opriv,
		unsigned long start_time_us);

/**
 * @brief Initialize a PCIe driver.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param dev_name Name of the PCIe device.
 * @param vendor_id Vendor ID of the PCIe device.
 * @param sub_vendor_id Sub-vendor ID of the PCIE device.
 * @param device_id Device ID of the PCIe device.
 * @param sub_device_id Sub-device ID of the PCIe device.
 *
 * @return OS specific PCIe device context.
 */
void *nrf_wifi_osal_bus_pcie_init(struct nrf_wifi_osal_priv *opriv,
								  const char *dev_name,
								  unsigned int vendor_id,
								  unsigned int sub_vendor_id,
								  unsigned int device_id,
								  unsigned int sub_device_id);

/**
 * @brief Deinitialize a PCIe device driver.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_pcie_priv OS specific PCIe context.
 *
 * This API should be called when the PCIe device driver is to be unregistered from
 * the OS's PCIe core.
 */
void nrf_wifi_osal_bus_pcie_deinit(struct nrf_wifi_osal_priv *opriv,
								   void *os_pcie_priv);

/**
 * @brief Add a PCIe device instance.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_pcie_priv OS specific PCIe context.
 * @param osal_pcie_dev_ctx Pointer to the OSAL PCIe device context.
 *
 * Function to be invoked when a matching PCIe device is added to the system.
 * It expects the pointer to a OS specific PCIe device context to be returned.
 *
 * @return OS specific PCIe device context.
 */
void *nrf_wifi_osal_bus_pcie_dev_add(struct nrf_wifi_osal_priv *opriv,
									 void *os_pcie_priv,
									 void *osal_pcie_dev_ctx);

/**
 * @brief Remove a PCIe device instance.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_pcie_dev_ctx Pointer to the OS specific PCIe device context which was
 *	returned by  nrf_wifi_osal_bus_pcie_dev_add.
 *
 * Function to be invoked when a matching PCIe device is removed from the system.
 */
void nrf_wifi_osal_bus_pcie_dev_rem(struct nrf_wifi_osal_priv *opriv,
									void *os_pcie_dev_ctx);

/**
 * @brief Initialize a PCIe device instance.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_pcie_dev_ctx Pointer to the OS specific PCIe device context which was
 *                        returned by  nrf_wifi_osal_bus_pcie_dev_add.
 * @return NRF_WIFI_STATUS_SUCCESS if successful, NRF_WIFI_STATUS_FAIL otherwise.
 */
enum nrf_wifi_status nrf_wifi_osal_bus_pcie_dev_init(struct nrf_wifi_osal_priv *opriv,
			void *os_pcie_dev_ctx);

/**
 * @brief Deinitialize a PCIe device instance.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_pcie_dev_ctx Pointer to the OS specific PCIe device context which was
 *                        returned by  nrf_wifi_osal_bus_pcie_dev_add.
 */
void nrf_wifi_osal_bus_pcie_dev_deinit(struct nrf_wifi_osal_priv *opriv,
									   void *os_pcie_dev_ctx);

/**
 * @brief Register an interrupt handler for a PCIe device.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_pcie_dev_ctx OS specific PCIe device context.
 * @param callbk_data Data to be passed to the ISR.
 * @param callbk_fn ISR to be invoked on receiving an interrupt.
 * @return NRF_WIFI_STATUS_SUCCESS if successful, NRF_WIFI_STATUS_FAIL otherwise.
 */
enum nrf_wifi_status nrf_wifi_osal_bus_pcie_dev_intr_reg(struct nrf_wifi_osal_priv *opriv,
		void *os_pcie_dev_ctx, void *callbk_data, int (*callbk_fn)(void *callbk_data));

/**
 * @brief Unregister an interrupt handler for a PCIe device.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_pcie_dev_ctx OS specific PCIe device context.
 */
void nrf_wifi_osal_bus_pcie_dev_intr_unreg(struct nrf_wifi_osal_priv *opriv,
			void *os_pcie_dev_ctx);

/**
 * @brief Map host memory for DMA access.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_pcie_dev_ctx Pointer to a OS specific PCIe device handle.
 * @param virt_addr Virtual host address to be DMA mapped.
 * @param size Size in bytes of the host memory to be DMA mapped.
 * @param dir DMA direction.
 * @return Pointer to the DMA mapped physical address if successful, NULL otherwise.
 */
void *nrf_wifi_osal_bus_pcie_dev_dma_map(struct nrf_wifi_osal_priv *opriv,
		void *os_pcie_dev_ctx,
		void *virt_addr,
		size_t size,
		enum nrf_wifi_osal_dma_dir dir);

/**
 * @brief Unmap DMA mapped host memory.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_pcie_dev_ctx Pointer to a OS specific PCIe device handle.
 * @param dma_addr DMA mapped physical host memory address.
 * @param size Size in bytes of the DMA mapped host memory.
 * @param dir DMA direction.
 * @return None.
 */
void nrf_wifi_osal_bus_pcie_dev_dma_unmap(struct nrf_wifi_osal_priv *opriv,
		void *os_pcie_dev_ctx,
		void *dma_addr,
		size_t size,
		enum nrf_wifi_osal_dma_dir dir);

/**
 * @brief Get host mapped address for a PCIe device.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_pcie_dev_ctx OS specific PCIe device context.
 * @param host_map Host map address information.
 * @return None.
 */
void nrf_wifi_osal_bus_pcie_dev_host_map_get(struct nrf_wifi_osal_priv *opriv,
		void *os_pcie_dev_ctx,
		struct nrf_wifi_osal_host_map *host_map);

/**
 * @brief Initialize a qspi driver.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @return OS specific qspi device context.
 */
void *nrf_wifi_osal_bus_qspi_init(struct nrf_wifi_osal_priv *opriv);

/**
 * @brief Deinitialize a qspi device driver.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_qspi_priv OS specific qspi context.
 * @return None.
 */
void nrf_wifi_osal_bus_qspi_deinit(struct nrf_wifi_osal_priv *opriv,
		void *os_qspi_priv);

/**
 * @brief Add a qspi device instance.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_qspi_priv Pointer to the OS specific qspi device context.
 * @param osal_qspi_dev_ctx Pointer to the OSAL qspi device context.
 *
 * Function to be invoked when a matching qspi device is added to the system.
 * It expects the pointer to an OS specific qspi device context to be returned.
 *
 * @return OS specific qspi device context.
 */
void *nrf_wifi_osal_bus_qspi_dev_add(struct nrf_wifi_osal_priv *opriv,
		void *os_qspi_priv,
		void *osal_qspi_dev_ctx);

/**
 * @brief Remove a qspi device instance.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_qspi_dev_ctx Pointer to the OS specific qspi device context which was
 *                        returned by  nrf_wifi_osal_bus_qspi_dev_add.
 *
 * Function to be invoked when a matching qspi device is removed from the system.
 *
 * @return None.
 */
void nrf_wifi_osal_bus_qspi_dev_rem(struct nrf_wifi_osal_priv *opriv,
		void *os_qspi_dev_ctx);

/**
 * @brief Initialize a qspi device instance.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_qspi_dev_ctx Pointer to the OS specific qspi device context which was
 *                        returned by  nrf_wifi_osal_bus_qspi_dev_add.
 *
 * Function to be invoked when a qspi device is to be initialized.
 *
 * @return
 *     - Pass: NRF_WIFI_STATUS_SUCCESS.
 *     - Fail: NRF_WIFI_STATUS_FAIL.
 */
enum nrf_wifi_status nrf_wifi_osal_bus_qspi_dev_init(struct nrf_wifi_osal_priv *opriv,
		void *os_qspi_dev_ctx);

/**
 * @brief Deinitialize a qspi device instance.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_qspi_dev_ctx Pointer to the OS specific qspi device context which was
 *                        returned by  nrf_wifi_osal_bus_qspi_dev_add.
 *
 * Function to be invoked when a qspi device is to be deinitialized.
 *
 * @return None.
 */
void nrf_wifi_osal_bus_qspi_dev_deinit(struct nrf_wifi_osal_priv *opriv,
		void *os_qspi_dev_ctx);

/**
 * @brief Register a interrupt handler for a qspi device.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_qspi_dev_ctx OS specific qspi device context.
 * @param callbk_data Data to be passed to the ISR.
 * @param callbk_fn ISR to be invoked on receiving an interrupt.
 * @return NRF_WIFI_STATUS_SUCCESS if successful, NRF_WIFI_STATUS_FAIL otherwise.
 */
enum nrf_wifi_status nrf_wifi_osal_bus_qspi_dev_intr_reg(struct nrf_wifi_osal_priv *opriv,
		void *os_qspi_dev_ctx,
		void *callbk_data,
		int (*callbk_fn)(void *callbk_data));

/**
 * @brief Unregister an interrupt handler for a qspi device.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_qspi_dev_ctx OS specific qspi device context.
 */
void nrf_wifi_osal_bus_qspi_dev_intr_unreg(struct nrf_wifi_osal_priv *opriv,
		void *os_qspi_dev_ctx);

/**
 * @brief Get host mapped address for a qspi device.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_qspi_dev_ctx OS specific qspi device context.
 * @param host_map Host map address information.
 */
void nrf_wifi_osal_bus_qspi_dev_host_map_get(struct nrf_wifi_osal_priv *opriv,
		void *os_qspi_dev_ctx,
		struct nrf_wifi_osal_host_map *host_map);

/**
 * @brief Read value from a 32 bit register on a QSPI slave device.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param priv
 * @param addr Address of the register to read from.
 *
 * @return 32 bit value read from register.
 */
unsigned int nrf_wifi_osal_qspi_read_reg32(struct nrf_wifi_osal_priv *opriv,
		void *priv,
		unsigned long addr);

/**
 * @brief Writes a 32 bit value to a 32 bit device register on a QSPI slave device.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param priv
 * @param addr Address of the register to write to.
 * @param val Value to be written to the register.
 *
 * @return None.
 */
void nrf_wifi_osal_qspi_write_reg32(struct nrf_wifi_osal_priv *opriv,
		void *priv,
		unsigned long addr,
		unsigned int val);

/**
 * @brief Copies data from a QSPI slave device to a destination buffer.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param priv
 * @param dest Destination buffer.
 * @param addr Address of the data to be copied.
 * @param count Number of bytes to be copied.
 *
 * @return None.
 */
void nrf_wifi_osal_qspi_cpy_from(struct nrf_wifi_osal_priv *opriv,
		void *priv,
		void *dest,
		unsigned long addr,
		size_t count);

/**
 * @brief Copies data from a source buffer to a QSPI slave device.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param priv
 * @param addr Address of the data to be written.
 * @param src Source buffer.
 * @param count Number of bytes to be copied.
 *
 * @return None.
 */
void nrf_wifi_osal_qspi_cpy_to(struct nrf_wifi_osal_priv *opriv,
		void *priv,
		unsigned long addr,
		const void *src,
		size_t count);

/**
 * @brief Initialize a spi driver.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 *
 * Registers a spi device driver to the OS's spi core.
 *
 * @return OS specific spi device context.
 */
void *nrf_wifi_osal_bus_spi_init(struct nrf_wifi_osal_priv *opriv);

/**
 * @brief Deinitialize a spi device driver.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_spi_priv OS specific spi context.
 *
 * This API should be called when the spi device driver is
 * to be unregistered from the OS's spi core.
 *
 * @return None.
 */
void nrf_wifi_osal_bus_spi_deinit(struct nrf_wifi_osal_priv *opriv,
		void *os_spi_priv);

/**
 * @brief Add a spi device instance.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_spi_priv Pointer to the OS specific spi context.
 * @param osal_spi_dev_ctx Pointer to the OSAL spi device context.
 *
 * Function to be invoked when a matching spi device is added to the system.
 * It expects the pointer to a OS specific spi device context to be returned.
 *
 * @return OS specific spi device context.
 */
void *nrf_wifi_osal_bus_spi_dev_add(struct nrf_wifi_osal_priv *opriv,
		void *os_spi_priv,
		void *osal_spi_dev_ctx);

/**
 * @brief Remove a spi device instance.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_spi_dev_ctx Pointer to the OS specific spi device context which was
 *                       returned by  nrf_wifi_osal_bus_spi_dev_add.
 *
 * Function to be invoked when a matching spi device is removed from the system.
 *
 * @return None.
 */
void nrf_wifi_osal_bus_spi_dev_rem(struct nrf_wifi_osal_priv *opriv,
		void *os_spi_dev_ctx);

/**
 * @brief Initialize a spi device instance.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_spi_dev_ctx Pointer to the OS specific spi device context which was
 *                       returned by  nrf_wifi_osal_bus_spi_dev_add.
 *
 * Function to be invoked when a spi device is to be initialized.
 *
 * @return
 *      - Pass: nrf_wifi_STATUS_SUCCESS.
 *      - Fail: nrf_wifi_STATUS_FAIL.
 */
enum nrf_wifi_status nrf_wifi_osal_bus_spi_dev_init(struct nrf_wifi_osal_priv *opriv,
			void *os_spi_dev_ctx);

/**
 * @brief Deinitialize a spi device instance.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_spi_dev_ctx Pointer to the OS specific spi device context which was
 *                       returned by  nrf_wifi_osal_bus_spi_dev_add.
 *
 * Function to be invoked when a spi device is to be deinitialized.
 *
 * @return None.
 */
void nrf_wifi_osal_bus_spi_dev_deinit(struct nrf_wifi_osal_priv *opriv,
			void *os_spi_dev_ctx);

/**
 * @brief Register an interrupt handler for a spi device.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_spi_dev_ctx OS specific spi device context.
 * @param callbk_data Data to be passed to the ISR.
 * @param callbk_fn ISR to be invoked on receiving an interrupt.
 *
 * Registers an interrupt handler to the OS. This API also passes the callback
 * data to be passed to the ISR when an interrupt is received.
 *
 * @return
 *     Pass: nrf_wifi_STATUS_SUCCESS.
 *     Fail: nrf_wifi_STATUS_FAIL.
 */
enum nrf_wifi_status nrf_wifi_osal_bus_spi_dev_intr_reg(struct nrf_wifi_osal_priv *opriv,
		void *os_spi_dev_ctx,
		void *callbk_data,
		int (*callbk_fn)(void *callbk_data));

/**
 * @brief Unregister an interrupt handler for a spi device.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_spi_dev_ctx OS specific spi device context.
 *
 * Unregisters the interrupt handler that was registered using
 *  nrf_wifi_osal_bus_spi_dev_intr_reg.
 *
 * @return None.
 */
void nrf_wifi_osal_bus_spi_dev_intr_unreg(struct nrf_wifi_osal_priv *opriv,
		void *os_spi_dev_ctx);

/**
 * @brief Get host mapped address for a spi device.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param os_spi_dev_ctx OS specific spi device context.
 * @param host_map Host map address information.
 *
 * Get the host map address for a spi device.
 *
 * @return None.
 */
void nrf_wifi_osal_bus_spi_dev_host_map_get(struct nrf_wifi_osal_priv *opriv,
		void *os_spi_dev_ctx,
		struct nrf_wifi_osal_host_map *host_map);

/**
 * @brief Read value from a 32 bit register on a Linux SPI slave device.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param priv
 * @param addr Address of the register to read from.
 *
 * @return 32 bit value read from register.
 */
unsigned int nrf_wifi_osal_spi_read_reg32(struct nrf_wifi_osal_priv *opriv,
		void *priv,
		unsigned long addr);

/**
 * @brief Writes a 32 bit value to a 32 bit device register on a Linux SPI slave device.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param priv
 * @param addr Address of the register to write to.
 * @param val Value to be written to the register.
 *
 * @return None.
 */
void nrf_wifi_osal_spi_write_reg32(struct nrf_wifi_osal_priv *opriv,
		void *priv,
		unsigned long addr,
		unsigned int val);

/**
 * @brief Copies data from a SPI slave device to a destination buffer.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param priv
 * @param dest Destination buffer.
 * @param addr Address of the register to read from.
 * @param count Number of bytes to copy.
 *
 * @return None.
 */
void nrf_wifi_osal_spi_cpy_from(struct nrf_wifi_osal_priv *opriv,
		void *priv,
		void *dest,
		unsigned long addr,
		size_t count);

/**
 * @brief Copies data from a source buffer to a SPI slave device.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param priv
 * @param addr Address of the register to write to.
 * @param src Source buffer.
 * @param count Number of bytes to copy.
 *
 * @return None.
 */
void nrf_wifi_osal_spi_cpy_to(struct nrf_wifi_osal_priv *opriv,
		void *priv,
		unsigned long addr,
		const void *src,
		size_t count);

#if defined(CONFIG_NRF_WIFI_LOW_POWER) || defined(__DOXYGEN__)
/**
 * @brief Allocate a timer.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @return Pointer to the allocated timer instance.
 */
void *nrf_wifi_osal_timer_alloc(struct nrf_wifi_osal_priv *opriv);

/**
 * @brief Free a timer.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param timer Pointer to a timer instance.
 * @return None.
 */
void nrf_wifi_osal_timer_free(struct nrf_wifi_osal_priv *opriv,
							  void *timer);

/**
 * @brief Initialize a timer.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param timer Pointer to a timer instance.
 * @param callbk_fn Callback function to be invoked when the timer expires.
 * @param data Data to be passed to the callback function.
 * @return None.
 */
void nrf_wifi_osal_timer_init(struct nrf_wifi_osal_priv *opriv,
							  void *timer,
							  void (*callbk_fn)(unsigned long),
							  unsigned long data);

/**
 * @brief Schedule a timer.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param timer Pointer to a timer instance.
 * @param duration Duration of the timer in seconds.
 * @return None.
 */
void nrf_wifi_osal_timer_schedule(struct nrf_wifi_osal_priv *opriv,
								  void *timer,
								  unsigned long duration);

/**
 * @brief Kill a timer.
 *
 * @param opriv Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param timer Pointer to a timer instance.
 * @return None.
 */
/**
 * @brief Kills a timer.
 *
 * @param opriv Pointer to the nrf_wifi_osal_priv structure.
 * @param timer Pointer to the timer to be killed.
 */
void nrf_wifi_osal_timer_kill(struct nrf_wifi_osal_priv *opriv,
							  void *timer);

/**
 * @brief Puts the QSPI interface to sleep.
 *
 * @param opriv Pointer to the nrf_wifi_osal_priv structure.
 * @param os_qspi_priv Pointer to the QSPI private data.
 *
 * @return 0 on success, negative error code on failure.
 */
int nrf_wifi_osal_bus_qspi_ps_sleep(struct nrf_wifi_osal_priv *opriv,
									void *os_qspi_priv);

/**
 * @brief Wakes up the QSPI interface from sleep.
 *
 * @param opriv Pointer to the nrf_wifi_osal_priv structure.
 * @param os_qspi_priv Pointer to the QSPI private data.
 *
 * @return 0 on success, negative error code on failure.
 */
int nrf_wifi_osal_bus_qspi_ps_wake(struct nrf_wifi_osal_priv *opriv,
								   void *os_qspi_priv);

/**
 * @brief Get the power status of the QSPI interface.
 *
 * @param opriv Pointer to the nrf_wifi_osal_priv structure.
 * @param os_qspi_priv Pointer to the QSPI private data.
 *
 * @return 0 if the QSPI interface is in sleep mode,
 *		1 if it is awake, negative error code on failure.
 */
int nrf_wifi_osal_bus_qspi_ps_status(struct nrf_wifi_osal_priv *opriv,
									 void *os_qspi_priv);
#endif /* CONFIG_NRF_WIFI_LOW_POWER */

/**
 * @brief nrf_wifi_osal_assert() - Assert a condition with a value.
 *
 * @param opriv: Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param test: Variable to be tested.
 * @param val: Value to be checked for the @test
 * @param op: Type of operation to be done during assertion check.
 * @param msg: Assertion message.
 *
 * Compares @test with @val. If true, prints assert message.
 *
 * @return None.
 */
void nrf_wifi_osal_assert(struct nrf_wifi_osal_priv *opriv,
						  int test,
						  int val,
						  enum nrf_wifi_assert_op_type op,
						  char *msg);

/**
 * @brief nrf_wifi_osal_strlen() - Gives the length of the string str.
 *
 * @param opriv: Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param str: Pointer to the memory location of the string.
 *
 * Calculates the length of the string pointed to by str.
 *
 * @return The number of bytes of the string str.
 */
unsigned int nrf_wifi_osal_strlen(struct nrf_wifi_osal_priv *opriv,
								  const void *str);

/**
 * @brief nrf_wifi_osal_mem_cmp() - Compare contents from one memory location to another.
 *
 * @param opriv: Pointer to the OSAL context returned by the  nrf_wifi_osal_init API.
 * @param addr1: Pointer to the memory location of the first address.
 * @param addr2: Pointer to the memory location of the second address.
 * @param count: Number of bytes to be compared.
 *
 * Compares count number of bytes from addr1 location in memory to addr2
 * location in memory.
 *
 * @return An integer less than, equal to, or greater than zero.
 */
int nrf_wifi_osal_mem_cmp(struct nrf_wifi_osal_priv *opriv,
						  const void *addr1,
						  const void *addr2,
						  size_t count);
#endif /* __OSAL_API_H__ */
