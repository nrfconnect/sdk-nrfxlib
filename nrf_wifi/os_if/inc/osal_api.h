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
#define NRF_WIFI_LOG_LEVEL_ERR  1U
#define NRF_WIFI_LOG_LEVEL_INF  3U
#define NRF_WIFI_LOG_LEVEL_DBG  4U

#ifndef CONFIG_NRF700X_LOG_VERBOSE
#define __func__ "<snipped>"
#endif /* CONFIG_NRF700X_LOG_VERBOSE */

/**
 * nrf_wifi_osal_init() - Initialize the OSAL layer.
 * @ops: Pointer to the OSAL operations structure.
 *
 * Initializes the OSAL layer and is expected to be called
 * before using the OSAL layer.
 *
 * Return: None.
 */
void nrf_wifi_osal_init(const struct nrf_wifi_osal_ops *ops);


/**
 * nrf_wifi_osal_deinit() - Deinitialize the OSAL layer.
 *
 * Deinitializes the OSAL layer and is expected to be called after done using
 * the OSAL layer.
 *
 * Return: None.
 */
void nrf_wifi_osal_deinit(void);


/**
 * nrf_wifi_osal_mem_alloc() - Allocate memory.
 * @size: Size of the memory to be allocated in bytes.
 *
 * Allocates memory of @size bytes and returns a pointer to the start
 * of the memory allocated.
 *
 * Return:
 *		Pass: Pointer to start of allocated memory.
 *		Error: NULL.
 */
void *nrf_wifi_osal_mem_alloc(size_t size);

/**
 * nrf_wifi_osal_mem_zalloc() - Allocated zero-initialized memory.
 * @size: Size of the memory to be allocated in bytes.
 *
 * Allocates memory of @size bytes, zeroes it out and returns a pointer to the
 * start of the memory allocated.
 *
 * Return:
 *		Pass: Pointer to start of allocated memory.
 *		Error: NULL.
 */
void *nrf_wifi_osal_mem_zalloc(size_t size);

/**
 * nrf_wifi_osal_mem_free() - Free previously allocated memory.
 * @buf: Pointer to the memory to be freed.
 *
 * Free up memory which has been allocated using @nrf_wifi_osal_mem_alloc or
 * @nrf_wifi_osal_mem_zalloc.
 *
 * Return: None.
 */
void nrf_wifi_osal_mem_free(void *buf);

/**
 * nrf_wifi_osal_mem_cpy() - Copy contents from one memory location to another.
 * @dest: Pointer to the memory location where contents are to be copied.
 * @src: Pointer to the memory location from where contents are to be copied.
 * @count: Number of bytes to be copied.
 *
 * Copies @count number of bytes from @src location in memory to @dest
 * location in memory.
 *
 * Return:
 *		Pass: Pointer to destination memory.
 *		Error: NULL.
 */
void *nrf_wifi_osal_mem_cpy(void *dest,
			    const void *src,
			    size_t count);

/**
 * nrf_wifi_osal_mem_set() - Fill a block of memory with a particular value.
 * @start: Pointer to the memory location whose contents are to be set.
 * @val: Value to be set.
 * @size: Number of bytes to be set.
 *
 * Fills a block of memory of @size bytes starting at @start with a particular
 * value represented by @val.
 *
 * Return:
 *		Pass: Pointer to memory location which was set.
 *		Error: NULL.
 */
void *nrf_wifi_osal_mem_set(void *start,
			    int val,
			    size_t size);


/**
 * nrf_wifi_osal_iomem_mmap() - Memory map IO memory into CPU space.
 * @addr: Address of the IO memory to be mapped.
 * @size: Size of the IO memory in bytes.
 *
 * Maps IO memory of @size bytes pointed to by @addr into CPU space.
 *
 * Return:
 *		Pass: Pointer to the mapped IO memory.
 *		Error: NULL.
 */
void *nrf_wifi_osal_iomem_mmap(unsigned long addr,
			       unsigned long size);

/**
 * nrf_wifi_osal_iomem_unmap() - Unmap previously mapped IO memory from CPU space.
 * @addr: Pointer to mapped IO memory to be unmapped.
 *
 * Unmaps IO memory from CPU space that was mapped using @nrf_wifi_osal_iomem_mmap.
 *
 * Return: None.
 */
void nrf_wifi_osal_iomem_unmap(volatile void *addr);

/**
 * nrf_wifi_osal_iomem_read_reg32() - Read value from a 32 bit IO memory mapped
 *                                   register.
 * @addr: Pointer to the IO memory mapped register address.
 *
 * Reads value from a 32 bit device register using a memory mapped
 * address(@addr).
 *
 * Return: 32 bit value read from register.
 */
unsigned int nrf_wifi_osal_iomem_read_reg32(const volatile void *addr);

/**
 * nrf_wifi_osal_iomem_write_reg32() - Write a 32 bit value to a IO memory mapped register.
 * @addr: Pointer to the IO memory mapped register address.
 * @val: Value to be written to the register.
 *
 * Writes a 32 bit value (@val) to a 32 bit device register using a memory
 * mapped address(@addr).
 *
 * Return: None.
 */
void nrf_wifi_osal_iomem_write_reg32(volatile void *addr,
				     unsigned int val);

/**
 * nrf_wifi_osal_iomem_cpy_from() - Copy data from the memory of a memory
 *                                 mapped IO device to host memory.
 * @dest: Pointer to the host memory where data is to be copied.
 * @src: Pointer to the memory of the memory mapped IO device from where
 *       data is to be copied.
 * @count: The size of the data to be copied in bytes.
 *
 * Copies a block of data of size @count bytes from memory mapped device
 * memory(@src) to host memory(@dest).
 *
 * Return: None.
 */
void nrf_wifi_osal_iomem_cpy_from(void *dest,
				  const volatile void *src,
				  size_t count);

