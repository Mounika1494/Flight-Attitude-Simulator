/**********************************************************************************************************
* Copyright (C) 2017 by JayaKrishnan HJ
*
*Redistribution,modification or use of this software in source or binary fors is permitted as long 
*as the files maintain this copyright. JayaKrishnan HJ is not liable for any misuse of this material
*
*********************************************************************************************************/
/**
* @file heartbeat.h
* @brief all the structures used by system
*
*This header file provides the mutex and conditional variable declarations for monitoring the tasks
*
* @author Mounika Reddy Edula
* @       JayaKrishnan HJ
* @date December 11 2017
* @version 1.0
*
*/


#ifndef _HEARTBEAT_H_
#define _HEARTBEAT_H_

#define NUM_OF_TASKS 4


int heartBeatIndex[NUM_OF_TASKS];

enum{
       TRANSPORT =0,
       PROCESS,
       LED,
       LOGGER
};



//mutex
pthread_mutex_t transportMutex;
//condition var
pthread_cond_t transportCond;

//mutex
pthread_mutex_t processMutex;
//condition var
pthread_cond_t processCond;

//mutex
pthread_mutex_t loggerMutex;
//condition var
pthread_cond_t loggerCond;

//mutex
pthread_mutex_t ledMutex;
//condition var
pthread_cond_t ledCond;


#endif