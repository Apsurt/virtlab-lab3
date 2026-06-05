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

// Redirect printf to ITM (Serial Wire Viewer) for STM32CubeIDE
int __io_putchar(int ch) {
    ITM_SendChar(ch);
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
    LOG("Main application loop started at 10Hz.");

    uint32_t tick = osKernelGetTickCount();
    const uint32_t frequency = 100; // 100ms for 10Hz

    while( 1 ) {
        // Toggle LED0 at 10Hz for debugging
        static int led_state = 0;
        led0(led_state ? ON : OFF);
        led_state = !led_state;

        // TODO: Process messages from acquisitionQueueHandle
        // TODO: Send updates to displayQueueHandle

        // Wait for the next cycle (10Hz)
        tick += frequency;
        osDelayUntil(tick);
    }
}

