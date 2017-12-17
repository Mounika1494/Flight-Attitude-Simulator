/**********************************************************************************************************
* Copyright (C) 2017 by JayaKrishnan HJ
*
*Redistribution,modification or use of this software in source or binary fors is permitted as long 
*as the files maintain this copyright. JayaKrishnan HJ is not liable for any misuse of this material
*
*********************************************************************************************************/
/**
* @file heartbeat.c
* @brief all the functions to monitor tasks
*
*This file provides the mutex and conditional variable declarations for monitoring the tasks
*
* @author Mounika Reddy Edula
* @       JayaKrishnan HJ
* @date December 11 2017
* @version 1.0
*
*/

#include "system.h"
#include "heartbeat.h"
#include <time.h>
#include <asm/errno.h>



//blink led with frequency given
// uint8_t blink_led(uint16_t period){
// 	FILE* LED = NULL;
// 	if(period == 0)
// 	return -1;
// 	const char *LEDBright = "/sys/class/leds/beaglebone:green:usr0/brightness";
//     int i =0;
//     for(i=0;i<2;i++){
// 	if((LED = fopen(LEDBright,"r+")) !=NULL){
// 		fwrite("1",sizeof(char),1,LED);
// 		fclose(LED);
// 	}
// 	else
// 	return -1;
//  	usleep(period);
//  	if((LED = fopen(LEDBright,"r+")) != NULL){
//  		fwrite("0",sizeof(char),1,LED);
//  		fclose(LED);
//  		}
//  	else
//  	return -1;
// 	}
// 	return 0;
// }


// "0" : Periodic mode
// "1" : Latch mode
int8_t setMode(char* mode){

    FILE *fp;

    if((fp = fopen("/sys/ebb/gpio53/mode", "w+"))!=NULL){
        fwrite(mode, 1, sizeof(char), fp);
        fclose(fp);
    }else{
        perror("File Open Error:");
        return -1;
    }
    return 0;
}



// Set TIme period of LED flashing.
// Unit milliseconds
// Ensure mode is set to Periodic mode
int8_t blink_led(char* period){

    FILE *fp;

    if((fp = fopen("/sys/ebb/gpio53/period", "w+"))!=NULL){
        fwrite(period, 1, strlen(period)+1 , fp);
        fclose(fp);
    }else{
        perror("File Open Error:");
        return -1;
    }
    return 0;
}



// "1" : Latch LED ON
// "0" : Latch LED OFF
// Ensure mode is set to Latch mode

int8_t setLED(char* state){

    FILE *fp;

    if((fp = fopen("/sys/ebb/gpio53/ledState", "w+"))!=NULL){
        fwrite(state, 1, sizeof(char), fp);
        fclose(fp);
    }else{
        perror("File Open Error:");
        return -1;
    }
    return 0;
}



void *heartBeatThread(void *threadp){
    
    printf("hearbeat thread\n");
    
    struct timespec timeToWait;
    struct timeval now;
    
    int timeInMs = 1;
    
    int rc;
       
    heartBeatIndex[TRANSPORT] = -1;
    heartBeatIndex[PROCESS] = -1;
    heartBeatIndex[LOGGER] = -1;
    heartBeatIndex[LED] = -1;
    
    setMode("0");
    
       while(1){
           
           
           
            pthread_mutex_lock(&transportMutex);
            
            gettimeofday(&now,NULL);
            timeToWait.tv_sec = now.tv_sec+2;
            // timeToWait.tv_nsec = (now.tv_usec+1000UL*timeInMs)*1000UL;
            
            while( heartBeatIndex[TRANSPORT] != 0){
                rc = pthread_cond_timedwait(&transportCond, &transportMutex, &timeToWait); // We wait until globalvar is 1
                if(rc == ETIMEDOUT){
                    printf("******************transport Heartbeat timed out\n\n");
                    setMode("1");
                    setLED("0");
                    break;
                }else{
                    printf("transport heart beat received******************\n");
                     setMode("0");
                    blink_led("1000");
                    break;
                }
            }
                
            heartBeatIndex[TRANSPORT] = -1;
            pthread_mutex_unlock(&transportMutex);
           
            //printf("\n\n****************Transport heart beat received*************\n\n");  
            
            
            
            pthread_mutex_lock(&processMutex);
            
            gettimeofday(&now,NULL);
            timeToWait.tv_sec = now.tv_sec+2;
            // timeToWait.tv_nsec = (now.tv_usec+1000UL*timeInMs)*1000UL;
            
            while( heartBeatIndex[PROCESS] != 0){
                rc = pthread_cond_timedwait(&processCond, &processMutex, &timeToWait); // We wait until globalvar is 1
                if(rc == ETIMEDOUT){
                    printf("*****************process Heartbeat timed out\n\n");
                    setMode("1");
                    setLED("0");
                    break;
                }else{
                    printf("process heart beat received******************\n");
                     setMode("0");
                    blink_led("1000");
                    break;
                }
            }
            heartBeatIndex[PROCESS] = -1;
            pthread_mutex_unlock(&processMutex);
           
            // printf("****************Process heart beat received*************\n\n"); 
            
            
            
            pthread_mutex_lock(&ledMutex);
            
            gettimeofday(&now,NULL);
            timeToWait.tv_sec = now.tv_sec+2;
            // timeToWait.tv_nsec = (now.tv_usec+1000UL*timeInMs)*1000UL;
            
            while( heartBeatIndex[LED] != 0){
                rc = pthread_cond_timedwait(&ledCond, &ledMutex, &timeToWait); // We wait until globalvar is 1
                if(rc == ETIMEDOUT){
                    printf("****************led Heartbeat timed out\n\n");
                    setMode("1");
                    setLED("0");
                    break;
                }else{
                    printf("led heart beat received******************\n");
                    setMode("0");
                    blink_led("1000");
                    break;
                }
                
            }
            heartBeatIndex[LED] = -1;
            pthread_mutex_unlock(&ledMutex);
           
           
           
            // printf("****************LED heart beat received*************\n\n"); 
            
            pthread_mutex_lock(&loggerMutex);
            
            gettimeofday(&now,NULL);
            timeToWait.tv_sec = now.tv_sec+2;
            // timeToWait.tv_nsec = (now.tv_usec+1000UL*timeInMs)*1000UL;
            
            while( heartBeatIndex[LOGGER] != 0){
                rc = pthread_cond_timedwait(&loggerCond, &loggerMutex, &timeToWait); // We wait until globalvar is 1
                if(rc == ETIMEDOUT){
                    printf("********************logger Heartbeat timed out\n\n");
                    break;
                }else{
                    printf("logger heart beat received******************\n");
                    break;
                }
            }
            heartBeatIndex[LOGGER] = -1;
            pthread_mutex_unlock(&loggerMutex);
           
            // printf("****************LOGGER heart beat received*************"); 
           
           
           
           
       }
       
       
       
       
       
}