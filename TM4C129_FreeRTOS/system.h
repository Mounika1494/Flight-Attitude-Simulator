/**********************************************************************************************************
* Copyright (C) 2017 by Mounika Reddy Edula Jayakrishnan.H.J
*
*Redistribution,modification or use of this software in source or binary format is permitted as long
*as the files maintain this copyright.Mounika Reddy Edula is not liable for any misuse of this material
*
*********************************************************************************************************/
/**
* @file console.h
* @brief to create all tasks,Queues,Message API
*
*This header file provides create all tasks,Queues,Message API structures
*
* @author Mounika Reddy Edula
* @       JayaKrishnan H.J
* @date September 12/11/2017
* @version 1.0
*
*/
#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "queue.h"
void sequencerTask(void *pvParameters);
void monitorTask(void *pvParameters);
void IMUTask(void *pvParameters);
void EEPROMTask(void *pvParameters);
void clientTask(void *pvParameters);
void loggerTask(void *pvParameters);
#define MAX_SIZE 100
typedef struct
{
    float x_ddot;
    float y_ddot;
    float z_ddot;
    float pitch_dot;
    float roll_dot;
    float yaw_dot;
}IMUdata_t;

typedef union{

    IMUdata_t IMUdata;
    char loggerData[MAX_SIZE];
}data_t;

typedef enum
{
    BAD = 1,
    GOOD
}status_t;

typedef enum
{
    FATAL = 1,
    NON_FATAL
}state_t;

typedef struct
{
    status_t status;
    state_t  state;
    char timestamp[40];
    data_t data;
}message_t;

char EEPROM_data[10];
QueueHandle_t IMU_Queue;
QueueHandle_t Socket_Queue;
QueueHandle_t Logger_Queue;
QueueHandle_t EEPROM_Queue;

extern TaskHandle_t EEPROMTaskHandle;
extern TaskHandle_t IMUTaskHandle;
extern TaskHandle_t sequencerTaskHandle;
extern TaskHandle_t loggerTaskHandle;
extern TaskHandle_t clientTaskHandle;
extern TaskHandle_t monitorTaskHandle;

#define E2PROM_TEST_ADRES 0x0000
struct EEPROM
{
    uint8_t value1;
    uint8_t value2;
    uint16_t value3;
    char value4[12];
};

uint8_t flag;
volatile int system_status[3];
#endif /* SYSTEM_H_ */
