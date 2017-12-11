/**********************************************************************************************************
* Copyright (C) 2017 by Mounika Reddy Edula Jayakrishnan.H.J
*
*Redistribution,modification or use of this software in source or binary format is permitted as long
*as the files maintain this copyright.Mounika Reddy Edula is not liable for any misuse of this material
*
*********************************************************************************************************/
/**
* @file gpio.h
* @brief GPIO driver to initilaise a GPIO pin as interrupt
*
*This header file provides functions to initialise a GPIO pin as interrupt
*
* @author Mounika Reddy Edula
* @       JayaKrishnan H.J
* @date September 12/11/2017
* @version 1.0
*
*/
#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"

//#define MPU9250_INTERRUPT
/***
* @brief Initialise GPIO pins for interrupt and alarm
*
*
*@param void
*
*@return void
*/
void GPIO_Init(void);

/***
* @brief To turn on the GPIO pin
*
*
*@param pins - pin number you want to turn ON
*
*@return void
*/
void GPIO_on(uint8_t pins);

#endif /* GPIO_H_ */
