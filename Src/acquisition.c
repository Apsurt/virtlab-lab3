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

// ADC handles defined in main.c
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim1;
extern osMessageQueueId_t acquisitionQueueHandle;

// Event Flags for task synchronization
osEventFlagsId_t acquisitionEventFlags;

// Flag bits
#define FLAG_TIM1_TRIGGER    (1 << 0)
#define FLAG_ADC1_COMPLETE   (1 << 1)
#define FLAG_ADC2_COMPLETE   (1 << 2)

// Global variables to store raw ADC values from interrupts
static uint32_t adc1_raw = 0;
static uint32_t adc2_raw = 0;

/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim: TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM1) {
    // Signal the task to start acquisition
    osEventFlagsSet(acquisitionEventFlags, FLAG_TIM1_TRIGGER);
  }
}

/**
  * @brief  Conversion complete callback in non blocking mode
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
  if (hadc->Instance == ADC1) {
    adc1_raw = HAL_ADC_GetValue(hadc);
    osEventFlagsSet(acquisitionEventFlags, FLAG_ADC1_COMPLETE);
  } else if (hadc->Instance == ADC2) {
    adc2_raw = HAL_ADC_GetValue(hadc);
    osEventFlagsSet(acquisitionEventFlags, FLAG_ADC2_COMPLETE);
  }
}

// Acquisition task entry point
void StartAcquisitionTask( void *argument ) {
  // Create Event Flags
  acquisitionEventFlags = osEventFlagsNew(NULL);
  
  // Configure TIM1 for 10Hz
  // Clock is 80MHz. 80,000,000 / (7999 + 1) = 10,000 Hz timer frequency
  // 10,000 / (999 + 1) = 10 Hz interrupt frequency
  __HAL_TIM_SET_PRESCALER(&htim1, 7999);
  __HAL_TIM_SET_AUTORELOAD(&htim1, 999);
  
  // Start Timer in interrupt mode
  HAL_TIM_Base_Start_IT(&htim1);

  float voltage3 = 0.0f;
  float voltage4 = 0.0f;

  /* Infinite loop */
  for( ; ; ) {
    // Wait for TIM1 to trigger (10Hz)
    osEventFlagsWait(acquisitionEventFlags, FLAG_TIM1_TRIGGER, osFlagsWaitAny, osWaitForever);

    // Start ADC1 Conversion in interrupt mode
    HAL_ADC_Start_IT(&hadc1);
    // Wait for ADC1 completion
    osEventFlagsWait(acquisitionEventFlags, FLAG_ADC1_COMPLETE, osFlagsWaitAny, osWaitForever);
    voltage3 = (adc1_raw * 3.3f) / 4095.0f;

    // Start ADC2 Conversion in interrupt mode
    HAL_ADC_Start_IT(&hadc2);
    // Wait for ADC2 completion
    osEventFlagsWait(acquisitionEventFlags, FLAG_ADC2_COMPLETE, osFlagsWaitAny, osWaitForever);
    voltage4 = (adc2_raw * 3.3f) / 4095.0f;

    // Preserve original logic
    float r_ntc = (10000.0f * voltage3) / (2.0f * voltage4 - voltage3);
    float t_c = 25.0f + (logf(r_ntc / 10000.0f) / logf(0.96f));
    float t_k = t_c + 273.15f;

    uint16_t display_val;
    if (getSwitch0()) {
        display_val = (uint16_t)(t_k * 10.0f);
    } else {
        display_val = (uint16_t)(t_c * 10.0f);
    }
    
    osMessageQueuePut(acquisitionQueueHandle, &display_val, 0, 0);
  }
}
