/*
 * IMU.c
 *
 *  Created on: Dec 4, 2017
 *      Author: Mounika Reddy
 */
#include "mpu9250.h"
#include "console.h"
#include "alarm.h"
#include <math.h>
#include <stdio.h>

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "system.h"
#include <string.h>
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

SemaphoreHandle_t IMU_mutex;
void IMUTask(void *pvParameters)
{

    message_t message;
    message_t *p_message;
    p_message = &message;
    /*static int i = 0;
    while(1)
    {
     message.data.x_ddot = i;
     message.data.y_ddot = i;
     message.data.z_ddot = i;
     message.status = BAD;
     i++;
     if(xQueueSend( Socket_Queue, ( void * ) &p_message, ( TickType_t ) 0 ) != pdTRUE){
         UARTprintf("Error\n");
     }
    }*/
     I2C_Init();
     IMU_mutex = xSemaphoreCreateMutex();
     uint8_t id;

     /*Checking connection here*/
     id = MPU_WhoAmI();
    /* if(id == 0x71){
         /*We are connected set armed alarm*/
         //LED_ArmedAlarm();
     //}else{
         /*connection failed somehow, set failed alarm. Check connections*/
     //    UARTprintf("Connection failed id found: %x", id);
       //  LED_FailedAlarm();
        // while(1);
     //}*/

     /*MPU9250 Simple Initialization*/
     MPU_WritePowerManagement1(CLKSEL_1);
     MPU_WriteConfiguration(DLPF_CFG_3);
     MPU_WriteSampleRateDivider(4);
     MPU_WriteGyroConfiguration(GYRO_FS_SELECT_250);
     MPU_WriteAcceConfiguration(ACCE_FS_SELECT_2G);
     MPU_WriteAcceConfiguration2(A_DLPF_CFG_3);

     float acceleration_bias[3];
     float degree_bias[3];

     /*Calibrate MPU9250 and return bias values*/
     MPU9250_calibrate(acceleration_bias, degree_bias);

     float Acceleration[3];
     float Degrees[3];
     float Temperature;

     float Pitch = 0;
     float Roll = 0;
     float Yaw = 0;
     float Acceleration_Pitch = 0;
     float Acceleration_Roll = 0;
     //float Acceleration_Yaw = 0; /*it will not work magnetometer necessary!*/

     char Pitch_String[16];
     char Roll_String[16];

     /*Sample rate = System frequency / desired sample frequency*/
     uint32_t Sample_Rate = MAP_SysCtlClockGet()/100;
     sprintf(p_message->data.loggerData,"%s\n","L TIVA DEBUG INFO:IMU initialised");
     //strcpy(p_message->data.loggerData,"FATAL\n");
     if(xQueueSend( Logger_Queue, ( void * ) &p_message, ( TickType_t ) 0 ) != pdTRUE){
               UARTprintf("Error\n");
           }

     while(1)
     {
         if(xSemaphoreTake(IMU_mutex,portMAX_DELAY) != pdTRUE)
           {
               //error
           }
         /*Get data from Gyroscope and Accelerometer*/
         MPU9250_Motion(Acceleration, Degrees);
         MPU9250_Temperature(&Temperature);

         /*Readings minus bias*/
         Acceleration[0] -= acceleration_bias[0];
         Acceleration[1] -= acceleration_bias[1];
         //Acceleration[2] -= acceleration_bias[2];

         Degrees[0] -= degree_bias[0];
         Degrees[1] -= degree_bias[1];
         Degrees[2] -= degree_bias[2];

         Pitch += Degrees[0] * .01;
         Roll += Degrees[1] * .01;
         Yaw += Degrees[2] * .01;
         message.data.IMUdata.x_ddot = Acceleration[0];
         message.data.IMUdata.y_ddot = Acceleration[1];
         message.data.IMUdata.z_ddot = Acceleration[2];
         message.data.IMUdata.pitch_dot = Degrees[0];
         message.data.IMUdata.roll_dot = Degrees[1];
         message.data.IMUdata.yaw_dot = Degrees[2];

         //Acceleration_Pitch = atan2f(Acceleration[1], sqrtf(Acceleration[0] * Acceleration[0] + Acceleration[2] * Acceleration[2]) ) * 180 / M_PI;
         //Acceleration_Roll = atan2f(Acceleration[0], sqrt(Acceleration[1] * Acceleration[1] + Acceleration[2] * Acceleration[2]) ) * 180 / M_PI;
         //Acceleration_Yaw = atan2f(Acceleration[2], sqrt(Acceleration[0] * Acceleration[0] + Acceleration[1] * Acceleration[1]) ) * 180 / M_PI;

         //Pitch = 0.94 * Pitch + 0.06 * Acceleration_Pitch;
         //Roll = 0.94 * Roll + 0.06 * Acceleration_Roll;
         //Yaw = 0.98 * Yaw + 0.02 * Acceleration_Yaw;

         //sprintf(Pitch_String, "%f", Pitch);
         //sprintf(Roll_String, "%f", Roll);

         //UARTprintf("%s", Pitch_String);
         //UARTprintf(" %s\n", Roll_String);

         if(xQueueSend( Socket_Queue, ( void * ) &p_message, ( TickType_t ) 0 ) != pdTRUE){
                  UARTprintf("Error\n");
              }
         xSemaphoreGive(IMU_mutex);
         int i;
         /*delay 10 ms*/
         MAP_SysCtlDelay(Sample_Rate);
         //for(i =0;i<10000;i++);
     }
}


