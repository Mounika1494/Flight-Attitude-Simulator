/*
 * system.h
 *
 *  Created on: Dec 1, 2017
 *      Author: Mounika Reddy
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "queue.h"
void sequencerTask(void *pvParameters);
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

#endif /* SYSTEM_H_ */
