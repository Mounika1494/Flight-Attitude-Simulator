/**********************************************************************************************************
* Copyright (C) 2017 by Mounika Reddy Edula Jayakrishnan.H.J
*
*Redistribution,modification or use of this software in source or binary format is permitted as long
*as the files maintain this copyright.Mounika Reddy Edula is not liable for any misuse of this material
*
*********************************************************************************************************/
/**
* @file i2c.h
* @brief I2C service function to talk to driver
*
*This header file provides functions to initialise i2c,write one byte, write n bytes,read one byte,read two byte
*
* @author Mounika Reddy Edula
* @       JayaKrishnan H.J
* @date September 12/11/2017
* @version 1.0
*
*/

#ifndef I2C_H_
#define I2C_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "semphr.h"

/***
* @brief Writing n bytes of data to the given address
*
*
*@param device_address - (uint8_t) slave address of the device intended to communicate
*       device_register - (uint8_t) Address of the Register
*       data - pointer to the data you want to write
*       length - number of bytes you want to write
*
*@return void
*/
void I2C_WriteBytes(uint8_t device_address, uint8_t device_register, uint8_t *data, uint8_t lenght);
/***
* @brief Writing one byte of data to the given address
*
*
*@param device_address - (uint8_t) slave address of the device intended to communicate
*       device_register - (uint8_t) Address of the Register
*       data - (uint8_t) data you want to write
*
*@return void
*/
void I2C_WriteByte(uint8_t slave_address, uint8_t slave_register, uint8_t data);
/***
* @brief Reading n bytes of data to the given address
*
*
*@param device_address - (uint8_t) slave address of the device intended to communicate
*       device_register - (uint8_t) Address of the Register
*       data - pointer to the data you want to store
*       length - number of bytes you want to read
*
*@return void
*/
void I2C_ReadBytes(uint8_t device_address, uint8_t device_register, uint8_t *data, uint8_t lenght);

/***
* @brief Reading one byte of data to the given address
*
*
*@param device_address - (uint8_t) slave address of the device intended to communicate
*       device_register - (uint8_t) Address of the Register
*
*
*@return uint8_t returns the data in the register
*/
uint8_t I2C_ReadByte(uint8_t device_address, uint8_t device_register);
/***
* @brief Initialises the i2c driver
* like enabling clock for I2C,GPIO for a slave address
*
*@param void
*@return void
*/
void I2C_Init(void);

#endif /* I2C_H_ */