/**
 * nrf_wifi_osal_iomem_cpy_to() - Copy data to the memory of a memory
 *                               mapped IO device from host memory.
 * @dest: Pointer to the memory of the memory mapped IO device where
 *        data is to be copied.
 * @src: Pointer to the host memory from where data is to be copied.
 * @count: The size of the data to be copied in bytes.
 *
 * Copies a block of data of size @count bytes from host memory (@src) to
 * memory mapped device memory(@dest).
 *
 * Return: None.
 */
void nrf_wifi_osal_iomem_cpy_to(volatile void *dest,
				const void *src,
				size_t count);


/**
 * nrf_wifi_osal_spinlock_alloc() - Allocate a busy lock.
 *
 * Allocates a busy lock.
 *
 * Return:
 *		Pass: Pointer to the busy lock instance.
 *		Error: NULL.
 */
void *nrf_wifi_osal_spinlock_alloc(void);

/**
 * nrf_wifi_osal_spinlock_free() - Free a busy lock.
 * @lock: Pointer to a busy lock instance.
 *
 * Frees a busy lock (@lock) allocated by @nrf_wifi_osal_spinlock_alloc
 *
 * Return: None.
 */
void nrf_wifi_osal_spinlock_free(void *lock);


/**
 * nrf_wifi_osal_spinlock_init() - Initialize a busy lock.
 * @lock: Pointer to a busy lock instance.
 *
 * Initializes a busy lock (@lock) allocated by @nrf_wifi_osal_spinlock_alloc.
 *
 * Return: None.
 */
void nrf_wifi_osal_spinlock_init(void *lock);


/**
 * nrf_wifi_osal_spinlock_take() - Acquire a buys lock.
 * @lock: Pointer to a busy lock instance.
 *
 * Acquires a busy lock (@lock) allocated by @nrf_wifi_osal_spinlock_alloc.
 *
 * Return: None.
 */
void nrf_wifi_osal_spinlock_take(void *lock);


/**
 * nrf_wifi_osal_spinlock_rel() - Releases a busy lock.
 * @lock: Pointer to a busy lock instance.
 *
 * Releases a busy lock (@lock) acquired by @nrf_wifi_osal_spinlock_take.
 *
 * Return: None.
 */
void nrf_wifi_osal_spinlock_rel(void *lock);


/**
 * nrf_wifi_osal_spinlock_irq_take() - Acquire a busy lock and disable
 *                                    interrupts.
 * @lock: Pointer to a busy lock instance.
 * @flags: Interrupt state flags.
 *
 * Saves interrupt states (@flags), disable interrupts and take a
 * busy lock (@lock).
 *
 * Return: None.
 */
void nrf_wifi_osal_spinlock_irq_take(void *lock,
				     unsigned long *flags);


/**
 * nrf_wifi_osal_spinlock_irq_rel() - Release a busy lock and enable interrupts.
 * @lock: Pointer to a busy lock instance.
 * @flags: Interrupt state flags.
 *
 * Restores interrupt states (@flags) and releases busy lock (@lock) acquired
 * using @nrf_wifi_osal_spinlock_irq_take.
 *
 * Return: None.
 */
void nrf_wifi_osal_spinlock_irq_rel(void *lock,
				    unsigned long *flags);


#if CONFIG_WIFI_NRF700X_LOG_LEVEL >= NRF_WIFI_LOG_LEVEL_DBG
/**
 * nrf_wifi_osal_log_dbg() - Log a debug message.
 * @fmt: Format string.
 * @...: Variable arguments.
 *
 * Logs a debug message.
 *
 * Return: Number of characters of the message logged.
 */
int nrf_wifi_osal_log_dbg(const char *fmt, ...);
#else
#define nrf_wifi_osal_log_dbg(fmt, ...)
#endif


#if CONFIG_WIFI_NRF700X_LOG_LEVEL >= NRF_WIFI_LOG_LEVEL_INF
/**
 * nrf_wifi_osal_log_info() - Log a informational message.
 * @fmt: Format string.
 * @...: Variable arguments.
 *
 * Logs an informational message.
 *
 * Return: Number of characters of the message logged.
 */
int nrf_wifi_osal_log_info(const char *fmt, ...);
#else
#define nrf_wifi_osal_log_info(fmt, ...)
#endif


#if CONFIG_WIFI_NRF700X_LOG_LEVEL >= NRF_WIFI_LOG_LEVEL_ERR
/**
 * nrf_wifi_osal_log_err() - Logs an error message.
 * @fmt: Format string.
 * @...: Variable arguments.
 *
 * Logs an error message.
 *
 * Return: Number of characters of the message logged.
 */
int nrf_wifi_osal_log_err(const char *fmt, ...);
#else
#define nrf_wifi_osal_log_err(fmt, ...)
#endif


/**
 * nrf_wifi_osal_llist_node_alloc() - Allocate a linked list mode.
 *
 * Allocates a linked list node.
 *
 * Return:
 *		Pass: Pointer to the linked list node allocated.
 *		Error: NULL.
 */
void *nrf_wifi_osal_llist_node_alloc(void);


/**
 * nrf_wifi_osal_llist_node_free() - Free a linked list node.
 * @node: Pointer to a linked list node.
 *
 * Frees a linked list node(@node) which was allocated by
 * @nrf_wifi_osal_llist_node_alloc.
 *
 * Return: None.
 */
void nrf_wifi_osal_llist_node_free(void *node);

/**
 * nrf_wifi_osal_llist_node_data_get() - Get data stored in a linked list node.
 * @node: Pointer to a linked list node.
 *
 * Gets the pointer to the data which the linked list node(@node) points to.
 *
 * Return:
 *		Pass: Pointer to the data stored in the linked list node.
 *		Error: NULL.
 */
void *nrf_wifi_osal_llist_node_data_get(void *node);


/**
 * nrf_wifi_osal_llist_node_data_set() - Set data in a linked list node.
 * @node: Pointer to a linked list node.
 * @data: Pointer to the data to be stored in the linked list node.
 *
 * Stores the pointer to the data(@data) in a linked list node(@node).
 *
 *
 * Return: None.
 */
