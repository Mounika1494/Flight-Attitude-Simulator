#include "mpu9250.h"
#include "console.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "system.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// TivaWare includes
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

#include "driverlib/eeprom.h"


#include "uart.h"

#define M_PI    3.14159265359


TaskHandle_t EEPROMTaskHandle;
TaskHandle_t IMUTaskHandle;
TaskHandle_t sequencerTaskHandle;
TaskHandle_t loggerTaskHandle;
TaskHandle_t clientTaskHandle;
TaskHandle_t monitorTaskHandle;


int main(void)
{
    message_t message;
    message_t *p_message;
    p_message = &message;
    BaseType_t xReturned;
    MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                                 SYSCTL_OSC_MAIN |
                                                 SYSCTL_USE_PLL |
                                                 SYSCTL_CFG_VCO_480), 120000000);
    system_status[0] = -1;
    system_status[1] = -1;
    system_status[2] = -1;
    InitConsole();
    uart_init();
    Socket_Queue = xQueueCreate(1000,sizeof(message_t *));
    if(Socket_Queue == NULL)
        flag = 1;
    Logger_Queue = xQueueCreate(1000,sizeof(message_t *));
    if(Logger_Queue == NULL)
        flag = 1;
    EEPROM_Queue = xQueueCreate(1000,sizeof(EEPROM_data));
    if(EEPROM_Queue == NULL)
        flag = 1;
    xReturned = xTaskCreate(monitorTask, (const portCHAR *)"monitor",
                    configMINIMAL_STACK_SIZE, NULL, 1, &monitorTaskHandle);
    if( xReturned == pdFAIL )
    {
        flag = 1;
    }

    xReturned = xTaskCreate(sequencerTask, (const portCHAR *)"timer",
                    configMINIMAL_STACK_SIZE, NULL, 1, &sequencerTaskHandle);
    if( xReturned == pdFAIL )
    {
        flag = 1;
    }

    xReturned = xTaskCreate(loggerTask, (const portCHAR *)"Logger",
                    configMINIMAL_STACK_SIZE, NULL, 1, &loggerTaskHandle);
    if( xReturned == pdFAIL )
    {
        flag = 1;
    }
    xReturned = xTaskCreate(EEPROMTask, (const portCHAR *)"EEPROM",
                    configMINIMAL_STACK_SIZE, NULL, 1, &EEPROMTaskHandle);
    if( xReturned == pdFAIL )
    {

        flag = 1;
    }
    xReturned = xTaskCreate(clientTask, (const portCHAR *)"Serial",
                    configMINIMAL_STACK_SIZE, NULL, 1, &clientTaskHandle);
    if( xReturned == pdFAIL )
    {

        flag = 1;
    }
    xReturned = xTaskCreate(IMUTask, (const portCHAR *)"IMU",
                    configMINIMAL_STACK_SIZE, NULL, 1, &IMUTaskHandle);
    if( xReturned == pdFAIL )
    {
      flag = 1;
    }

    if(flag == 1)
    {
        sprintf(p_message->data.loggerData,"%s\n","L TIVA DEBUG ERROR:Task creation failed\n");
         //strcpy(p_message->data.loggerData,"FATAL\n");
         if(xQueueSend( Logger_Queue, ( void * ) &p_message, ( TickType_t ) 0 ) != pdTRUE){
                   UARTprintf("Error\n");
               }

    }
    vTaskStartScheduler();
    return 0;
}





/*  ASSERT() Error function
 *
 *  failed ASSERTS() from driverlib/debug.h are executed in this function
 */
void __error__(char *pcFilename, uint32_t ui32Line)
{
    // Place a breakpoint here to capture errors until logging routine is finished
    while (1)
    {
    }
}
