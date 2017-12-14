/*
 * client.c
 *
 *  Created on: Dec 4, 2017
 *      Author: Mounika Reddy
 */
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "system.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "inc/hw_ints.h"
#include "semphr.h"
#include "console.h"
#include "uart.h"


char buffer[100];
SemaphoreHandle_t client_mutex;

// Write text over the Stellaris debug interface UART port
void clientTask(void *pvParameters)
{
    message_t message;
    message_t *p_message;
    p_message = &message;
    client_mutex = xSemaphoreCreateMutex();
    if(client_mutex == NULL)
    {
        sprintf(p_message->data.loggerData,"%s\n","L TIVA Client: Mutex creation failed\n");
         if(xQueueSend( Logger_Queue, ( void * ) &p_message, ( TickType_t ) 0 ) != pdTRUE){
                   UARTprintf("Error\n");
               }
    }
    while(1)
    {
    if(xQueueReceive(Socket_Queue,  &p_message, (TickType_t)10000 ) == pdTRUE)
    {
        sprintf(buffer,"D %f %f %f %f %f %f\n ",p_message->data.IMUdata.x_ddot,p_message->data.IMUdata.y_ddot,p_message->data.IMUdata.z_ddot,
                p_message->data.IMUdata.pitch_dot,p_message->data.IMUdata.roll_dot,p_message->data.IMUdata.yaw_dot);
        UARTprintf("%s",buffer);
       if(xSemaphoreTake(client_mutex,portMAX_DELAY) != pdTRUE)
        {
           sprintf(p_message->data.loggerData,"%s\n","L TIVA Client: Semaphore take failed\n");
           if(xQueueSend( Logger_Queue, ( void * ) &p_message, ( TickType_t ) 0 ) != pdTRUE){
                     UARTprintf("Error\n");
                 }
        }
        UARTSendbytes(buffer,strlen(buffer) + 1);
    }
    else
        UARTprintf("Queue Rx ERROR\n");

    if(xTaskNotify( monitorTaskHandle,0x02,eSetValueWithOverwrite) != pdPASS)
    {
        sprintf(p_message->data.loggerData,"%s\n","L TIVA Client: Task Notify failed\n");
         if(xQueueSend( Logger_Queue, ( void * ) &p_message, ( TickType_t ) 0 ) != pdTRUE){
                   UARTprintf("Error\n");
               }
    }
    xSemaphoreGive(client_mutex);
    }
}




