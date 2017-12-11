#include "system.h"

void *ledThread(void *threadp){
       
       printf("led thread\n");
       message_t sensor_recv;
       int nbytes, prio;
       
       while(1){
              
              printf("\n waiting on led_mq \n");
              if((nbytes = mq_receive(led_mq,(char*)&sensor_recv, MAX_MSG_SIZE, &prio)) == ERROR)
              {
                     perror("mq_receive");
              }
              else{
                     
                     
                     
                     
                     
                     
                     
                     
              }
              
       }
       
       
}