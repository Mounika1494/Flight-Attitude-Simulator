/*
 * sequencer.c
 *
 *  Created on: Dec 1, 2017
 *      Author: Mounika Reddy
 */
#include "main.h"

// FreeRTOS includes
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "system.h"

TimerHandle_t xTimer;
int lExpireCounters;
void vTimerCallback( TimerHandle_t  *pxTimer )
 {
     message_t message;
     long lArrayIndex;
     /* Optionally do something if the pxTimer parameter is NULL. */
     configASSERT( pxTimer );

     /* Which timer expired? */
     lArrayIndex = ( long ) pvTimerGetTimerID( pxTimer );
     if(lArrayIndex == 5)
     {
         /* Increment the number of times that pxTimer has expired. */
         lExpireCounters += 1;

         switch(lExpireCounters){
         case 1:
             message.state = FATAL;
             xQueueSend( IMU_Queue, ( void * ) &message, ( TickType_t ) 0 );
         case 2:
             lExpireCounters = 0;
         }
     }
 }

void sequencerTask(void *pvParameters)
{
    xTimer = xTimerCreate(  "Timer",        /* Just a text name, not used by the kernel. */
                            ( 100 * 5 ),    /* The timer period in ticks. */
                             pdTRUE,         /* The timers will auto-reload themselves when they expire. */
                             ( void * ) 5,   /* Assign each timer a unique id equal to its array index. */
                              vTimerCallback  /* Each timer calls the same callback when it expires. */
                          );

     if( xTimer == NULL )
     {
         /* The timer was not created. */
     }
     else
     {
         /* Start the timer.  No block time is specified, and even if one was
         it would be ignored because the scheduler has not yet been
         started. */
         if( xTimerStart( xTimer, 0 ) != pdPASS )
         {
             /* The timer could not be set into the Active state. */
         }
     }
     while(1);

}



