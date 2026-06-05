/*
 * application.c
 *
 *  Created on: Oct 20, 2020
 *      Author: max
 */

#include "main.h"
#include "cmsis_os2.h"
#include "stm32l4xx_hal_tim.h"
#include "stm32l4xx_hal_tim_ex.h"
#include "io.h"
#include <stdio.h>

// Declare external references to timer instances

extern TIM_HandleTypeDef htim1;

// Simple logging macro
#define LOG(fmt, ...) printf("[DEBUG] " fmt "\r\n", ##__VA_ARGS__)

// Dummy __io_putchar for printf support
// In a real scenario, this would send data to UART or ITM
int __io_putchar(int ch) {
    // Placeholder for actual output implementation
    return ch;
}

// Callback used by the TIM1 on period interrupt
// Registered before starting timer1

void tim1Callback( TIM_HandleTypeDef *htim ) {
	HAL_GPIO_WritePin( IO10_GPIO_GPIO_Port, IO10_GPIO_Pin, GPIO_PIN_SET );
	HAL_GPIO_WritePin( IO10_GPIO_GPIO_Port, IO10_GPIO_Pin, GPIO_PIN_RESET );
}

// Declaration of queues, defined in main.c

extern osMessageQueueId_t acquisitionQueueHandle;
extern osMessageQueueId_t displayQueueHandle;

void StartDefaultTask( void *argument ) {
    LOG("System started. Initializing LCD...");

    // Show '1234' on the LCD display
    lcdWriteDigit('1', 0);
    lcdWriteDigit('2', 1);
    lcdWriteDigit('3', 2);
    lcdWriteDigit('4', 3);
    lcdUpdateDisplay();

    LOG("LCD updated with '1234'");

	while( 1 ) {
		if( getSwitch0() ) { // Blink LEDs
            LOG("Switch 0 pressed - Blinking LEDs");
			while( getSwitch0() ) {
				led0( OFF );
				led1( ON );
				led2( OFF );
				led3( ON );
				osDelay( 500 );
				led0( ON );
				led1( OFF );
				led2( ON );
				led3( OFF );
				osDelay( 500 );
			}
            LOG("Switch 0 released");
		} else if( getSwitch1() ) { // This must be filled by students, to perform requested functions
            LOG("Switch 1 pressed");
			while( getSwitch1() );
            LOG("Switch 1 released");
		}
        osDelay(100);
	}
}

