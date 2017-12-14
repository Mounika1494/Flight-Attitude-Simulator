/*
 * eeprom.c
 *
 *  Created on: Dec 10, 2017
 *      Author: Mounika Reddy
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/eeprom.h"
#include "utils/uartstdio.h"
#include "console.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "system.h"
#include <string.h>


void EEPROMTask(void *pvParameters)
{
    message_t message;
    message_t *p_message;
    p_message = &message;
    //uint32_t e2size,e2block;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
    struct EEPROM e2prom_write_value;
    struct EEPROM e2prom_read_value;
    EEPROMInit();
    //e2size = EEPROMSizeGet(); // Get EEPROM Size
    //UARTprintf("EEPROM Size %d bytes\n", e2size);

   // e2block = EEPROMBlockCountGet(); // Get EEPROM Block Count
    //UARTprintf("EEPROM Blok Count: %d\n", e2block);
    strcpy(p_message->data.loggerData,"L TIVA DEBUG INFO: EEPROM initialised\n");
    if(xQueueSend( Logger_Queue, ( void * ) &p_message, ( TickType_t ) 0 ) != pdTRUE){
              UARTprintf("Error\n");
          }
    char buffer[10];
    char recv_buffer[10];
    while(1)
    {
        if(xQueueReceive(EEPROM_Queue, buffer, portMAX_DELAY ) == pdTRUE)
                {
                UARTprintf("Recieved EEPROM %s\n",buffer);
                strcpy(e2prom_write_value.value4,buffer);
                EEPROMProgram((uint32_t *)&e2prom_write_value, E2PROM_TEST_ADRES, sizeof(e2prom_write_value)); //Write struct to EEPROM start from 0x0000
                //EEPROMRead((uint32_t *)&e2prom_read_value, E2PROM_TEST_ADRES, sizeof(e2prom_read_value));
                UARTprintf("Read from EEPROM %s\n",e2prom_read_value.value4);
                }

    }
}



