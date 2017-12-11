#include "mpu9250.h"
#include "console.h"
#include "alarm.h"

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

#include "uart.h"

#define M_PI    3.14159265359
//uint32_t g_ui32SysClock;



int main(void)
{
    //SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                                 SYSCTL_OSC_MAIN |
                                                 SYSCTL_USE_PLL |
                                                 SYSCTL_CFG_VCO_480), 120000000);
    InitConsole();
    uart_init();
    Socket_Queue = xQueueCreate(1000,sizeof(message_t *));
    Logger_Queue = xQueueCreate(1000,sizeof(message_t *));
    EEPROM_Queue = xQueueCreate(1000,sizeof(EEPROM_data));

    xTaskCreate(loggerTask, (const portCHAR *)"Logger",
                    configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(EEPROMTask, (const portCHAR *)"EEPROM",
                    configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(clientTask, (const portCHAR *)"Serial",
                    configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(IMUTask, (const portCHAR *)"IMU",
                    configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler();
    return 0;

//    //
//    // Read the next character from the UART and write it back to the UART.
//    //
//    ROM_UARTCharPutNonBlocking(UART3_BASE,'w');
//    ROM_UARTCharPut(UART3_BASE,'a');
//    ROM_UARTCharPutNonBlocking(UART3_BASE,'o');
//    ROM_UARTCharPutNonBlocking(UART3_BASE,'r');

    //
    // Loop forever echoing data through the UART.
    //
 //   while(1)
   // {

    //}
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
