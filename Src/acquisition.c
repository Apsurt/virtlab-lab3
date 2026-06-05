/*
 * acquisition.c
 *
 *  Created on: June 5, 2026
 *      Author: tymonb
 */

#include "main.h"
#include "cmsis_os.h"
#include "io.h"
#include <stdio.h>
#include <math.h>

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern osMessageQueueId_t acquisitionQueueHandle;

void StartAcquisitionTask( void *argument ) {
  uint32_t val3 = 0;
  uint32_t val4 = 0;
  float voltage3 = 0.0f;
  float voltage4 = 0.0f;

  for( ; ; ) {
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
        val3 = HAL_ADC_GetValue(&hadc1);
        voltage3 = (val3 * 3.3f) / 4095.0f;
    }
    HAL_ADC_Stop(&hadc1);

    HAL_ADC_Start(&hadc2);
    if (HAL_ADC_PollForConversion(&hadc2, 10) == HAL_OK) {
        val4 = HAL_ADC_GetValue(&hadc2);
        voltage4 = (val4 * 3.3f) / 4095.0f;
    }
    HAL_ADC_Stop(&hadc2);

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

    static uint32_t tick = 0;
    if (tick == 0) tick = osKernelGetTickCount();
    tick += 100;
    osDelayUntil(tick);
  }
}
