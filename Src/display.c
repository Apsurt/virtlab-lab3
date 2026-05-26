/*
 * acquisition.c
 *
 *  Created on: May 18, 2026
 *      Author: max
 */

#include "main.h"
#include "cmsis_os.h"

// Declaration of input queue, defined in main.c

extern osMessageQueueId_t displayQueueHandle;

void StartDisplayTask( void *argument ) {
  /* Infinite loop */
  for( ; ; ) {
    osDelay( 1 );
  }
}

