/*
 * application.c
 *
 *  Created on: June 5, 2026
 *      Author: tymonb
 */

#include "main.h"
#include "cmsis_os.h"
#include "io.h"

extern osMessageQueueId_t acquisitionQueueHandle;
extern osMessageQueueId_t displayQueueHandle;
extern osMessageQueueId_t heaterQueueHandle;

void StartDefaultTask( void *argument ) {
    uint32_t tick = osKernelGetTickCount();
    const uint32_t frequency = 100;
    uint16_t sensorValue = 0;

    while( 1 ) {
        if (osMessageQueueGet(acquisitionQueueHandle, &sensorValue, NULL, 0) == osOK) {
            osMessageQueuePut(displayQueueHandle, &sensorValue, 0, 0);
            osMessageQueuePut(heaterQueueHandle, &sensorValue, 0, 0);
        }

        tick += frequency;
        osDelayUntil(tick);
    }
}
