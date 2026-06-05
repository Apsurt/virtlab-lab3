//https://docs.google.com/document/d/1LxXKFGCpF1TWD_6vYKUU3s3WxV-luuKITnVaJ_n7c-U/edit?usp=sharing

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
