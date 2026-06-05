/*
 * acquisition.c
 *
 *  Created on: May 18, 2026
 *      Author: max
 */

#include "main.h"
#include "cmsis_os.h"
#include <stdio.h>

// Simple logging macro
#define LOG(fmt, ...) printf("[ACQ] " fmt "\r\n", ##__VA_ARGS__)

// ADC handles defined in main.c
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

extern osMessageQueueId_t acquisitionQueueHandle;

// Acquisition task entry point

void StartAcquisitionTask( void *argument ) {
  LOG("Acquisition task started");

  uint32_t val3 = 0;
  uint32_t val4 = 0;
  float voltage3 = 0.0f;
  float voltage4 = 0.0f;

  /* Infinite loop */
  for( ; ; ) {
    // Read AN3 (ADC1)
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
        val3 = HAL_ADC_GetValue(&hadc1);
        voltage3 = (val3 * 3.3f) / 4095.0f;
    }
    HAL_ADC_Stop(&hadc1);

    // Read AN4 (ADC2)
    HAL_ADC_Start(&hadc2);
    if (HAL_ADC_PollForConversion(&hadc2, 10) == HAL_OK) {
        val4 = HAL_ADC_GetValue(&hadc2);
        voltage4 = (val4 * 3.3f) / 4095.0f;
    }
    HAL_ADC_Stop(&hadc2);

    LOG("AN3: %lu (%.2fV), AN4: %lu (%.2fV)", val3, voltage3, val4, voltage4);

    // Convert AN3 voltage to millivolts and send to queue
    uint16_t an3_mv = (uint16_t)(voltage3 * 1000.0f);
    osMessageQueuePut(acquisitionQueueHandle, &an3_mv, 0, 0);

    // Run at 10Hz (100ms)
    static uint32_t tick = 0;
    if (tick == 0) tick = osKernelGetTickCount();
    tick += 100;
    osDelayUntil(tick);
  }
}

