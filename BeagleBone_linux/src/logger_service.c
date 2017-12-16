#include "system.h"
#include "heartbeat.h"


void *logThread(void *threadp){
       
       
       printf("logger thread\n");
       message_t sensor_recv;
       int nbytes, prio;
       FILE *fp_logger;
       fp_logger = fopen(logger_file, "a");
       if(fp_logger == NULL)
       {
              printf("Couldn't log status \n");
              exit(-1);
       }
       
       while(1){
              
              // printf("\n waiting on log_mq \n");
              if((nbytes = mq_receive(log_mq,(char*)&sensor_recv, MAX_MSG_SIZE, &prio)) == ERROR)
              {
                     perror("mq_receive");
              }
              else{
                     
                     //printf("%s\n", sensor_recv.data.logger_data);
                     fprintf(fp_logger, "%s %s\n\n", getDateString(),sensor_recv.data.logger_data+1);
                     
              }
              
              pthread_mutex_lock(&loggerMutex);
              heartBeatIndex[LOGGER]++;
              pthread_cond_signal(&loggerCond); // Signal as now globalvar is 1
              pthread_mutex_unlock(&loggerMutex);
              
              // pthread_exit(0);
              
       }
       
       
       
}