/*
 * application.c
 *
 *  Created on: May 18, 2026
 *      Author: max
 */

#include "main.h"
#include "cmsis_os.h"
#include "io.h"

extern osMessageQueueId_t acquisitionQueueHandle;
extern osMessageQueueId_t displayQueueHandle;

void StartDefaultTask( void *argument ) {
    uint32_t tick = osKernelGetTickCount();
    const uint32_t frequency = 100;
    uint16_t sensorValue = 0;

    while( 1 ) {
        if (osMessageQueueGet(acquisitionQueueHandle, &sensorValue, NULL, 0) == osOK) {
            osMessageQueuePut(displayQueueHandle, &sensorValue, 0, 0);
        }

        tick += frequency;
        osDelayUntil(tick);
    }
}
