#include "system.h"


void *logThread(void *threadp){
       
       
       printf("logger thread\n");
       message_t sensor_recv;
       int nbytes, prio;
       
       while(1){
              
              printf("\n waiting on log_mq \n");
              if((nbytes = mq_receive(log_mq,(char*)&sensor_recv, MAX_MSG_SIZE, &prio)) == ERROR)
              {
                     perror("mq_receive");
              }
              else{
                     
                     
                     printf("%s\n", sensor_recv.data.logger_data);
                     
                     
                     
                     
                     
              }
              
       }
       
       
       
}