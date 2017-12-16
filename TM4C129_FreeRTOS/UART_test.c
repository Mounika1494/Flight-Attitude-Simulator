/**************************************************************************************
*@Filename:UART_test.c
*
*@Description: Test for uart see UARTPrintf in the serial terminal and connect in loopback mode
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

//int main()
//{
//    uart_init();
//    UARTSendbytes("w",1);
//}



