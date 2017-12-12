#include "communication.h"
#include "system.h"
#include <signal.h>



int main(){
    
    int8_t error_flag = 0;
    int8_t errorno = 0;
    pthread_t threads[NUM_THREADS];
    
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
    printf("exiting... %d \n", error_flag);
   
   
   
    pthread_join(threads[3], NULL);
    pthread_join(threads[2], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[0], NULL);
   
   
   
   
   
   
//   char c;
//   int num;
//   char *device = "/dev/ttyO1";
//   char str[100];
   
//   signal(SIGINT, int_handler);
//   fd = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
//   if(fd == -1){
              
// 		perror("ERROR opening file descriptor\n");
// 	}

// 	configure = (struct termios*)malloc(sizeof(struct termios));
// 	tty_config(configure, fd);
	
	
// 	//read_bytes(fd, str, c);
// 	//printf("%s\n",str);
// 	transfer_bytes(fd, "hello\n");
	
	
	
// 	free(configure);
// 	close(fd);
       
       
       
       
}