void nrf_wifi_osal_llist_node_data_set(void *node,
				       void *data);


/**
 * nrf_wifi_osal_llist_alloc() - Allocate a linked list.
 *
 * Allocates a linked list.
 *
 * Return:
 *		Pass: Pointer to the allocated linked list.
 *		Error: NULL.
 */
void *nrf_wifi_osal_llist_alloc(void);


/**
 * nrf_wifi_osal_llist_free() - Free a linked list.
 * @llist: Pointer to a linked list.
 *
 * Frees a linked list(@llist) allocated by @nrf_wifi_osal_llist_alloc.
 *
 * Return: None.
 */
void nrf_wifi_osal_llist_free(void *llist);


/**
 * nrf_wifi_osal_llist_init() - Initialize a linked list.
 * @llist: Pointer to a linked list.
 *
 * Initializes a linked list(@llist) allocated by @nrf_wifi_osal_llist_alloc.
 *
 * Return: None.
 */
void nrf_wifi_osal_llist_init(void *llist);


/**
 * nrf_wifi_osal_llist_add_node_tail() - Add a node to the tail of a linked list.
 * @llist: Pointer to a linked list.
 * @llist_node: Pointer to a linked list node.
 *
 * Adds a linked list node(@llist_node) allocated by @nrf_wifi_osal_llist_node_alloc
 * to the tail of a linked list(@llist) allocated by @nrf_wifi_osal_llist_alloc.
 *
 * Return: None.
 */
void nrf_wifi_osal_llist_add_node_tail(void *llist,
				       void *llist_node);


/**
 * nrf_wifi_osal_llist_add_node_head() - Add a node to the head of a linked list.
 * @llist: Pointer to a linked list.
 * @llist_node: Pointer to a linked list node.
 *
 * Adds a linked list node(@llist_node) allocated by @nrf_wifi_osal_llist_node_alloc
 * to the head of a linked list(@llist) allocated by @nrf_wifi_osal_llist_alloc.
 *
 * Return: None.
 */
void nrf_wifi_osal_llist_add_node_head(void *llist,
				       void *llist_node);
/**
 * nrf_wifi_osal_llist_get_node_head() - Get the head of a linked list.
 * @llist: Pointer to a linked list.
 *
 * Returns the head node from a linked list(@llist) while still leaving the
 * node as part of the linked list. If the linked list is empty
 * returns NULL.
 *
 * Return:
 *		Pass: Pointer to the head of the linked list.
 *		Error: NULL.
 */
void *nrf_wifi_osal_llist_get_node_head(void *llist);


/**
 * nrf_wifi_osal_llist_get_node_nxt() - Get the next node in a linked list.
 * @llist: Pointer to a linked list.
 * @llist_node: Pointer to a linked list node.
 *
 * Return the node next to the node passed in the @llist_node parameter.
 * The node returned is not removed from the linked list.
 *
 * Return:
 *		Pass: Pointer to the next node in the linked list.
 *		Error: NULL.
 */
void *nrf_wifi_osal_llist_get_node_nxt(void *llist,
				       void *llist_node);


/**
 * nrf_wifi_osal_llist_del_node() - Delete node from a linked list.
 * @llist: Pointer to a linked list.
 * @llist_node: Pointer to a linked list node.
 *
 * Removes the node passed in the @llist_node parameter from the linked list
 * passed in the @llist parameter.
 *
 * Return: None.
 */
void nrf_wifi_osal_llist_del_node(void *llist,
				  void *llist_node);


/**
 * nrf_wifi_osal_llist_len() - Get length of a linked list.
 * @llist: Pointer to a linked list.
 *
 * Returns the length of the linked list(@llist).
 *
 * Return: Linked list length in bytes.
 */
unsigned int nrf_wifi_osal_llist_len(void *llist);


/**
 * nrf_wifi_osal_nbuf_alloc() - Allocate a network buffer
 * @size: Size in bytes of the network buffer to allocated.
 *
 * Allocates a network buffer of size @size.
 *
 * Return:
 *		Pass: Pointer to the allocated network buffer.
 *		Error: NULL.
 */
void *nrf_wifi_osal_nbuf_alloc(unsigned int size);


/**
 * nrf_wifi_osal_nbuf_free() - Free a network buffer.
 * @nbuf: Pointer to a network buffer.
 *
 * Frees a network buffer(@nbuf) which was allocated by
 * @nrf_wifi_osal_nbuf_alloc.
 *
 * Return: None.
 */
void nrf_wifi_osal_nbuf_free(void *nbuf);


/**
 * nrf_wifi_osal_nbuf_headroom_res() - Reserve headroom space in a network buffer.
 * @nbuf: Pointer to a network buffer.
 * @size: Size in bytes of the headroom to be reserved.
 *
 * Reserves headroom of size(@size) bytes at the beginning of the data area of
 * a network buffer(@nbuf).
 *
 * Return: None.
 */
void nrf_wifi_osal_nbuf_headroom_res(void *nbuf,
				     unsigned int size);



/**
 * nrf_wifi_osal_nbuf_headroom_get() - Get the size of the headroom in a network buffer.
 * @nbuf: Pointer to a network buffer.
 *
 * Gets the size of the reserved headroom at the beginning
 * of the data area of a network buffer(@nbuf).
 *
 * Return: Size of the network buffer data headroom in bytes.
 */
unsigned int nrf_wifi_osal_nbuf_headroom_get(void *nbuf);

/**
 * nrf_wifi_osal_nbuf_data_size() - Get the size of data in a network buffer.
 * @nbuf: Pointer to a network buffer.
 *
 * Gets the size of the data area of a network buffer(@nbuf).
 *
 * Return: Size of the network buffer data in bytes.
 */
unsigned int nrf_wifi_osal_nbuf_data_size(void *nbuf);


