/*
 * Copyright (c) 2018-2021 T2 Software, Inc. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-T2-LC3
 */

#include <string.h>
#include <stdlib.h>
#include "osal.h"

#define OSAL_NUM_DEBUG_MEM_ENTRIES        50

#if OSAL_NUM_DEBUG_MEM_ENTRIES > 0
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#endif

/**@brief Internal memory debug entry type.
 */
typedef struct OSAL_Mem_Debug_Entry_s
{
   void   *Buffer;
   size_t  Length;
} OSAL_Mem_Debug_Entry_t;

#if OSAL_NUM_DEBUG_MEM_ENTRIES > 0
static OSAL_Mem_Debug_Entry_t OSALMemDebugEntries[OSAL_NUM_DEBUG_MEM_ENTRIES];
static size_t                 OSALMemDebugHighWaterMark;
#endif

/**@brief Allocates heap memory.
 *
 * @param[in] Size Size of the memory to allocate.
 *
 * @returns Pointer to the memory allocated or NULL upon failure.
 */
void *OSAL_Malloc(size_t Size)
{
#if OSAL_NUM_DEBUG_MEM_ENTRIES > 0
   uint16_t Index;
   size_t   CurrentHeapUsage = 0;
#endif
   void *RetVal;

   RetVal = malloc(Size);

#if OSAL_NUM_DEBUG_MEM_ENTRIES > 0
   // If memory debug is enabled, store the pointer and size.
   if (RetVal)
   {
      // Find an empty memory entry.
      for (Index = 0; Index < OSAL_NUM_DEBUG_MEM_ENTRIES; Index++)
      {
         if (OSALMemDebugEntries[Index].Buffer == NULL)
         {
            // Store the pointer and length.
            OSALMemDebugEntries[Index].Buffer = RetVal;
            OSALMemDebugEntries[Index].Length = Size;
            break;
         }
      }

      // Ensure we found an entry.
      assert(Index != OSAL_NUM_DEBUG_MEM_ENTRIES);

      // Sum each memory entry.
      for (Index = 0; Index < OSAL_NUM_DEBUG_MEM_ENTRIES; Index++)
      {
         CurrentHeapUsage += OSALMemDebugEntries[Index].Length;
      }

      // Update the high water mark if needed.
      if(CurrentHeapUsage > OSALMemDebugHighWaterMark)
      {
         OSALMemDebugHighWaterMark = CurrentHeapUsage;
      }
   }
#endif

   return RetVal;
}

/**@brief Frees heap memory.
 *
 * @param[in] Ptr Pointer to the memory that was allocated in a call to
                  OSAL_Malloc().
 */
void OSAL_Free(void *Ptr)
{
#if OSAL_NUM_DEBUG_MEM_ENTRIES > 0
   uint16_t Index;
#endif
   free(Ptr);

#if OSAL_NUM_DEBUG_MEM_ENTRIES > 0
   // If memory debug is enabled, remove the pointer and size.
   if (Ptr)
   {
      // Find an empty memory entry.
      for (Index = 0; Index < OSAL_NUM_DEBUG_MEM_ENTRIES; Index++)
      {
         if (OSALMemDebugEntries[Index].Buffer == Ptr)
         {
            // Clear the pointer and length.
            OSALMemDebugEntries[Index].Buffer = NULL;
            OSALMemDebugEntries[Index].Length = 0;
            break;
         }
      }

      // Ensure we found an entry.
      assert(Index != OSAL_NUM_DEBUG_MEM_ENTRIES);
   }
#endif
}

#if OSAL_NUM_DEBUG_MEM_ENTRIES > 0
/**@brief Gets the heap usage.
 *
 * @param[out] HighWaterMark       Optional pointer to the high water mark heap value.
 * @param      ResetHighWaterMark  Flag whether to reset the high water mark or not.
 *
 * @returns Current heap usage in bytes.
 */
size_t OSAL_Current_Heap_Usage(size_t *HighWaterMark, bool_t ResetHighWaterMark)
{
   uint16_t Index;
   size_t   RetVal = 0;

   // Set the caller's high water mark.
   if (HighWaterMark)
   {
      *HighWaterMark = OSALMemDebugHighWaterMark;
   }

   // Reset the high water mark.
   if (ResetHighWaterMark)
   {
      OSALMemDebugHighWaterMark = 0;
   }

   // Sum each memory entry.
   for (Index = 0; Index < OSAL_NUM_DEBUG_MEM_ENTRIES; Index++)
   {
      RetVal += OSALMemDebugEntries[Index].Length;
   }

   return RetVal;
}
#endif

/**@brief Sets buffer entries to a value.
 *
 * @param[in] ptr   Pointer to the buffer to set.
 * @param[in] value Value to set the buffer to.
 * @param[in] num   Size of the buffer.
 *
 * @returns Pointer to the input buffer.
 */
void *OSAL_Memset(void *Ptr, int Value, size_t Num)
{
   void *RetVal;

   RetVal = memset(Ptr, Value, Num);

   return RetVal;
}

