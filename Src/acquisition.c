/*
 * acquisition.c
 *
 *  Created on: May 18, 2026
 *      Author: max
 */

#include "main.h"
#include "cmsis_os.h"

// Declaration of output queue, defined in main.c

extern osMessageQueueId_t acquisitionQueueHandle;

// Acquisition task entry point

void StartAcquisitionTask( void *argument ) {
  /* Infinite loop */
  for( ; ; ) {
    osDelay( 1 );
  }
}

