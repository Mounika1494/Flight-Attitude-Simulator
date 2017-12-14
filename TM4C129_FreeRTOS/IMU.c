/**************************************************************************************
*@Filename:IMU.c
*
*@Description: Implementation of IMU thread which iniitialises I2C and configures
*              all registers for making IMU work
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

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "system.h"
#include <string.h>
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

//Mutex to prevent preemption
SemaphoreHandle_t IMU_mutex;
void IMUTask(void *pvParameters)
{

    message_t message;
    message_t *p_message;
    p_message = &message;
    I2C_Init();//I2C initalisation
    IMU_mutex = xSemaphoreCreateMutex();
    if(IMU_mutex == NULL)
     {
         sprintf(p_message->data.loggerData,"%s\n","L TIVA IMU: Mutex creation failed\n");
           if(xQueueSend( Logger_Queue, ( void * ) &p_message, ( TickType_t ) 0 ) != pdTRUE){
                     UARTprintf("Error\n");
                 }
     }
     uint8_t id;
     //Test to check I2C working
     id = MPU_WhoAmI();
     if(id == 0x71){
         sprintf(p_message->data.loggerData,"%s\n","L TIVA IMU: I2C initialised\n");
             if(xQueueSend( Logger_Queue, ( void * ) &p_message, ( TickType_t ) 0 ) != pdTRUE){
                       UARTprintf("Error\n");
                   }
     }

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

     /*Sample rate = System frequency / desired sample frequency*/
     uint32_t Sample_Rate = MAP_SysCtlClockGet()/100;
     sprintf(p_message->data.loggerData,"%s\n","L TIVA DEBUG INFO:IMU initialised");
     if(xQueueSend( Logger_Queue, ( void * ) &p_message, ( TickType_t ) 0 ) != pdTRUE){
               UARTprintf("Error\n");
           }

     while(1)
     {
         if(xSemaphoreTake(IMU_mutex,portMAX_DELAY) != pdTRUE)
           {
             sprintf(p_message->data.loggerData,"%s\n","L TIVA IMU: Taking Mutex  failed\n");
               if(xQueueSend( Logger_Queue, ( void * ) &p_message, ( TickType_t ) 0 ) != pdTRUE){
                         UARTprintf("Error\n");
                     }
           }
         /*Get data from Gyroscope and Accelerometer*/
         MPU9250_Motion(Acceleration, Degrees);
         MPU9250_Temperature(&Temperature);

         /*Readings minus bias*/
         Acceleration[0] -= acceleration_bias[0];
         Acceleration[1] -= acceleration_bias[1];

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


         if(xQueueSend( Socket_Queue, ( void * ) &p_message, ( TickType_t ) 0 ) != pdTRUE){
                  UARTprintf("Error\n");
              }
         xSemaphoreGive(IMU_mutex);

         /*delay 10 ms*/
         if(xTaskNotify( monitorTaskHandle,0x03,eSetValueWithOverwrite) != pdPASS)
         {
            sprintf(p_message->data.loggerData,"%s\n","L TIVA IMU: Task notify failed\n");
            if(xQueueSend( Logger_Queue, ( void * ) &p_message, ( TickType_t ) 0 ) != pdTRUE){
                     UARTprintf("Error\n");
                 }
         }
         MAP_SysCtlDelay(Sample_Rate);
     }
}