/**@brief Copies data from one buffer to another.
 *
 * @param[in] Dst Copy destination buffer.
 * @param[in] Src Copy source buffer.
 * @param[in] Num Size of the data to copy.
 *
 * @returns Pointer to the destination buffer.
 */
void *OSAL_Memcpy(void *dst, const void *src, size_t Num)
{
   void *RetVal;

   RetVal = memcpy(dst, src, Num);

   return RetVal;
}

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
int OSAL_Memcmp(const void *Ptr1, const void *Ptr2, size_t Num)
{
   int RetVal;

   RetVal = memcmp(Ptr1, Ptr2, Num);

   return RetVal;
}

/**@brief Creates a mutex.
 *
 * @param[in] owned Creates the mutex as owned if TRUE or unowned if FALSE.
 *
 * @returns Handle to the created mutex upon successor NULL upon failure.
 */
OSAL_Mutex_t OSAL_Create_Mutex(bool_t Owned)
{
   return (OSAL_Mutex_t)NULL;
}

/**@brief Destroys a mutex.
 *
 * @param[in] mutex Handle to the mutex returned from OSAL_Create_Mutex().
 */
void OSAL_Close_Mutex(OSAL_Mutex_t Mutex)
{
}

/**@brief Waits for ownership of a mutex.
 *
 * @param[in] Mutex   Handle to the mutex returned from OSAL_Create_Mutex().
 * @param[in] Timeout Value in milliseconds to wait for the mutex or OSAL_TIMEOUT_INFINITE
 *                    to wait forever.
 *
 * @returns TRUE upon success or FALSE upon failure.
 */
bool_t OSAL_Wait_Mutex(OSAL_Mutex_t Mutex, unsigned long Timeout)
{
   return FALSE;
}

/**@brief Releases ownership of a mutex.
 *
 * @param[in] mutex Handle to the mutex returned from OSAL_Create_Mutex().
 */
void OSAL_Release_Mutex(OSAL_Mutex_t Mutex)
{
}

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
OSAL_Timer_t OSAL_Create_Timer(uint32_t Delay, bool_t Start, OSAL_Timer_Type_t Type, OSAL_Timer_Function_t TimerFunction, void *Data)
{
   return (OSAL_Timer_t)NULL;
}

/**@brief Starts a timer.
 *
 * @param[in] Timer Handle to the timer returned from OSAL_Create_Timer().
 *
 * @returns OSAL_STATUS_SUCCESS upon success or other status value upon failure.
 */
int OSAL_Start_Timer(OSAL_Timer_t Timer)
{
   return OSAL_STATUS_INTERNAL_ERROR;
}

/**@brief Frees a timer.
 *
 * @param[in] Timer Handle to the timer returned from OSAL_Create_Timer().
 */
void OSAL_Close_Timer(OSAL_Timer_t Timer)
{
}

/**@brief Creates a thread.
 *
 * @param[in] StackSize      Stack size of the thread; may be 0 for the default thread stack size.
 * @param[in] ThreadFunction Timer handler function.
 * @param[in] Data           Pointer to application data to be provided as an argument to the thread function call.
 *
 * @returns Handle to the created thread upon successor NULL upon failure.
 */
OSAL_Thread_t OSAL_Create_Thread(uint32_t StackSize, OSAL_Thread_Function_t ThreadFunction, void *Data)
{
   return (OSAL_Thread_t)NULL;
}

/**@brief Creates an event.
 *
 * @returns Handle to the created thread upon successor NULL upon failure.
 */
OSAL_Event_t OSAL_Create_Event(void)
{
   return (OSAL_Event_t)NULL;
}

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
int OSAL_Wait_Event(OSAL_Event_t Event, uint32_t Flags, uint32_t *FlagsSet, bool_t Reset, int Timeout)
{
   return OSAL_STATUS_INTERNAL_ERROR;
}

/**@brief Sets an event.
 *
 * @param[in] Event Handle to the event returned from OSAL_Create_Event().
 * @param[in] Flags Bitmask of the event bits to set, must include at least one bit.
 *
 * @returns OSAL_STATUS_SUCCESS upon success or other status value upon failure.
 */
int OSAL_Set_Event(OSAL_Event_t Event, uint32_t Flags)
{
   return OSAL_STATUS_INTERNAL_ERROR;
}

/**@brief Resets an event.
 *
 * @param[in] Event Handle to the event returned from OSAL_Create_Event().
 * @param[in] Flags Bitmask of the event bits to reset, must include at least one bit.
 *
 * @returns OSAL_STATUS_SUCCESS upon success or other status value upon failure.
 */
int OSAL_Reset_Event(OSAL_Event_t Event, uint32_t Flags)
{
   return OSAL_STATUS_INTERNAL_ERROR;
}

/**@brief Frees an event.
 *
 * @param[in] Event Handle to the event returned from OSAL_Create_Event().
 */
void OSAL_Close_Event(OSAL_Event_t Event)
{
}

/**@brief Outputs Message to Console.
 *
 * @param[in] Format String and Parameters.
 */
void OSAL_OutputMessage(const char *DebugString, ...)
{
}
