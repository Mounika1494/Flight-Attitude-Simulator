/**********************************************************************************************************
* Copyright (C) 2017 by Mounika Reddy Edula Jayakrishnan.H.J
*
*Redistribution,modification or use of this software in source or binary format is permitted as long
*as the files maintain this copyright.Mounika Reddy Edula is not liable for any misuse of this material
*
*********************************************************************************************************/
/**
* @file console.h
* @brief to initialise the UART0 pins for use of UARTPrintf
*
*This header file provides functions to initialise the UART0 to utilise UARTPrintf
*
* @author Mounika Reddy Edula
* @       JayaKrishnan H.J
* @date September 12/11/2017
* @version 1.0
*
*/
#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "utils/uartstdio.h"

/***
* @brief Initialise the UART0 enabling UART pins and clock
*
*
*@param void
*
*@return void
*/
void InitConsole(void);

#endif /* CONSOLE_H_ */
