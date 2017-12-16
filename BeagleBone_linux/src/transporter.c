/**********************************************************************************************************
* Copyright (C) 2017 by JayaKrishnan HJ
*
*Redistribution,modification or use of this software in source or binary fors is permitted as long 
*as the files maintain this copyright. JayaKrishnan HJ is not liable for any misuse of this material
*
*********************************************************************************************************/
/**
* @file transporter.c
* @brief thread of receiving data from Tiva
*
*This file acts as the main thread of receiving data from Tiva
*
* @author Mounika Reddy Edula
* @       JayaKrishnan HJ
* @date December 11 2017
* @version 1.0
*
*/

#include "system.h"
#include "communication.h"
#include "transporter.h"
#include "heartbeat.h"
#include <signal.h>

static float Acceleration[3], Degrees[3];
static char str[100];

void int_handler(){
       
       mq_close(led_mq);
       mq_close(send_mq);
       mq_close(proc_mq);
       mq_close(log_mq);
       mq_unlink(SNDRCV_MQ3);
       mq_unlink(SNDRCV_MQ4);
       mq_unlink(SNDRCV_MQ5);
       mq_unlink(SNDRCV_MQ6);
       
       close(fd);
       exit(0);
       
}


void *uartThread(void *threadp){
       
       printf("uart thread\n");
       char *device = "/dev/ttyO1";
       message_t sensor_recv;
       message_t tiva_send;
       int nbytes;
       int prio;
       char *temp;
       int i =0;
       printf("%s\n", str);
       char debug[100];
       // char str[100];
       
       signal(SIGINT, int_handler);
       
       
        fd = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
       if(fd == -1){
       
              perror("ERROR opening file descriptor\n");
       }
       
       configure = (struct termios*)malloc(sizeof(struct termios));
       tty_config(configure, fd);
       
       while(1){
              
              
              // printf("transporter thread \n");
              
              if(tcsetattr(fd, TCSAFLUSH, configure) < 0){
	    
		perror("ERROR in set attr\n");
		
	       }
              read_bytes(fd, str);
              
              if(*str==0 || strlen(str)==1){
                     
                     continue;
              }
              
              // printf("string from tiva: %s\n\n", str);
              strcpy(debug, str);
              temp = strtok(str, " ");
       
              if(strcmp(temp,"D")==0){
                     
                     while (temp != NULL)
                     {
                            //printf ("%s\n",temp);
                            temp = strtok (NULL, " ");
                     
                            if(temp != NULL){
                     
                                   if(i<3){
                                          Acceleration[i] = atof(temp);
                                   }else{
                                          Degrees[i%3] = atof(temp);
                                   }
                                   //printf("float: %f\n", atof(temp));
                            }
                     ++i;
                     }
                     
                     i = 0;
                     
                     sensor_recv.data.IMUdata.x_ddot = Acceleration[0];
                     sensor_recv.data.IMUdata.y_ddot = Acceleration[1];
                     sensor_recv.data.IMUdata.z_ddot = Acceleration[2];
                     
                     sensor_recv.data.IMUdata.pitch_dot = Degrees[0];
                     sensor_recv.data.IMUdata.roll_dot = Degrees[1];
                     sensor_recv.data.IMUdata.yaw_dot = Degrees[2];
                     
                     sensor_recv.state = GOOD;
                     
                     if((nbytes = mq_send(proc_mq, (char *)&sensor_recv, sizeof(sensor_recv), 30)) == ERROR)
                     {
                            printf("%d\n",nbytes);
                            perror("mq_send");
                     }
                     else
                     {
                            // printf("Sending IMU data to process MQ %d bytes: message successfully sent\n", nbytes);
                     }
                     
                     
              }else if(strcmp(temp,"L")==0) {
                     
                     strcpy(sensor_recv.data.logger_data, debug);
                     if((nbytes = mq_send(log_mq, (char *)&sensor_recv, sizeof(sensor_recv), 30)) == ERROR)
                     {
                            perror("mq_send");
                     }
                     else
                     {
                            // printf("Sending IMU data to LOG MQ %d bytes: message successfully sent\n", nbytes);
                     }
                     
                     
              }else{
                     // printf("Error: Unknown data format\n");
                     read_bytes(fd, str);
                     continue;
              }
              
              
              
              // if((nbytes = mq_receive(send_mq,(char*)&tiva_send, MAX_MSG_SIZE, &prio)) == ERROR)
              // {
              //        perror("mq_receive");
              // }
              // else{
              //        printf("Sending to Tiva\n");
              //        if(tiva_send.status == FATAL){
              //               transfer_bytes(fd, "FATAL\n");
	
              //        }else if(tiva_send.status == NON_FATAL){
              //               transfer_bytes(fd, "NON_FATAL\n");
              //        }
              // }
              
              pthread_mutex_lock(&transportMutex);
              heartBeatIndex[TRANSPORT]++;
              pthread_cond_signal(&transportCond); // Signal as now globalvar is 1
              pthread_mutex_unlock(&transportMutex);
              
              
       }
       
       
}