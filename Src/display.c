/*
 * display.c
 *
 *  Created on: June 5, 2026
 *      Author: tymonb
 */

#include "main.h"
#include "cmsis_os.h"
#include "io.h"
#include <stdio.h>

extern osMessageQueueId_t displayQueueHandle;

void StartDisplayTask( void *argument ) {
  uint16_t displayValue = 0;
  char buf[5];

  for( ; ; ) {
    if (osMessageQueueGet(displayQueueHandle, &displayValue, NULL, osWaitForever) == osOK) {
        snprintf(buf, sizeof(buf), "%04u", displayValue % 10000);

        lcdWriteDigit(buf[0], 0);
        lcdWriteDigit(buf[1], 1);
        lcdWriteDigit(buf[2], 2);
        lcdWriteDigit(buf[3], 3);
        lcdUpdateDisplay();
    }
  }
}