/**
 * nrf_wifi_osal_nbuf_data_get() - Get a handle to the data in a network buffer.
 * @nbuf: Pointer to a network buffer.
 *
 * Gets the pointer to the data area of a network buffer(@nbuf).
 *
 * Return:
 *		Pass: Pointer to the data in the network buffer.
 *		Error: NULL.
 */
void *nrf_wifi_osal_nbuf_data_get(void *nbuf);


/**
 * nrf_wifi_osal_nbuf_data_put() - Extend the tail portion of the data
 *                                in a network buffer.
 * @nbuf: Pointer to a network buffer.
 * @size: Size in bytes, of the extension.
 *
 * Increases the data area of a network buffer(@nbuf) by @size bytes at the
 * end of the area and returns the pointer to the beginning of
 * the data area.
 *
 * Return:
 *		Pass: Updated pointer to the data in the network buffer.
 *		Error: NULL.
 */
void *nrf_wifi_osal_nbuf_data_put(void *nbuf,
				  unsigned int size);


/**
 * nrf_wifi_osal_nbuf_data_push() - Extend the head portion of the data
 *                                 in a network buffer.
 * @nbuf: Pointer to a network buffer.
 * @size: Size in bytes, of the extension.
 *
 * Increases the data area of a network buffer(@nbuf) by @size bytes at the
 * start of the area and returns the pointer to the beginning of the
 * data area.
 *
 * Return:
 *		Pass: Updated pointer to the data in the network buffer.
 *		Error: NULL.
 */
void *nrf_wifi_osal_nbuf_data_push(void *nbuf,
				   unsigned int size);


/**
 * nrf_wifi_osal_nbuf_data_pull() - Reduce the head portion of the data
 *                                 in a network buffer.
 * @nbuf: Pointer to a network buffer.
 * @size: Size in bytes, of the reduction.
 *
 * Decreases the data area of a network buffer(@nbuf) by @size bytes at the
 * start of the area and returns the pointer to the beginning
 * of the data area.
 *
 * Return:
 *		Pass: Updated pointer to the data in the network buffer.
 *		Error: NULL.
 */
void *nrf_wifi_osal_nbuf_data_pull(void *nbuf,
				   unsigned int size);


/**
 * nrf_wifi_osal_nbuf_get_priority() - Get the priority of a network buffer.
 *
 * @nbuf: Pointer to a network buffer.
 *
 * Gets the priority of a network buffer(@nbuf).
 *
 * Return: Priority of the network buffer.
 */
unsigned char nrf_wifi_osal_nbuf_get_priority(void *nbuf);

/**
 * nrf_wifi_osal_nbuf_get_chksum_done() - Get the checksum status of a network buffer.
 *
 * @nbuf: Pointer to a network buffer.
 *
 * Gets the checksum status of a network buffer(@nbuf).
 *
 * Return: Checksum status of the network buffer.
 */
unsigned char nrf_wifi_osal_nbuf_get_chksum_done(void *nbuf);

/**
 * nrf_wifi_osal_nbuf_set_chksum_done() - Set the checksum status of a network buffer.
 *
 * @nbuf: Pointer to a network buffer.
 * @chksum_done: Checksum status of the network buffer.
 *
 * Sets the checksum status of a network buffer(@nbuf).
 *
 * Return: None
 */
void nrf_wifi_osal_nbuf_set_chksum_done(void *nbuf,
					unsigned char chksum_done);
/**
 * nrf_wifi_osal_tasklet_alloc() - Allocate a tasklet.
 * @type: Type of tasklet.
 *
 * Allocates a tasklet structure and returns a pointer to it.
 *
 * Return:
 *		Pass: Pointer to the tasklet instance allocated.
 *		Error: NULL.
 */
void *nrf_wifi_osal_tasklet_alloc(int type);


/**
 * nrf_wifi_osal_tasklet_free() - Free a tasklet.
 * @tasklet: Pointer to a tasklet.
 *
 * Frees a tasklet structure that had been allocated using
 * @nrf_wifi_osal_tasklet_alloc.
 *
 * Return: None.
 */
void nrf_wifi_osal_tasklet_free(void *tasklet);


/**
 * nrf_wifi_osal_tasklet_init() - Initialize a tasklet.
 * @tasklet:  Pointer to a tasklet.
 * @callbk_fn: Callback function to be invoked by the tasklet.
 * @data: Data to be passed to the callback function when the tasklet
 *        invokes it.
 *
 * Initializes a tasklet structure(@tasklet) that had been allocated using
 * @nrf_wifi_osal_tasklet_alloc. Needs to be passed a callback
 * function (@callbk_fn) which should get invoked when the tasklet is
 * scheduled and the data(@data) which needs to be passed to the
 * callback function.
 *
 * Return: None.
 */
void nrf_wifi_osal_tasklet_init(void *tasklet,
				void (*callbk_fn)(unsigned long),
				unsigned long data);


/**
 * nrf_wifi_osal_tasklet_schedule() - Schedule a tasklet.
 * @tasklet:  Pointer to a tasklet.
 *
 * Schedules a tasklet that had been allocated using
 * @nrf_wifi_osal_tasklet_alloc and initialized using
 * @nrf_wifi_osal_tasklet_init.
 *
 * Return: None.
 */
void nrf_wifi_osal_tasklet_schedule(void *tasklet);


/**
 * nrf_wifi_osal_tasklet_kill() - Terminate a tasklet.
 * @tasklet:  Pointer to a tasklet.
 *
 * Terminates a tasklet(@tasklet) that had been scheduled by
 * @nrf_wifi_osal_tasklet_schedule.
 *
 * Return: None.
 */
void nrf_wifi_osal_tasklet_kill(void *tasklet);


/**
 * nrf_wifi_osal_sleep_ms() - Sleep for a specified duration in milliseconds.
 * @msecs: Sleep duration in milliseconds.
 *
 * Puts the calling thread to sleep for atleast @msecs milliseconds.
 *
 * Return: None.
 */
void nrf_wifi_osal_sleep_ms(unsigned int msecs);


