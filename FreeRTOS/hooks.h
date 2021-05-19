#ifndef MY_HOOKS_H
#define MY_HOOKS_H

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Utility functions to implement run time stats on Cortex-M CPUs.  The collected
run time data can be viewed through the CLI interface.  See the following URL for
more information on run time stats:
http://www.freertos.org/rtos-run-time-stats.html */

/* Addresses of registers in the Cortex-M debug hardware. */
#define rtsDWT_CYCCNT 			( *( ( unsigned long * ) 0xE0001004 ) )
#define rtsDWT_CONTROL 			( *( ( unsigned long * ) 0xE0001000 ) )
#define rtsSCB_DEMCR 			( *( ( unsigned long * ) 0xE000EDFC ) )
#define rtsTRCENA_BIT			( 0x01000000UL )
#define rtsCOUNTER_ENABLE_BIT	( 0x01UL )

/* Simple shift divide for scaling to avoid an overflow occurring too soon. */
#define runtimeSHIFT_13				13
#define runtimeOVERFLOW_BIT_13		( 1UL << ( 32UL - runtimeSHIFT_13 ) )
static const uint32_t ulPrescaleBits = runtimeSHIFT_13;
static const uint32_t ulOverflowBit = runtimeOVERFLOW_BIT_13;

/*-----------------------------------------------------------*/

void vConfigureTimerForRunTimeStats( void )
{
	/* Enable TRCENA. */
	rtsSCB_DEMCR = rtsSCB_DEMCR | rtsTRCENA_BIT;

	/* Reset counter. */
	rtsDWT_CYCCNT = 0;

	/* Enable counter. */
	rtsDWT_CONTROL = rtsDWT_CONTROL | rtsCOUNTER_ENABLE_BIT;
}
/*-----------------------------------------------------------*/

uint32_t ulGetRunTimeCounterValue( void )
{
static unsigned long ulLastCounterValue = 0UL, ulOverflows = 0;
unsigned long ulValueNow;

	ulValueNow = rtsDWT_CYCCNT;

	/* Has the value overflowed since it was last read. */
	if( ulValueNow < ulLastCounterValue )
	{
		ulOverflows += ulOverflowBit;
	}
	ulLastCounterValue = ulValueNow;

	/* There is no prescale on the counter, so simulate in software. */
	ulValueNow = ( ulValueNow >> ulPrescaleBits ) + ulOverflows;

	return ulValueNow;
}
/*-----------------------------------------------------------*/

/* This function will be called by each tick interrupt if
		configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
		added here, but the tick hook is called from an interrupt context, so
		code must not attempt to block, and only the interrupt safe FreeRTOS API
		functions can be used (those that end in FromISR()). */
void vApplicationTickHook(void) {

    //__WFI();
}

/* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created.  It is also called by various parts of the
   demo application.  If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
void vApplicationMallocFailedHook(void) {
  taskDISABLE_INTERRUPTS();
  for(;;);
}

/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
   task.  It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()).  If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
void vApplicationIdleHook(void) {
}

void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName) {
  (void) pcTaskName;
  (void) pxTask;
  /* Run time stack overflow checking is performed if
     configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
     function is called if a stack overflow is detected. */
  taskDISABLE_INTERRUPTS();
  for(;;);
}

StaticTask_t xIdleTaskTCB;
StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize) {
  /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
  state will be stored. */
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

  /* Pass out the array that will be used as the Idle task's stack. */
  *ppxIdleTaskStackBuffer = uxIdleTaskStack;

  /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
  Note that, as the array is necessarily of type StackType_t,
  configMINIMAL_STACK_SIZE is specified in words, not bytes. */
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize) {
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
  *ppxTimerTaskStackBuffer = uxTimerTaskStack;
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

void vPreSleepProcessing( uint32_t ulExpectedIdleTime ){
    // do nothing.
}

#ifdef __cplusplus
}
#endif

#endif //MY_HOOKS_H
