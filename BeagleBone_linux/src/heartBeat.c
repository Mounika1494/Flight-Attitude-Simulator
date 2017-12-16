#include "system.h"
#include "heartbeat.h"
#include <time.h>
#include <asm/errno.h>



//blink led with frequency given
uint8_t blink_led(uint16_t period){
	FILE* LED = NULL;
	if(period == 0)
	return -1;
	const char *LEDBright = "/sys/class/leds/beaglebone:green:usr0/brightness";
    int i =0;
    for(i=0;i<2;i++){
	if((LED = fopen(LEDBright,"r+")) !=NULL){
		fwrite("1",sizeof(char),1,LED);
		fclose(LED);
	}
	else
	return -1;
 	usleep(period);
 	if((LED = fopen(LEDBright,"r+")) != NULL){
 		fwrite("0",sizeof(char),1,LED);
 		fclose(LED);
 		}
 	else
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
    
       while(1){
           
           
           
            pthread_mutex_lock(&transportMutex);
            
            gettimeofday(&now,NULL);
            timeToWait.tv_sec = now.tv_sec+2;
            // timeToWait.tv_nsec = (now.tv_usec+1000UL*timeInMs)*1000UL;
            
            while( heartBeatIndex[TRANSPORT] != 0){
                rc = pthread_cond_timedwait(&transportCond, &transportMutex, &timeToWait); // We wait until globalvar is 1
                if(rc == ETIMEDOUT){
                    printf("******************transport Heartbeat timed out\n\n");
                    break;
                }else{
                    printf("transport heart beat received******************\n");
                    blink_led(1000);
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
                    break;
                }else{
                    printf("process heart beat received******************\n");
                    blink_led(1000);
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
                    break;
                }else{
                    printf("led heart beat received******************\n");
                    blink_led(1000);
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