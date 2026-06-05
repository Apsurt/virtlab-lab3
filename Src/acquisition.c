/*
 * acquisition.c
 *
 *  Created on: May 18, 2026
 *      Author: max
 */

#include "main.h"
#include "cmsis_os.h"
#include "io.h"
#include <stdio.h>
#include <math.h>

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

    // Calculate Temperature in Kelvin
    // 1. Find R_NTC
    // Formula: R_NTC = (10000.0 * V3) / (2.0 * V4 - V3)
    float v3 = voltage3;
    float v4 = voltage4;
    float r_ntc = (10000.0f * v3) / (2.0f * v4 - v3);

    // 2. Calculate T_C (Celsius)
    // Formula: T_C = 25.0 + (log(R_NTC / 10000.0) / log(0.96))
    float t_c = 25.0f + (logf(r_ntc / 10000.0f) / logf(0.96f));

    // 3. Convert to Kelvin
    float t_k = t_c + 273.15f;

    LOG("R_NTC: %.1f Ohm, Temp: %.2f C, %.2f K", r_ntc, t_c, t_k);

    // Send T_K * 10 to display (e.g., 298.1K -> 2981)
    uint16_t display_val = (uint16_t)(t_k * 10.0f);
    osMessageQueuePut(acquisitionQueueHandle, &display_val, 0, 0);

    // Run at 10Hz (100ms)
    static uint32_t tick = 0;
    if (tick == 0) tick = osKernelGetTickCount();
    tick += 100;
    osDelayUntil(tick);
  }
}