/**
 * nrf_wifi_osal_delay_us() - Delay for a specified duration in microseconds.
 * @usecs: Delay duration in microseconds.
 *
 * Delays execution of calling thread for @usecs microseconds. This is
 * busy-waiting and won't allow other threads to execute during
 * the time lapse.
 *
 * Return: None.
 */
void nrf_wifi_osal_delay_us(unsigned long usecs);


/**
 * nrf_wifi_osal_time_get_curr_us() - Get current system uptime in microseconds.
 *
 * Gets the current system uptime in microseconds.
 *
 * Return: System uptime in microseconds.
 */
unsigned long nrf_wifi_osal_time_get_curr_us(void);


/**
 * nrf_wifi_osal_time_elapsed_us() - Get elapsed time in microseconds
 * @start_time_us: The timestamp in microseconds from which elapsed
 *                 time is to be measured.
 *
 * Returns the time elapsed in microseconds since some
 * time instant (@start_time_us).
 *
 * Return: Elapsed time in microseconds.
 */
unsigned int nrf_wifi_osal_time_elapsed_us(unsigned long start_time_us);



/**
 * nrf_wifi_osal_bus_pcie_init() - Initialize a PCIe driver.
 * @dev_name: Name of the PCIe device.
 * @vendor_id: Vendor ID of the PCIe device.
 * @sub_vendor_id: Sub-vendor ID of the PCIE device.
 * @device_id: Device ID of the PCIe device.
 * @sub_device_id: Sub-device ID of the PCIe device.
 *
 * Registers a PCIe device driver to the OS's PCIe core.
 *
 * Return: OS specific PCIe device context.
 */
void *nrf_wifi_osal_bus_pcie_init(const char *dev_name,
				  unsigned int vendor_id,
				  unsigned int sub_vendor_id,
				  unsigned int device_id,
				  unsigned int sub_device_id);


/**
 * nrf_wifi_osal_bus_pcie_deinit() - Deinitialize a PCIe device driver.
 * @os_pcie_priv: OS specific PCIe context.
 *
 * This API should be called when the PCIe device driver is
 * to be unregistered from the OS's PCIe core.
 *
 * Return: None.
 */
void nrf_wifi_osal_bus_pcie_deinit(void *os_pcie_priv);


/**
 * nrf_wifi_osal_bus_pcie_dev_add() - Add a PCIe device instance.
 * @osal_pcie_dev_ctx: Pointer to the OSAL PCIe device context.
 *
 * Function to be invoked when a matching PCIe device is added to the system.
 * It expects the pointer to a OS specific PCIe device context to be returned.
 *
 * Return: OS specific PCIe device context.
 */
void *nrf_wifi_osal_bus_pcie_dev_add(void *os_pcie_priv,
				     void *osal_pcie_dev_ctx);


/**
 * nrf_wifi_osal_bus_pcie_dev_rem() - Remove a PCIe device instance.
 * @os_pcie_dev_ctx: Pointer to the OS specific PCIe device context which was
 *                   returned by @nrf_wifi_osal_bus_pcie_dev_add.
 *
 * Function to be invoked when a matching PCIe device is removed from the system.
 *
 * Return: None.
 */
void nrf_wifi_osal_bus_pcie_dev_rem(void *os_pcie_dev_ctx);


/**
 * nrf_wifi_osal_bus_pcie_dev_init() - Initialize a PCIe device instance.
  * @os_pcie_dev_ctx: Pointer to the OS specific PCIe device context which was
 *                   returned by @nrf_wifi_osal_bus_pcie_dev_add.
 *
 * Function to be invoked when a PCIe device is to be initialized.
 *
 * Return:
 *		Pass: NRF_WIFI_STATUS_SUCCESS.
 *		Fail: NRF_WIFI_STATUS_FAIL.
 */
enum nrf_wifi_status nrf_wifi_osal_bus_pcie_dev_init(void *os_pcie_dev_ctx);


/**
 * nrf_wifi_osal_bus_pcie_dev_deinit() - Deinitialize a PCIe device instance.
  * @os_pcie_dev_ctx: Pointer to the OS specific PCIe device context which was
 *                   returned by @nrf_wifi_osal_bus_pcie_dev_add.
 *
 * Function to be invoked when a PCIe device is to be deinitialized.
 *
 * Return: None.
 */
void nrf_wifi_osal_bus_pcie_dev_deinit(void *os_pcie_dev_ctx);


/**
 * nrf_wifi_osal_bus_pcie_dev_intr_reg() - Register an interrupt handler for a PCIe device.
 * @os_pcie_dev_ctx: OS specific PCIe device context.
 * @callbk_data: Data to be passed to the ISR.
 * @callbk_fn: ISR to be invoked on receiving an interrupt.
 *
 * Registers an interrupt handler to the OS. This API also passes the callback
 * data to be passed to the ISR when an interrupt is received.
 *
 * Return:
 *	    Pass: NRF_WIFI_STATUS_SUCCESS.
 *	    Fail: NRF_WIFI_STATUS_FAIL.
 */
enum nrf_wifi_status nrf_wifi_osal_bus_pcie_dev_intr_reg(void *os_pcie_dev_ctx,
							 void *callbk_data,
							 int (*callbk_fn)(void *callbk_data));


/**
 * nrf_wifi_osal_bus_pcie_dev_intr_unreg() - Unregister an interrupt handler for a PCIe device.
 * @os_pcie_dev_ctx: OS specific PCIe device context.
 *
 * Unregisters the interrupt handler that was registered using
 * @nrf_wifi_osal_bus_pcie_dev_intr_reg.
 *
 * Return: None.
 */
void nrf_wifi_osal_bus_pcie_dev_intr_unreg(void *os_pcie_dev_ctx);


