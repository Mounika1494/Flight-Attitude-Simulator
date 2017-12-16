/**********************************************************************************************************
* Copyright (C) 2017 by Mounika Reddy Edula Jayakrishnan.H.J
*
*Redistribution,modification or use of this software in source or binary format is permitted as long
*as the files maintain this copyright.Mounika Reddy Edula is not liable for any misuse of this material
*
*********************************************************************************************************/
/**
* @file mpu9250.h
* @brief to calibrate,Accelerometer,Gyroscope,Temperature,Motion registers
*
*
* @author Mounika Reddy Edula
* @       JayaKrishnan H.J
* @date September 12/11/2017
* @version 1.0
*
*/
#ifndef MPU9250_H_
#define MPU9250_H_

#include "mpu.h"

/***
* @brief Calibrate registers
*
*
*@param acceleration_bias - (float) to store acceleration
*       degree_bias - (float) to store degree
*
*@return void
*/
void MPU9250_calibrate(float *acceleration_bias, float *degree_bias);

/***
* @brief accelerometer
*
*@param acceleration - (float) to store acceleration
*
*@return void
*/
void MPU9250_Accelerometer(float *acceleration);

/***
* @brief Gyroscope registers
*
*
*@param degrees - store data read
*
*@return void
*/
void MPU9250_Gyroscope(float *degrees);

/***
* @brief Temperature registers
*
* @param store temperature registers
*
*@return void
*/
void MPU9250_Temperature(float *temperature);

/***
* @brief Motion registers
*
*
*@param acceleration - (float) to store acceleration
*       degree - (float) to store degree
*
*@return void
*/
void MPU9250_Motion(float *acceleration, float *degrees);

#endif /* MPU9250_H_ */
