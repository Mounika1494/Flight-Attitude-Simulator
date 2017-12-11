#include "system.h"

void *processThread(void *threadp){
       
       printf("processor thread\n");
       message_t sensor_recv;
       int nbytes, prio;
       
       while(1){
              
              fprintf(stderr,"\n waiting on process_mq \n");
              if((nbytes = mq_receive(proc_mq,(char*)&sensor_recv, MAX_MSG_SIZE, &prio)) == ERROR)
              {
                     perror("mq_receive");
              }
              else{
                     printf("x_ddot : %f\n", sensor_recv.data.IMUdata.x_ddot);
                     
                     
                     
                     
                     
                     
                     
              }
              
       }
       
}