/**
 * nrf_wifi_osal_bus_pcie_dev_dma_map() - Map host memory for DMA access.
 * @os_pcie_dev_ctx: Pointer to a OS specific PCIe device handle.
 * @virt_addr: Virtual host address to be DMA mapped.
 * @size: Size in bytes of the host memory to be DMA mapped.
 * @dir: DMA direction.
 *
 * Maps host memory of @size bytes pointed to by the virtual address
 * @virt_addr to be used by the device(@dma_dev) for DMAing contents.
 * The contents are available for DMAing to the device if @dir has a
 * value of @NRF_WIFI_OSAL_DMA_DIR_TO_DEV. Conversely the device can DMA
 * contents to the host memory if @dir has a value of
 * @NRF_WIFI_OSAL_DMA_DIR_FROM_DEV. The function returns the DMA address
 * of the mapped memory.
 *
 * Return:
 *		Pass: Pointer to the DMA mapped physical address.
 *		Error: NULL.
 */
void *nrf_wifi_osal_bus_pcie_dev_dma_map(void *os_pcie_dev_ctx,
					 void *virt_addr,
					 size_t size,
					 enum nrf_wifi_osal_dma_dir dir);


/**
 * nrf_wifi_osal_bus_pcie_dev_dma_unmap() - Unmap DMA mapped host memory.
 * @os_pcie_dev_ctx: Pointer to a OS specific PCIe device handle.
 * @dma_addr: DMA mapped physical host memory address.
 * @size: Size in bytes of the DMA mapped host memory.
 * @dir: DMA direction.
 *
 * Unmaps the host memory which was mapped for DMA using @nrf_wifi_osal_dma_map.
 *
 * Return: None.
 */
void nrf_wifi_osal_bus_pcie_dev_dma_unmap(void *os_pcie_dev_ctx,
					  void *dma_addr,
					  size_t size,
					  enum nrf_wifi_osal_dma_dir dir);


/**
 * nrf_wifi_osal_bus_pcie_dev_host_map_get() - Get host mapped address for a PCIe device.
 * @os_pcie_dev_ctx: OS specific PCIe device context.
 * @host_map: Host map address information.
 *
 * Gets the host map address for a PCIe device.
 *
 * Return: None.
 */
void nrf_wifi_osal_bus_pcie_dev_host_map_get(void *os_pcie_dev_ctx,
					     struct nrf_wifi_osal_host_map *host_map);




/**
 * nrf_wifi_osal_bus_qspi_init() - Initialize a qspi driver.
 *
 * Registers a qspi device driver to the OS's qspi core.
 *
 * Return: OS specific qspi device context.
 */
void *nrf_wifi_osal_bus_qspi_init(void);


/**
 * nrf_wifi_osal_bus_qspi_deinit() - Deinitialize a qspi device driver.
 * @os_qspi_priv: OS specific qspi context.
 *
 * This API should be called when the qspi device driver is
 * to be unregistered from the OS's qspi core.
 *
 * Return: None.
 */
void nrf_wifi_osal_bus_qspi_deinit(void *os_qspi_priv);


/**
 * nrf_wifi_osal_bus_qspi_dev_add() - Add a qspi device instance.
 * @osal_qspi_dev_ctx: Pointer to the OSAL qspi device context.
 *
 * Function to be invoked when a matching qspi device is added to the system.
 * It expects the pointer to a OS specific qspi device context to be returned.
 *
 * Return: OS specific qspi device context.
 */
void *nrf_wifi_osal_bus_qspi_dev_add(void *os_qspi_priv,
				     void *osal_qspi_dev_ctx);


/**
 * nrf_wifi_osal_bus_qspi_dev_rem() - Remove a qspi device instance.
 * @os_qspi_dev_ctx: Pointer to the OS specific qspi device context which was
 *                   returned by @nrf_wifi_osal_bus_qspi_dev_add.
 *
 * Function to be invoked when a matching qspi device is removed from the system.
 *
 * Return: None.
 */
void nrf_wifi_osal_bus_qspi_dev_rem(void *os_qspi_dev_ctx);


/**
 * nrf_wifi_osal_bus_qspi_dev_init() - Initialize a qspi device instance.
 * @os_qspi_dev_ctx: Pointer to the OS specific qspi device context which was
 *                   returned by @nrf_wifi_osal_bus_qspi_dev_add.
 *
 * Function to be invoked when a qspi device is to be initialized.
 *
 * Return:
 *		Pass: NRF_WIFI_STATUS_SUCCESS.
 *		Fail: NRF_WIFI_STATUS_FAIL.
 */
enum nrf_wifi_status nrf_wifi_osal_bus_qspi_dev_init(void *os_qspi_dev_ctx);


/**
 * nrf_wifi_osal_bus_qspi_dev_deinit() - Deinitialize a qspi device instance.
 * @os_qspi_dev_ctx: Pointer to the OS specific qspi device context which was
 *                   returned by @nrf_wifi_osal_bus_qspi_dev_add.
 *
 * Function to be invoked when a qspi device is to be deinitialized.
 *
 * Return: None.
 */
void nrf_wifi_osal_bus_qspi_dev_deinit(void *os_qspi_dev_ctx);


/**
 * nrf_wifi_osal_bus_qspi_dev_intr_reg() - Register a interrupt handler for a qspi device.
 * @os_qspi_dev_ctx: OS specific qspi device context.
 * @callbk_data: Data to be passed to the ISR.
 * @callbk_fn: ISR to be invoked on receiving an interrupt.
 *
 * Registers an interrupt handler to the OS. This API also passes the callback
 * data to be passed to the ISR when an interrupt is received.
 *
 * Return:
 *	    Pass: NRF_WIFI_STATUS_SUCCESS.
 *	    Fail: NRF_WIFI_STATUS_FAIL.
 */
enum nrf_wifi_status nrf_wifi_osal_bus_qspi_dev_intr_reg(void *os_qspi_dev_ctx,
							 void *callbk_data,
							 int (*callbk_fn)(void *callbk_data));


/**
 * nrf_wifi_osal_bus_qspi_dev_intr_unreg() - Unregister an interrupt handler for a qspi device.
 * @os_qspi_dev_ctx: OS specific qspi device context.
 *
 * Unregisters the interrupt handler that was registered using
 * @nrf_wifi_osal_bus_qspi_dev_intr_reg.
 *
 * Return: None.
 */
