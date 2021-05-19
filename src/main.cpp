#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <msp.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "hooks.h"


#include "main.h"
#include "bsp.h"
#include "led.h"
#include "msp432_spi.h"
#include "msp432_uart.h"
#include "led.h"
#include "tinyprintf.h"

#include "Servo.h"

static void setupHardWare(void);
static void servo_task_func(void *pvParams);
static void vPortCleanUpTCB (void *pxTCB);

static StackType_t stack[512];
static StaticTask_t tcb;
static TaskHandle_t taskh;
static Led led;
static Servo servo;
 
int main(void){

	setupHardWare();
	
	printf("PWM Servo Demo\r\n%s\r\n", __TIMESTAMP__);

	xTaskCreateStatic(
		servo_task_func,
		"servo_task",
		512,
		nullptr,
		1,
		&stack[0], // its stack
		&tcb 
	);

	vTaskStartScheduler();

	while(1);
	
}

static void setupHardWare(void){
	taskDISABLE_INTERRUPTS();
	
	/* Disable the watchdog timer */
	MAP_WDT_A_holdTimer();

	/* Ensure the FPU is enabled */
	MAP_FPU_enableModule();
	
	/* Selecting P1.2 and P1.3 in UART mode (USB Connection) */
	UART0_init();

	/* Selecting P3.2 and P3.3 in UART mode for CM10 BTLE uart communication */
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
	
	taskENABLE_INTERRUPTS();
}

static void servo_task_func(void *pvParams){
    
    (void)pvParams;

    for(;;){

		led.led1_toggle();
		
		printf("Going to negative 90\r\n");
		servo.goToNegative90Deg();

		vTaskDelay(2000);

		printf("Going to positive 90\r\n");
		servo.goToPositive90Deg();

		vTaskDelay(2000);

		printf("Going to zero degrees\r\n");
		servo.goToZeroDeg();

		vTaskDelay(2000);
	}
		
}

static void vPortCleanUpTCB ( void *pxTCB ) {
// place clean up code here
}


