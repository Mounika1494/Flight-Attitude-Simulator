/*
 * logger.c
 *
 *  Created on: Dec 10, 2017
 *      Author: Mounika Reddy
 */
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "console.h"
#include "system.h"
#include "uart.h"
#include <string.h>
#include <stdio.h>

SemaphoreHandle_t logger_mutex;
void loggerTask(void *pvParameters)
{
    message_t message;
    message_t *p_message;
    p_message = &message;
    char buffer[120];
    logger_mutex = xSemaphoreCreateMutex();
    if(logger_mutex == NULL)
    {
        sprintf(p_message->data.loggerData,"%s\n","L TIVA Logger:Mutex creation failed\n");
        //strcpy(p_message->data.loggerData,"FATAL\n");
        if(xQueueSend( Logger_Queue, ( void * ) &p_message, ( TickType_t ) 0 ) != pdTRUE){
                  UARTprintf("Error\n");
              }
    }
while(1)
{
   if(xQueueReceive(Logger_Queue,  &p_message, portMAX_DELAY ) == pdTRUE)
        {
        strcpy(buffer,p_message->data.loggerData);
        UARTprintf("%s",buffer);
        if(xSemaphoreTake(logger_mutex,portMAX_DELAY) != pdTRUE)
             {
                sprintf(p_message->data.loggerData,"%s\n","L TIVA Logger: Semaphore take failed\n");
                //strcpy(p_message->data.loggerData,"FATAL\n");
                if(xQueueSend( Logger_Queue, ( void * ) &p_message, ( TickType_t ) 0 ) != pdTRUE){
                          UARTprintf("Error\n");
                      }
             }
        UARTSendbytes(buffer,strlen(buffer) + 1);
        if(xTaskNotify(monitorTaskHandle,0x01,eSetValueWithOverwrite) != pdPASS)
        {
            sprintf(p_message->data.loggerData,"%s\n","L TIVA Logger: Task notify failed\n");
             //strcpy(p_message->data.loggerData,"FATAL\n");
             if(xQueueSend( Logger_Queue, ( void * ) &p_message, ( TickType_t ) 0 ) != pdTRUE){
                       UARTprintf("Error\n");
                   }
        }
        xSemaphoreGive(logger_mutex);
        }
    }
}

