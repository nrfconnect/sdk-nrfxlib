/*
 * Copyright (c) 2018-2021 T2 Software, Inc. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-T2-LC3
 */

#ifndef __OSAL_H__
#define __OSAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"
#include "osal_types.h"

/**@brief OSAL status values.
 */
#define OSAL_STATUS_SUCCESS               ( 0)
#define OSAL_STATUS_INVALID_PARAMETER     (-1)
#define OSAL_STATUS_OUT_OF_MEMORY         (-2)
#define OSAL_STATUS_INTERNAL_ERROR        (-3)

/**@brief Infinite timeout value.
 */
#define OSAL_TIMEOUT_INFINITE                (0xFFFFFFFF)

/**@brief OSAL mutex handle.
 */
typedef void *OSAL_Mutex_t;

/**@brief OSAL timer handle.
 */
typedef void *OSAL_Timer_t;

/**@brief OSAL thread handle.
 */
typedef void *OSAL_Thread_t;

/**@brief OSAL event handle.
 */
typedef void *OSAL_Event_t;

/**@brief OSAL timer types.
 */
typedef enum
{
   OSAL_TIMER_PERIODIC,
   OSAL_TIMER_ONE_SHOT
} OSAL_Timer_Type_t;

/**@brief Timer function type.
 */
typedef void(*OSAL_Timer_Function_t)(OSAL_Timer_t TimerHandle, void *CallbackData);

/**@brief Thread function type.
 */
typedef void(*OSAL_Thread_Function_t)(OSAL_Thread_t ThreadHandle, void *Data);

/**@brief Allocates heap memory.
 *
 * @param[in] Size Size of the memory to allocate.
 *
 * @returns Pointer to the memory allocated or NULL upon failure.
 */
void *OSAL_Malloc(size_t Size);

/**@brief Frees heap memory.
 *
 * @param[in] Ptr Pointer to the memory that was allocated in a call to
                  OSAL_Malloc().
 */
void OSAL_Free(void *Ptr);

/**@brief Sets buffer entries to a value.
 *
 * @param[in] Ptr   Pointer to the buffer to set.
 * @param[in] Value Value to set the buffer to.
 * @param[in] Num   Size of the buffer.
 *
 * @returns Pointer to the input buffer.
 */
void *OSAL_Memset(void *Ptr, int Value, size_t Num);

/**@brief Copies data from one buffer to another.
 *
 * @param[in] Dst Copy destination buffer.
 * @param[in] Src Copy source buffer.
 * @param[in] Num Size of the data to copy.
 *
 * @returns Pointer to the destination buffer.
 */
void *OSAL_Memcpy(void *Dst, const void *Src, size_t Num);

/**@brief Compares data between two buffers.
 *
 * @param[in] Ptr1 Pointer to first buffer.
 * @param[in] Ptr2 Pointer to second buffer.
 * @param[in] Num  Size of the data to compare.
 *
 * @returns < 0 if the first non-matching byte has a lower value in ptr1 than in ptr2.
 *          > 0 if the first non-matching byte has a greater value in ptr1 than in ptr2.
 *          0 if the data in both buffers matches.
 */
int OSAL_Memcmp(const void *Ptr1, const void *Ptr2, size_t Num);

/**@brief Creates a mutex.
 *
 * @param[in] Owned Creates the mutex as owned if TRUE or unowned if FALSE.
 *
 * @returns Handle to the created mutex upon successor NULL upon failure.
 */
OSAL_Mutex_t OSAL_Create_Mutex(bool_t Owned);

/**@brief Destroys a mutex.
 *
 * @param[in] Mutex Handle to the mutex returned from OSAL_Create_Mutex().
 */
void OSAL_Close_Mutex(OSAL_Mutex_t Mutex);

/**@brief Waits for ownership of a mutex.
 *
 * @param[in] Mutex   Handle to the mutex returned from OSAL_Create_Mutex().
 * @param[in] Timeout Value in milliseconds to wait for the mutex or OSAL_TIMEOUT_INFINITE
 *                    to wait forever.
 *
 * @returns TRUE upon success or FALSE upon failure.
 */
bool_t OSAL_Wait_Mutex(OSAL_Mutex_t Mutex, unsigned long Timeout);

/**@brief Releases ownership of a mutex.
 *
 * @param[in] Mutex Handle to the mutex returned from OSAL_Create_Mutex().
 */
void OSAL_Release_Mutex(OSAL_Mutex_t Mutex);

/**@brief Creates a timer.
 *
 * @param[in] Delay         Interval on which to trigger the timer.
 * @param[in] Start         Start the timer at creation or not.
 * @param[in] Type          Periodic or one-shot timer.
 * @param[in] TimerFunction Timer handler function.
 * @param[in] Data          Pointer to application data to be provided as an argument to the timer function call.
 *
 * @returns Handle to the created timer upon successor NULL upon failure.
 */