void nrf_wifi_osal_bus_qspi_dev_intr_unreg(void *os_qspi_dev_ctx);


/**
 * nrf_wifi_osal_bus_qspi_dev_host_map_get() - Get host mapped address for a qspi device.
 * @os_qspi_dev_ctx: OS specific qspi device context.
 * @host_map: Host map address information.
 *
 * Gets the host map address for a qspi device.
 *
 * Return: None.
 */
void nrf_wifi_osal_bus_qspi_dev_host_map_get(void *os_qspi_dev_ctx,
					     struct nrf_wifi_osal_host_map *host_map);

/**
 * nrf_wifi_osal_qspi_read_reg32() - Read value from a 32 bit register on a
 *                                  QSPI slave device.
 * @priv:
 * @addr: Address of the register to read from.
 *
 * Reads value from a 32 bit device register at address(@addr) on
 * a QSPI slave device.
 *
 * Return: 32 bit value read from register.
 */
unsigned int nrf_wifi_osal_qspi_read_reg32(void *priv,
					   unsigned long addr);

/**
 * nrf_wifi_osal_qspi_write_reg32() -
 * @priv:
 * @addr: Address of the register to write to.
 * @val: Value to be written to the register.
 *
 * Writes a 32 bit value (@val) to a 32 bit device register at address(@addr)
 * on a QSPI slave device.
 *
 * Return: None.
 */
void nrf_wifi_osal_qspi_write_reg32(void *priv,
				    unsigned long addr,
				    unsigned int val);

/**
 * nrf_wifi_osal_qspi_cpy_from() -
 * @priv:
 * @dest:
 * @src:
 * @count:
 *
 *
 * Return: None.
 */
void nrf_wifi_osal_qspi_cpy_from(void *priv,
				 void *dest,
				 unsigned long addr,
				 size_t count);

/**
 * nrf_wifi_osal_qspi_cpy_to() -
 * @priv:
 * @dest:
 * @src:
 * @count:
 *
 *
 * Return: None.
 */
void nrf_wifi_osal_qspi_cpy_to(void *priv,
			       unsigned long addr,
			       const void *src,
			       size_t count);

/**
 * nrf_wifi_osal_bus_spi_init() - Initialize a spi driver.
 *
 * Registers a spi device driver to the OS's spi core.
 *
 * Return: OS specific spi device context.
 */
void *nrf_wifi_osal_bus_spi_init(void);

/**
 * nrf_wifi_osal_bus_spi_deinit() - Deinitialize a spi device driver.
 * @os_spi_priv: OS specific spi context.
 *
 * This API should be called when the spi device driver is
 * to be unregistered from the OS's spi core.
 *
 * Return: None.
 */
void nrf_wifi_osal_bus_spi_deinit(void *os_spi_priv);


/**
 * nrf_wifi_osal_bus_spi_dev_add() - Add a spi device instance.
 * @osal_spi_dev_ctx: Pointer to the OSAL spi device context.
 *
 * Function to be invoked when a matching spi device is added to the system.
 * It expects the pointer to a OS specific spi device context to be returned.
 *
 * Return: OS specific spi device context.
 */
void *nrf_wifi_osal_bus_spi_dev_add(void *os_spi_priv,
				    void *osal_spi_dev_ctx);


/**
 * nrf_wifi_osal_bus_spi_dev_rem() - Remove a spi device instance.
 * @os_spi_dev_ctx: Pointer to the OS specific spi device context which was
 *                  returned by @nrf_wifi_osal_bus_spi_dev_add.
 *
 * Function to be invoked when a matching spi device is removed from the system.
 *
 * Return: None.
 */
void nrf_wifi_osal_bus_spi_dev_rem(void *os_spi_dev_ctx);


/**
 * nrf_wifi_osal_bus_spi_dev_init() - Initialize a spi device instance.
 * @os_spi_dev_ctx: Pointer to the OS specific spi device context which was
 *                  returned by @nrf_wifi_osal_bus_spi_dev_add.
 *
 * Function to be invoked when a spi device is to be initialized.
 *
 * Return:
 *		Pass: nrf_wifi_STATUS_SUCCESS.
 *		Fail: nrf_wifi_STATUS_FAIL.
 */
enum nrf_wifi_status nrf_wifi_osal_bus_spi_dev_init(void *os_spi_dev_ctx);


/**
 * nrf_wifi_osal_bus_spi_dev_deinit() - Deinitialize a spi device instance.
 * @os_spi_dev_ctx: Pointer to the OS specific spi device context which was
 *                  returned by @nrf_wifi_osal_bus_spi_dev_add.
 *
 * Function to be invoked when a spi device is to be deinitialized.
 *
 * Return: None.
 */
void nrf_wifi_osal_bus_spi_dev_deinit(void *os_spi_dev_ctx);


/**
 * nrf_wifi_osal_bus_spi_dev_intr_reg() - Register a interrupt handler for a spi device.
 * @os_spi_dev_ctx: OS specific spi device context.
 * @callbk_data: Data to be passed to the ISR.
 * @callbk_fn: ISR to be invoked on receiving an interrupt.
 *
 * Registers an interrupt handler to the OS. This API also passes the callback
 * data to be passed to the ISR when an interrupt is received.
 *
 * Return:
 *	    Pass: nrf_wifi_STATUS_SUCCESS.
 *	    Fail: nrf_wifi_STATUS_FAIL.
 */
enum nrf_wifi_status nrf_wifi_osal_bus_spi_dev_intr_reg(void *os_spi_dev_ctx,
							void *callbk_data,
							int (*callbk_fn)(void *callbk_data));


/**
 * nrf_wifi_osal_bus_spi_dev_intr_unreg() - Unregister an interrupt handler for a spi device.
 * @os_spi_dev_ctx: OS specific spi device context.
 *
 * Unregisters the interrupt handler that was registered using
 * @nrf_wifi_osal_bus_spi_dev_intr_reg.
 *
 * Return: None.
 */
