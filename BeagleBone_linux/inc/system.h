/**********************************************************************************************************
* Copyright (C) 2017 by JayaKrishnan HJ
*
*Redistribution,modification or use of this software in source or binary fors is permitted as long 
*as the files maintain this copyright. JayaKrishnan HJ is not liable for any misuse of this material
*
*********************************************************************************************************/
/**
* @file system.h
* @brief all the structures used by system
*
*This header file provides thread declrations and messaging struccture
*
* @author Mounika Reddy Edula
* @       JayaKrishnan HJ
* @date December 11 2017
* @version 1.0
*
*/


#ifndef _SYSTEM_H
#define _SYSTEM_H_

#include <mqueue.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define MAX_SIZE 250
#define NUM_THREADS 5
#define MAX_MSG_SIZE 1024 //maximum messagequeue width
#define ERROR -1

#define SNDRCV_MQ3 "/send_receive_mq3" 
#define SNDRCV_MQ4 "/send_receive_mq4"
#define SNDRCV_MQ5 "/send_receive_mq5"
#define SNDRCV_MQ6 "/send_receive_mq6"

//thread creation
typedef struct
{
  int threadIdx;
} threadParams_t;

pthread_t threads[NUM_THREADS];
threadParams_t threadParams[NUM_THREADS];

// message queue attributes
struct mq_attr mq_attr;

//messgage queue for every thread
mqd_t proc_mq, log_mq;
mqd_t led_mq, send_mq;

char *logger_file;

typedef struct{
       uint8_t LED1;
       uint8_t LED2;
       uint8_t LED3;
       uint8_t LED4;
}LED_t;

typedef struct{
       
       float x_ddot;
       float y_ddot;
       float z_ddot;
       float pitch_dot;
       float roll_dot;
       float yaw_dot;
}IMUdata_t;

typedef union{
       IMUdata_t IMUdata;
       char logger_data[MAX_SIZE];
       LED_t LED;
       
}data_t;

typedef enum{
       GLOW = 1,
       DARK
}actuator_t;

typedef enum{
       BAD = 1,
       GOOD
}state_t;

typedef enum{
       FATAL = 1,
       NON_FATAL
}status_t;

typedef struct{
       
       status_t status;
       state_t state;
       char timestamp[40];
       data_t data;
       
}message_t;


void *logThread(void *threadp);
void *ledThread(void *threadp);
void *processorThread(void *threadp);
void *uartThread(void *threadp);
void *processThread(void *threadp);
void *heartBeatThread(void *threadp);


#endif