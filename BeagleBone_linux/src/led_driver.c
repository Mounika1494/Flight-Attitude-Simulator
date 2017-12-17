/**********************************************************************************************************
* Copyright (C) 2017 by JayaKrishnan HJ
*
*Redistribution,modification or use of this software in source or binary fors is permitted as long 
*as the files maintain this copyright. JayaKrishnan HJ is not liable for any misuse of this material
*
*********************************************************************************************************/
/**
* @file led_driver.c
* @brief drives the actuator LEDs
*
*Thisfile contains the functions to initialize and set the actuator LEDs
*
* @author Mounika Reddy Edula
* @       JayaKrishnan HJ
* @date December 11 2017
* @version 1.0
*
*/

#include "system.h"
#include "heartbeat.h"


//Initialize actuator LEDs
static int8_t LED_init(){
       FILE *fp;
       int nbytes;
       message_t sensor_recv;
       
       if((fp = fopen("/sys/class/gpio/gpio66/direction", "w+"))!=NULL){
               fwrite("out", 1, 4 , fp);
               fclose(fp);
       }else{
               perror("File Open Error:");
               return -1;
       }
       
       
       if((fp = fopen("/sys/class/gpio/gpio67/direction", "w+"))!=NULL){
               fwrite("out", 1, 4 , fp);
               fclose(fp);
       }else{
               perror("File Open Error:");
               return -1;
       }
       
       
       
       if((fp = fopen("/sys/class/gpio/gpio68/direction", "w+"))!=NULL){
               fwrite("out", 1, 4 , fp);
               fclose(fp);
       }else{
               perror("File Open Error:");
               return -1;
       }
       
       
       
       if((fp = fopen("/sys/class/gpio/gpio69/direction", "w+"))!=NULL){
               fwrite("out", 1, 4 , fp);
               fclose(fp);
       }else{
               perror("File Open Error:");
               return -1;
       }
       
       sprintf(sensor_recv.data.logger_data,"%s"," DEUBG INFO: LEDs initialized");
       
       if((nbytes = mq_send(log_mq, (char *)&sensor_recv, sizeof(sensor_recv), 30)) == -1)
       {
              perror("mq_send");
       }
       else
       {
              //printf("Core Dumped\n", nbytes);
       }
       
       
       return 0;
}


//set LED1
int set_LED1(char* val){
       
       FILE *fp;
       
       if((fp = fopen("/sys/class/gpio/gpio68/value", "w+"))!=NULL){
               fwrite(val, 1, 1 , fp);
               fclose(fp);
       }else{
               perror("File Open Error:");
               return -1;
       }
       
}


//set LED2
int set_LED2(char* val){
       
       FILE *fp;
       
       if((fp = fopen("/sys/class/gpio/gpio67/value", "w+"))!=NULL){
               fwrite(val, 1, 1 , fp);
               fclose(fp);
       }else{
               perror("File Open Error:");
               return -1;
       }
       
}


//set LED3
int set_LED3(char* val){
       
       FILE *fp;
       
       if((fp = fopen("/sys/class/gpio/gpio69/value", "w+"))!=NULL){
               fwrite(val, 1, 1 , fp);
               fclose(fp);
       }else{
               perror("File Open Error:");
               return -1;
       }
       
}


//set LED4
int set_LED4(char* val){
       
       FILE *fp;
       
       if((fp = fopen("/sys/class/gpio/gpio66/value", "w+"))!=NULL){
               fwrite(val, 1, 1 , fp);
               fclose(fp);
       }else{
               perror("File Open Error:");
               return -1;
       }
       
}

// This thread blocks on LED message queue and receives the LED data
void *ledThread(void *threadp){
       
       printf("led thread\n");
       message_t sensor_recv;
       int nbytes, prio;
       
       LED_init();
       
       while(1){
              
              // printf("\n waiting on led_mq \n");
              if((nbytes = mq_receive(led_mq,(char*)&sensor_recv, MAX_MSG_SIZE, &prio)) == ERROR)
              {
                     perror("mq_receive");
              }
              else{
                     // printf("\n Received LED_MQ state %d\n", sensor_recv.data.LED.LED1);
                     
                     if(sensor_recv.data.LED.LED1 == GLOW){
                            
                            // printf("LED1 ON\n");
                            set_LED1("1");
                            set_LED2("0");
                            // set_LED3("0");
                            // set_LED4("0");
                     }else if(sensor_recv.data.LED.LED2 == GLOW){
                            // printf("LED2 ON\n");
                            set_LED1("0");
                            set_LED2("1");
                            // set_LED3("0");
                            // set_LED4("0");
                     }
                     
                     if(sensor_recv.data.LED.LED3 == GLOW){
                            // printf("LED3 ON\n");
                            // set_LED1("0");
                            // set_LED2("0");
                            set_LED3("1");
                            set_LED4("0");
                     }else if(sensor_recv.data.LED.LED4 == GLOW){
                            // printf("LED4 ON\n");
                            // set_LED1("0");
                            // set_LED2("0");
                            set_LED3("0");
                            set_LED4("1");
                     }
                     
                     
              }
              
              
              pthread_mutex_lock(&ledMutex);
              heartBeatIndex[LED]++;
              pthread_cond_signal(&ledCond); // Signal as now globalvar is 1
              pthread_mutex_unlock(&ledMutex);
              
              
              
       }
       
       
}