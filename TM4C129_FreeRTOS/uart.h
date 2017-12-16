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
#ifndef UART_H_
#define UART_H_

/***
* @brief UART send multiple bytes
*
*
*@param pui8Buffer - write buffer
*
*@return ui32Count - send the length
*/
void UARTSendbytes(char *pui8Buffer, uint32_t ui32Count);

/***
* @brief Initialise uart
*
*
*@param void
*
*@return void
*/
void uart_init();


#endif /* UART_H_ */
