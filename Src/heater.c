/*
 * heater.c
 *
 *  Created on: June 5, 2026
 *      Author: tymonb
 */

#include "main.h"
#include "cmsis_os.h"
#include "io.h"

extern osMessageQueueId_t heaterQueueHandle;

void StartHeaterTask(void *argument) {
    uint16_t temp;
    for(;;) {
        // Wait indefinitely for a new temperature reading
        if (osMessageQueueGet(heaterQueueHandle, &temp, NULL, osWaitForever) == osOK) {
            
            // Check switch 0 to see if the queue value is Celsius or Kelvin
            // Target is 50°C -> 3231 for Kelvin (50 + 273.15)*10, 500 for Celsius (50 * 10)
            uint16_t target = getSwitch0() ? 3231 : 500;
            
            if (temp < target) {
                // Temperature strictly less than target -> Turn ON (Set HIGH)
                HAL_GPIO_WritePin(IO10_GPIO_GPIO_Port, IO10_GPIO_Pin, GPIO_PIN_SET);
            } else {
                // Temperature equal or greater than target -> Turn OFF (Set LOW)
                HAL_GPIO_WritePin(IO10_GPIO_GPIO_Port, IO10_GPIO_Pin, GPIO_PIN_RESET);
            }
        }
    }
}
