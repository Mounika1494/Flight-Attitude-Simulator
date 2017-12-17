/**************************************************************************************
*@Filename:IMU.c
*
*@Description: Test for IMU see UARTPrintf in the serial terminal and look for atleast one
*              one accelerometer should have g
*
*@Author:Mounika Reddy Edula
*        JayaKrishnan H.J
*@Date:12/11/2017
*@compiler:gcc
*@debugger:gdb
**************************************************************************************/
#include "mpu9250.h"
#include "console.h"
#include <math.h>
#include <stdio.h>
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

#define M_PI    3.14159265359

//int main(void) {
//    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
//    InitConsole();
//    I2C_Init();
//
//    uint8_t id;
//    UARTprintf("IMU and I2C test\n");
//    /*Checking connection here*/
//    id = MPU_WhoAmI();
//    if(id == 0x71){
//        UARTprintf("I2C working\n");
//    }
//    /*MPU9250 Simple Initialization*/
//       MPU_WritePowerManagement1(CLKSEL_1);
//       MPU_WriteConfiguration(DLPF_CFG_3);
//       MPU_WriteSampleRateDivider(4);
//       MPU_WriteGyroConfiguration(GYRO_FS_SELECT_250);
//       MPU_WriteAcceConfiguration(ACCE_FS_SELECT_2G);
//       MPU_WriteAcceConfiguration2(A_DLPF_CFG_3);
//
//       float acceleration_bias[3];
//       float degree_bias[3];
//
//       /*Calibrate MPU9250 and return bias values*/
//       MPU9250_calibrate(acceleration_bias, degree_bias);
//
//       float Acceleration[3];
//       float Degrees[3];
//       float Temperature;
//
//       float Pitch = 0;
//       float Roll = 0;
//       float Yaw = 0;
//       float Acceleration_Pitch = 0;
//       float Acceleration_Roll = 0;
//       //float Acceleration_Yaw = 0; /*it will not work magnetometer necessary!*/
//
//       char Pitch_String[16];
//       char Roll_String[16];
//
//       /*Sample rate = System frequency / desired sample frequency*/
//       uint32_t Sample_Rate = MAP_SysCtlClockGet()/100;
//
//           /*Get data from Gyroscope and Accelerometer*/
//           MPU9250_Motion(Acceleration, Degrees);
//           MPU9250_Temperature(&Temperature);
//
//           /*Readings minus bias*/
//           Acceleration[0] -= acceleration_bias[0];
//           Acceleration[1] -= acceleration_bias[1];
//           //Acceleration[2] -= acceleration_bias[2];
//
//           Degrees[0] -= degree_bias[0];
//           Degrees[1] -= degree_bias[1];
//           Degrees[2] -= degree_bias[2];
//
//           Pitch += Degrees[0] * .01;
//           Roll += Degrees[1] * .01;
//           Yaw += Degrees[2] * .01;
//
//           Acceleration_Pitch = atan2f(Acceleration[1], sqrtf(Acceleration[0] * Acceleration[0] + Acceleration[2] * Acceleration[2]) ) * 180 / M_PI;
//           Acceleration_Roll = atan2f(Acceleration[0], sqrt(Acceleration[1] * Acceleration[1] + Acceleration[2] * Acceleration[2]) ) * 180 / M_PI;
//           //Acceleration_Yaw = atan2f(Acceleration[2], sqrt(Acceleration[0] * Acceleration[0] + Acceleration[1] * Acceleration[1]) ) * 180 / M_PI;
//
//           Pitch = 0.94 * Pitch + 0.06 * Acceleration_Pitch;
//           Roll = 0.94 * Roll + 0.06 * Acceleration_Roll;
//           //Yaw = 0.98 * Yaw + 0.02 * Acceleration_Yaw;
//
//           sprintf(Pitch_String, "%f", Pitch);
//           sprintf(Roll_String, "%f", Roll);
//
//           UARTprintf("%s", Pitch_String);
//           UARTprintf(" %s\n", Roll_String);
//
//           /*delay 10 ms*/
//           MAP_SysCtlDelay(Sample_Rate);
//   }
