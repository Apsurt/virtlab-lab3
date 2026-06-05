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

  // Show '1234' on the LCD display
  lcdWriteDigit('1', 0);
  lcdWriteDigit('2', 1);
  lcdWriteDigit('3', 2);
  lcdWriteDigit('4', 3);
  lcdUpdateDisplay();

  LOG("LCD initialized with '1234'");

  /* Infinite loop */
  for( ; ; ) {
    osDelay( 1 );
  }
}