OSAL_Timer_t OSAL_Create_Timer(uint32_t Delay, bool_t Start, OSAL_Timer_Type_t Type, OSAL_Timer_Function_t TimerFunction, void *Data);

/**@brief Starts a timer.
 *
 * @param[in] Timer Handle to the timer returned from OSAL_Create_Timer().
 *
 * @returns OSAL_STATUS_SUCCESS upon success or other status value upon failure.
 */
int OSAL_Start_Timer(OSAL_Timer_t Timer);

/**@brief Frees a timer.
 *
 * @param[in] Timer Handle to the timer returned from OSAL_Create_Timer().
 */
void OSAL_Close_Timer(OSAL_Timer_t Timer);

/**@brief Creates a thread.
 *
 * @param[in] StackSize      Stack size of the thread; may be 0 for the default thread stack size.
 * @param[in] ThreadFunction Timer handler function.
 * @param[in] Data           Pointer to application data to be provided as an argument to the thread function call.
 *
 * @returns Handle to the created thread upon successor NULL upon failure.
 */
OSAL_Thread_t OSAL_Create_Thread(uint32_t StackSize, OSAL_Thread_Function_t ThreadFunction, void *Data);

/**@brief Creates an event.
 *
 * @returns Handle to the created thread upon successor NULL upon failure.
 */
OSAL_Event_t OSAL_Create_Event(void);

/**@brief Waits for one or more event bits to be set.
 *
 * @param[in] Event    Handle to the event returned from OSAL_Create_Event().
 * @param[in] Flags    Bitmask of the event bits to wait for, must include at least one bit.
 * @param[in] FlagsSet Bitmask of the events that were set.
 * @param[in] Reset    If TRUE, resets the event bits that were set.
 * @param[in] Timeout  Value in milliseconds to wait for the event or OSAL_TIMEOUT_INFINITE to wait forever.
 *
 * @returns OSAL_STATUS_SUCCESS upon success or other status value upon failure.
 */
int OSAL_Wait_Event(OSAL_Event_t Event, uint32_t Flags, uint32_t *FlagsSet, bool_t Reset, int Timeout);

/**@brief Sets an event.
 *
 * @param[in] Event Handle to the event returned from OSAL_Create_Event().
 * @param[in] Flags Bitmask of the event bits to set.
 *
 * @returns OSAL_STATUS_SUCCESS upon success or other status value upon failure.
 */
int OSAL_Set_Event(OSAL_Event_t Event, uint32_t Flags);

/**@brief Resets an event.
 *
 * @param[in] Event Handle to the event returned from OSAL_Create_Event().
 * @param[in] Flags Bitmask of the event bits to reset.
 *
 * @returns OSAL_STATUS_SUCCESS upon success or other status value upon failure.
 */
int OSAL_Reset_Event(OSAL_Event_t Event, uint32_t Flags);

/**@brief Frees an event.
 *
 * @param[in] Event Handle to the event returned from OSAL_Create_Event().
 */
void OSAL_Close_Event(OSAL_Event_t Event);

/**@brief Outputs Message to Console.
 *
 * @param[in] DebugString String and Parameters.
 */
void OSAL_OutputMessage(const char *DebugString, ...);

/**@brief Initializes output message buffer for OSAL_OutputMessage.
 *
 * @param[in] bufPtr    Pointer to memory block for OSAL_OutputMessage output.
 * @param[in] size      Size of memory block.
 */
void OSAL_OutputMessageInit(char *bufPtr, uint16_t size);

/**@brief Sends data in debug output buffer.
 *
 */
void OSAL_OutputMessageFlush(void);

/**@brief Outputs a Dump of the supplied Data.
 *
 * @param[in] dataLength   Length of the Data to Dump.
 * @param[in] dataPtr      Pointer to the Data to Dump.
 * @param[in] size
 *
 *  @return OSAL_STATUS_SUCCESS upon success or other status value upon failure.
 */
int OSAL_DumpData(unsigned int dataLength, const unsigned char *dataPtr, int size);

#if defined LC3_CYCLES_PER_BLOCK && __arc__
/**@brief Start Cycle Count.
 * This function calls the external function for saving the starting cycle
 * count for a blockNumber.  This function is a workaround to link the
 * libLC3.a with the Cycle Block Count in the ARC makefile environment.
 *
 * @param[in] blockNumber Index to the cycle block count to start.
 */
void OSAL_CycleCountStart(uint8_t blockNumber);

/**@brief End Cycle Count.
 * This function calls the external function for ending the cycle
 * count for a blockNumber.  This function is a workaround to link the
 * libLC3.a with the Cycle Block Count in the ARC makefile environment.
 *
 * @param[in] blockNumber Index to the cycle block count to end.
 */
void OSAL_CycleCountEnd(uint8_t blockNumber);
#endif


#ifdef __cplusplus
};
#endif

#endif // __OSAL_H__
