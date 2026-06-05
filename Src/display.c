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

// Simple logging macro
#define LOG(fmt, ...) printf("[DISPLAY] " fmt "\r\n", ##__VA_ARGS__)

// Declaration of input queue, defined in main.c

extern osMessageQueueId_t displayQueueHandle;

void StartDisplayTask( void *argument ) {
  LOG("Display task started");

  uint16_t displayValue = 0;
  char buf[5];

  /* Infinite loop */
  for( ; ; ) {
    // Wait for a message from the queue (blocking)
    if (osMessageQueueGet(displayQueueHandle, &displayValue, NULL, osWaitForever) == osOK) {
        LOG("Received value: %u", displayValue);

        // Convert to 4-digit string
        snprintf(buf, sizeof(buf), "%04u", displayValue % 10000);

        // Update LCD
        lcdWriteDigit(buf[0], 0);
        lcdWriteDigit(buf[1], 1);
        lcdWriteDigit(buf[2], 2);
        lcdWriteDigit(buf[3], 3);
        lcdUpdateDisplay();
    }
  }
}

