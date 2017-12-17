/**************************************************************************************
*@Filename:uart.c
*
*@Description: Implementation of uart driver communication with BBG using UART3
*
*@Author:Mounika Reddy Edula
*        JayaKrishnan H.J
*@Date:12/11/2017
*@compiler:gcc
*@debugger:gdb
**************************************************************************************/
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "inc/hw_ints.h"
#include "console.h"
#include "uart.h"
#include "system.h"
#include <string.h>

uint32_t g_ui32SysClock;
SemaphoreHandle_t uartrxirq_mutex;
SemaphoreHandle_t uarttx_mutex;

// Send a string to the UART.
void UARTSendbytes(char *pui8Buffer, uint32_t ui32Count)
{
    //int j = 0;
    //
    // Loop while there are more characters to send.
    //
    if(xSemaphoreTake(uarttx_mutex,portMAX_DELAY) != pdTRUE)
             {
                 //error
             }
    while(ui32Count--)
    {
        //
        // Write the next character to the UART.
        //
        ROM_UARTCharPut(UART3_BASE, *pui8Buffer++);
        //for(j = 0;j<1000;j++);
        //SysCtlDelay(g_ui32SysClock / (1000 * 3));
    }
    xSemaphoreGive(uarttx_mutex);
}

//Initialise uart
void uart_init()
{
    //
    // Set the clocking to run directly from the crystal at 120MHz.
    //
    g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                             SYSCTL_OSC_MAIN |
                                             SYSCTL_USE_PLL |
                                             SYSCTL_CFG_VCO_480), 120000000);
    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    //
    // Enable the GPIO pins for the LED (PN0).
    //
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);

    //
    // Enable the peripherals used by this example.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable processor interrupts.
    //
    ROM_IntMasterEnable();

    GPIOPinConfigure(GPIO_PA4_U3RX);
    GPIOPinConfigure(GPIO_PA5_U3TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    //
    // Configure the UART for 115,200, 8-N-1 operation.
    //
    ROM_UARTConfigSetExpClk(UART3_BASE, g_ui32SysClock, 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
    //
    // Enable the UART interrupt.
    //

    ROM_IntEnable(INT_UART3);
    ROM_UARTIntEnable(UART3_BASE, UART_INT_RX | UART_INT_RT);

    uartrxirq_mutex = xSemaphoreCreateMutex();
    if(uartrxirq_mutex != NULL)
    {

    }
    uarttx_mutex = xSemaphoreCreateMutex();
    if(uarttx_mutex != NULL)
    {

    }

    //
    // Prompt for text to be entered.
    //
    UARTprintf("Uart initialised\n");
}

//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
void
UARTIntHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;
    char rx_str[10];
    int i = 0;
    if(xSemaphoreTake(uartrxirq_mutex,portMAX_DELAY) != pdTRUE)
         {
             //error
         }
    uint32_t ui32Status;
   // uint32_t a;
    //
    // Get the interrrupt status.
    //
    ui32Status = ROM_UARTIntStatus(UART3_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    ROM_UARTIntClear(UART3_BASE, ui32Status);

    //
    // Loop while there are characters in the receive FIFO.
    //
    while(ROM_UARTCharsAvail(UART3_BASE))
    {
        rx_str[i] = ROM_UARTCharGetNonBlocking(UART3_BASE);

        if(rx_str[i] == '\n')
        {
            i++;
            rx_str[i] = '\0';
            break;
        }
        i++;

        //
        // Blink the LED to show a character transfer is occuring.
        //
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);

        //
        // Delay for 1 millisecond.  Each SysCtlDelay is about 3 clocks.
        //
        SysCtlDelay(g_ui32SysClock / (1000 * 3));

        //
        // Turn off the LED
        //
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
    }
    UARTprintf("Recieved %s\n",rx_str);
    if(rx_str[0] == 'F')
    {
    xTaskNotifyFromISR( monitorTaskHandle,
                            0x04,
                            eSetValueWithoutOverwrite,
                            &xHigherPriorityTaskWoken );
    }
    if(rx_str[0] == 'N')
    {
        xTaskNotifyFromISR( monitorTaskHandle,
                              0x05,
                              eSetValueWithoutOverwrite,
                              &xHigherPriorityTaskWoken );

    }
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    xSemaphoreGive(uartrxirq_mutex);
}



