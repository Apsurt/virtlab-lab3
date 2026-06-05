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
    int led_state = OFF;
    for(;;) {
        // Wait indefinitely for a new temperature reading
        if (osMessageQueueGet(heaterQueueHandle, &temp, NULL, osWaitForever) == osOK) {
            
            // Check switch 1 for debug mode (30C) or normal mode (50C)
            uint16_t target_temp = getSwitch1() ? 300 : 500;
            
            // Check switch 0 to see if the queue value is Celsius or Kelvin
            // Kelvin target = (target_temp_c + 273.15) * 10
            uint16_t target;
            if (getSwitch0()) {
                target = target_temp == 300 ? 3031 : 3231;
            } else {
                target = target_temp;
            }
            
            if (temp < target) {
                // Temperature strictly less than target -> Turn ON (Set HIGH)
                HAL_GPIO_WritePin(IO10_GPIO_GPIO_Port, IO10_GPIO_Pin, GPIO_PIN_SET);
                
                // Toggle LEDs for blinking effect
                led_state = (led_state == ON) ? OFF : ON;
                led0(led_state);
                led1(led_state);
            } else {
                // Temperature equal or greater than target -> Turn OFF (Set LOW)
                HAL_GPIO_WritePin(IO10_GPIO_GPIO_Port, IO10_GPIO_Pin, GPIO_PIN_RESET);
                
                // Ensure LEDs are OFF when heater is OFF
                led_state = OFF;
                led0(OFF);
                led1(OFF);
            }
        }
    }
}
