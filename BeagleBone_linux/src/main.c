#include "communication.h"
#include "system.h"
#include <signal.h>



int main(int argc, char *argv[]){
    
    int8_t error_flag = 0;
    int8_t errorno = 0;
    int8_t nbytes;
    message_t sensor_recv;
    
    if (argc < 2)
	{
		printf("USAGE: <logger file>\n");
		exit(1);
	}
	
	logger_file = argv[1];
    
    /* setup common message q attributes */
    mq_attr.mq_maxmsg = 20;
    mq_attr.mq_msgsize = MAX_MSG_SIZE;
    mq_attr.mq_flags = 0;
    
    led_mq = mq_open(SNDRCV_MQ3, O_CREAT|O_RDWR, S_IRWXU, &mq_attr);
    if(led_mq == (mqd_t)ERROR){ 
    error_flag = 1;  
    }
    
    log_mq = mq_open(SNDRCV_MQ4, O_CREAT|O_RDWR, S_IRWXU, &mq_attr);
    if(log_mq == (mqd_t)ERROR){ 
    error_flag = 1; 
    
    }
    
    proc_mq = mq_open(SNDRCV_MQ5, O_CREAT|O_RDWR, S_IRWXU, &mq_attr);
    if(proc_mq == (mqd_t)ERROR){ 
     error_flag = 1; 
    }
    
    send_mq = mq_open(SNDRCV_MQ6, O_CREAT|O_RDWR, S_IRWXU, &mq_attr);
    if(proc_mq == (mqd_t)ERROR){ 
     error_flag = 1; 
    }
       
    printf("creating threads\n");
   	//Thread creation
    errno = pthread_create(&threads[0],   // pointer to thread descriptor
                		 NULL,     // use default attributes
                		 processThread, // thread function entry point
                		 (void *)&(threadParams[0]) // parameters to pass in		//Cant pass nothing so just pass a number
                		);
		if(errno){
		    error_flag = 1;
		 }

    errno = pthread_create(&threads[1],   // pointer to thread descriptor
                		 NULL,     // use default attributes
                		 logThread, // thread function entry point
                		 (void *)&(threadParams[1]) // parameters to pass in		//Cant pass nothing so just pass a number
                		);
		if(errno){
	        error_flag = 1;
	    }            		

    errno  = pthread_create(&threads[2],   // pointer to thread descriptor
                		 NULL,     // use default attributes
                		 ledThread, // thread function entry point
                		 (void *)&(threadParams[2]) // parameters to pass in		//Cant pass nothing so just pass a number
                		);
		if(errno){
            error_flag = 1;
        }     
        
    errno = pthread_create(&threads[3],   // pointer to thread descriptor
                		 NULL,     // use default attributes
                		 uartThread, // thread function entry point
                		 (void *)&(threadParams[3]) // parameters to pass in		//Cant pass nothing so just pass a number
                		);
		if(errno){
		    error_flag = 1;
		}
		
		  errno = pthread_create(&threads[4],   // pointer to thread descriptor
                		 NULL,     // use default attributes
                		 heartBeatThread, // thread function entry point
                		 (void *)&(threadParams[4]) // parameters to pass in		//Cant pass nothing so just pass a number
                		);
		if(errno){
		    error_flag = 1;
		}
    
    
    if(error_flag == 1)
    {
        error_flag = 0;
        sprintf(sensor_recv.data.logger_data,"%s"," DEBUG ERROR:pthread/MQ create error");
        sensor_recv.status = BAD;
        
        if((nbytes = mq_send(log_mq, (char *)&sensor_recv, sizeof(sensor_recv), 30)) == -1)
        {
            perror("mq_send");
        }
        else
        {
            printf("Core Dumped\n", nbytes);
        }
    }
   
   
    pthread_join(threads[4], NULL);
    pthread_join(threads[3], NULL);
    pthread_join(threads[2], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[0], NULL);
   
 

       
}