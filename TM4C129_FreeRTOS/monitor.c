/*
 * monitor.c
 *
 *  Created on: Dec 12, 2017
 *      Author: Mounika Reddy
 */
#include <stdbool.h>
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "driverlib/eeprom.h"
#include "system.h"
#include "console.h"
#include <string.h>


void monitorTask(void *pvParameters)
{
    uint32_t ulNotifiedValue;
    struct EEPROM e2prom_write_value;
    struct EEPROM e2prom_read_value;

    while(1)
    {
        xTaskNotifyWait( 0x00, /* Don't clear any notification bits on entry. */
                         0x0F, /* Reset the notification value to 0 on exit. */
                         &ulNotifiedValue, /* Notified value pass out in
                                             ulNotifiedValue. */
                         portMAX_DELAY );  /* Block indefinitely. */
        if ( ulNotifiedValue  == 0x01)
                {
                    UARTprintf("Logger thread alive\n");
                    system_status[0] = 1;
                }
        if( ulNotifiedValue == 0x02)
                 {
                     UARTprintf("Client thread alive\n");
                     system_status[1] = 1;
                 }
        if( ulNotifiedValue == 0x03 )
                  {
                      UARTprintf("IMU thread alive\n");
                      system_status[2] = 1;
                  }
        if( ulNotifiedValue == 0x04 )
                {
                 strcpy(e2prom_write_value.value4,"FATAL");
                 EEPROMProgram((uint32_t *)&e2prom_write_value, E2PROM_TEST_ADRES, sizeof(e2prom_write_value));
                 EEPROMRead((uint32_t *)&e2prom_read_value, E2PROM_TEST_ADRES, sizeof(e2prom_read_value));
                 UARTprintf("Read from EEPROM %s\n",e2prom_read_value.value4);
                }
    }
}


