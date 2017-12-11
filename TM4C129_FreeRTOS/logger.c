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

SemaphoreHandle_t logger_mutex;
void loggerTask(void *pvParameters)
{
    message_t message;
    message_t *p_message;
    p_message = &message;
    char buffer[120];
    logger_mutex = xSemaphoreCreateMutex();
    if(logger_mutex != NULL)
    {

    }
while(1)
{
    if(xQueueReceive(Logger_Queue,  &p_message, portMAX_DELAY ) == pdTRUE)
        {
        strcpy(buffer,p_message->data.loggerData);
        UARTprintf("%s",buffer);
        if(xSemaphoreTake(logger_mutex,portMAX_DELAY) != pdTRUE)
             {
                 //error
             }
        UARTSendbytes(buffer,strlen(buffer) + 1);
        xSemaphoreGive(logger_mutex);
        }
}
}