void nrf_wifi_osal_bus_spi_dev_intr_unreg(void *os_spi_dev_ctx);


/**
 * nrf_wifi_osal_bus_spi_dev_host_map_get() - Get host mapped address for a spi device.
 * @os_spi_dev_ctx: OS specific spi device context.
 * @host_map: Host map address information.
 *
 * Gets the host map address for a spi device.
 *
 * Return: None.
 */
void nrf_wifi_osal_bus_spi_dev_host_map_get(void *os_spi_dev_ctx,
					    struct nrf_wifi_osal_host_map *host_map);

/**
 * nrf_wifi_osal_spi_read_reg32() - Read value from a 32 bit register on a
 *                                  Linux SPI slave device.
 * @priv:
 * @addr: Address of the register to read from.
 *
 * Reads value from a 32 bit device register at address(@addr) on
 * a QSPI slave device.
 *
 * Return: 32 bit value read from register.
 */
unsigned int nrf_wifi_osal_spi_read_reg32(void *priv,
					  unsigned long addr);

/**
 * nrf_wifi_osal_spi_write_reg32() -
 * @priv:
 * @addr: Address of the register to write to.
 * @val: Value to be written to the register.
 *
 * Writes a 32 bit value (@val) to a 32 bit device register at address(@addr)
 * on a Linux SPI slave device.
 *
 * Return: None.
 */
void nrf_wifi_osal_spi_write_reg32(void *priv,
				   unsigned long addr,
				   unsigned int val);

/**
 * nrf_wifi_osal_spi_cpy_from() -
 * @priv:
 * @dest:
 * @src:
 * @count:
 *
 *
 * Return: None.
 */
void nrf_wifi_osal_spi_cpy_from(void *priv,
				void *dest,
				unsigned long addr,
				size_t count);

/**
 * nrf_wifi_osal_spi_cpy_to() -
 * @priv:
 * @dest:
 * @src:
 * @count:
 *
 *
 * Return: None.
 */
void nrf_wifi_osal_spi_cpy_to(void *priv,
			      unsigned long addr,
			      const void *src,
			      size_t count);


#ifdef CONFIG_NRF_WIFI_LOW_POWER
/**
 * nrf_wifi_osal_timer_alloc() - Allocate a timer.
 *
 * Allocates a timer instance.
 *
 * Return: Pointer to the allocated timer instance.
 */
void *nrf_wifi_osal_timer_alloc(void);


/**
 * nrf_wifi_osal_timer_free() - Free a timer.
 * @timer: Pointer to a timer instance.
 *
 * Frees/Deallocates a timer that has been allocated using
 * @nrf_wifi_osal_timer_alloc.
 *
 * Return: None.
 */
void nrf_wifi_osal_timer_free(void *timer);


/**
 * nrf_wifi_osal_timer_init() - Initialize a timer.
 * @timer: Pointer to a timer instance.
 * @callbk_fn: Callback function to be invoked when the timer expires.
 * @data: Data to be passed to the callback function.
 *
 * Initializes a timer that has been allocated using @nrf_wifi_osal_timer_alloc
 * Need to pass (@callbk_fn) callback function with the data(@data) to be
 * passed to the callback function, whenever the timer expires.
 *
 * Return: None.
 */
void nrf_wifi_osal_timer_init(void *timer,
			      void (*callbk_fn)(unsigned long),
			      unsigned long data);


/**
 * nrf_wifi_osal_timer_schedule() - Schedule a timer.
 * @timer: Pointer to a timer instance.
 * @duration: Duration of the timer in seconds.
 *
 * Schedules a timer with a @duration seconds that has been allocated using
 * @nrf_wifi_osal_timer_alloc and initialized with @nrf_wifi_osal_timer_init.
 *
 * Return: None.
 */
void nrf_wifi_osal_timer_schedule(void *timer,
				  unsigned long duration);


/**
 * nrf_wifi_osal_timer_kill() - Kill a timer.
 * @timer: Pointer to a timer instance.
 *
 * Kills a timer that has been scheduled using @nrf_wifi_osal_timer_schedule.
 *
 * Return: None.
 */
void nrf_wifi_osal_timer_kill(void *timer);



int nrf_wifi_osal_bus_qspi_ps_sleep(void *os_qspi_priv);

int nrf_wifi_osal_bus_qspi_ps_wake(void *os_qspi_priv);

int nrf_wifi_osal_bus_qspi_ps_status(void *os_qspi_priv);
#endif /* CONFIG_NRF_WIFI_LOW_POWER */

/**
 * nrf_wifi_osal_assert() - Assert a condition with a value.
 * @test: Variable to be tested.
 * @val: Value to be checked for the @test
 * @op: type of operation to be done during assertion check.
 * @msg: Assertion message.
 *
 * Compares @test aith @val. If true prints assert message.
 *
 * Return:
 *              None
 */
void nrf_wifi_osal_assert(int test,
			  int val,
			  enum nrf_wifi_assert_op_type op,
			  char *msg);

/**
 * nrf_wifi_osal_strlen() - Gives the length of the string @str.
 * @str: Pointer to the memory location of the string.
 *
 * Calculates the length of the string pointed to by @str
 *
 * Return:
 *              returns the number of bytes of the string @str.
 */
unsigned int nrf_wifi_osal_strlen(const void *str);

/**
 * nrf_wifi_osal_mem_cmp() - Compare contents from one memory location to another.
 * @addr1: Pointer to the memory location of first address.
 * @addr2: Pointer to the memory location of second address.
 * @count: Number of bytes to be compared.
 *
 * Compares @count number of bytes from @addr1 location in memory to @addr2
 * location in memory.
 *
 * Return:
 *              returns an integer less than, equal to, or greater than zero
 */
int nrf_wifi_osal_mem_cmp(const void *addr1,
			  const void *addr2,
			  size_t count);
#endif /* __OSAL_API_H__ */
