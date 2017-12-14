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
#include "console.h"
#include  <stdio.h>
#include "drivers/pinout.h"
#include "utils/uartstdio.h"

// TivaWare includes
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

TimerHandle_t xTimer;
int lExpireCounters;
int status_check[3] = {0,0,0};
void vTimerCallback( TimerHandle_t  *pxTimer )
 {
     message_t message;
     message_t *p_message;
     p_message = &message;
     LEDWrite(0x0F, 0x04);

     long lArrayIndex;
     /* Optionally do something if the pxTimer parameter is NULL. */
     configASSERT( pxTimer );

     /* Which timer expired? */
     lArrayIndex = ( long ) pvTimerGetTimerID( pxTimer );
     if(lArrayIndex == 5)
     {
         if(status_check[0] > 10)
          {
            LEDWrite(0x0F, 0x08);
            sprintf(p_message->data.loggerData,"%s\n","L TIVA DEBUG ERROR:Logger thread is dead\n");
            if(xQueueSend( Logger_Queue, ( void * ) &p_message, ( TickType_t ) 0 ) != pdTRUE){
                    UARTprintf("Error\n");
                }
            UARTprintf("Logger thread is killed\n");
          }
          if( status_check[1] >10 )
          {
              LEDWrite(0x0F, 0x08);
              sprintf(p_message->data.loggerData,"%s\n","L TIVA DEBUG ERROR: Client thread is dead\n");
              if(xQueueSend( Logger_Queue, ( void * ) &p_message, ( TickType_t ) 0 ) != pdTRUE){
                         UARTprintf("Error\n");
                     }
              UARTprintf("Client thread is killed\n");
          }
          if(status_check[2] >10)
          {
              LEDWrite(0x0F, 0x08);
              sprintf(p_message->data.loggerData,"%s\n","L TIVA DEBUG ERROR: IMU thread is dead\n");
              if(xQueueSend( Logger_Queue, ( void * ) &p_message, ( TickType_t ) 0 ) != pdTRUE){
                         UARTprintf("Error\n");
                     }
              UARTprintf("IMU task is dead\n");
          }
         if(system_status[0] == -1)
             status_check[0]++;
         if(system_status[1] == -1)
             status_check[1]++;
         if(system_status[2] == -1)
             status_check[2]++;
         if (system_status[0] == 1)
           {
             system_status[0] = -1;
             if(status_check[0] > 1)
             status_check[0]--;
           }
         if (system_status[1] == 1)
           {
             system_status[1] = -1;
             if(status_check[1] > 1)
             status_check[1]--;
           }
         if (system_status[2] == 1)
            {
              system_status[2] = -1;
              if(status_check[2] > 1)
              status_check[2]--;
            }
     }
 }

void sequencerTask(void *pvParameters)
{
    int error_flag = 0;
    message_t message;
    message_t *p_message;
    p_message = &message;
    PinoutSet(false, false);
    xTimer = xTimerCreate(  "Timer",        /* Just a text name, not used by the kernel. */
                            pdMS_TO_TICKS(1000),    /* The timer period in ticks. */
                             pdTRUE,         /* The timers will auto-reload themselves when they expire. */
                             ( void * ) 5,   /* Assign each timer a unique id equal to its array index. */
                              vTimerCallback  /* Each timer calls the same callback when it expires. */
                          );

     if( xTimer == NULL )
     {
         error_flag = 1;
     }
     else
     {
         /* Start the timer.  No block time is specified, and even if one was
         it would be ignored because the scheduler has not yet been
         started. */
         if( xTimerStart( xTimer, 0 ) != pdPASS )
         {
             /* The timer could not be set into the Active state. */
             error_flag = 1;
         }
     }
     if(error_flag == 1)
     {
        sprintf(p_message->data.loggerData,"%s\n","L TIVA DEBUG ERROR:Timer Error\n");
        if(xQueueSend( Logger_Queue, ( void * ) &p_message, ( TickType_t ) 0 ) != pdTRUE){
                 UARTprintf("Error\n");
             }
     }
     while(1);

}



