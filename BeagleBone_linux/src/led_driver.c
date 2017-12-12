#include "system.h"

static int8_t LED_init(){
       FILE *fp;
       
       if((fp = fopen("/sys/class/gpio/gpio66/direction", "w+"))!=NULL){
               fwrite("out", 1, 1 , fp);
               fclose(fp);
       }else{
               perror("File Open Error:");
               return -1;
       }
       
       
       if((fp = fopen("/sys/class/gpio/gpio67/direction", "w+"))!=NULL){
               fwrite("out", 1, 1 , fp);
               fclose(fp);
       }else{
               perror("File Open Error:");
               return -1;
       }
       
       
       
       if((fp = fopen("/sys/class/gpio/gpio68/direction", "w+"))!=NULL){
               fwrite("out", 1, 1 , fp);
               fclose(fp);
       }else{
               perror("File Open Error:");
               return -1;
       }
       
       
       
       if((fp = fopen("/sys/class/gpio/gpio69/direction", "w+"))!=NULL){
               fwrite("out", 1, 1 , fp);
               fclose(fp);
       }else{
               perror("File Open Error:");
               return -1;
       }
       
       
       return 0;
}

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


void *ledThread(void *threadp){
       
       printf("led thread\n");
       message_t sensor_recv;
       int nbytes, prio;
       
       LED_init();
       
       while(1){
              
              printf("\n waiting on led_mq \n");
              if((nbytes = mq_receive(led_mq,(char*)&sensor_recv, MAX_MSG_SIZE, &prio)) == ERROR)
              {
                     perror("mq_receive");
              }
              else{
                     printf("\n Received LED_MQ state %d\n", sensor_recv.data.LED.LED1);
                     
                     if(sensor_recv.data.LED.LED1 == GLOW){
                            
                            printf("LED1 ON\n");
                            set_LED1("1");
                            set_LED2("0");
                            // set_LED3("0");
                            // set_LED4("0");
                     }else if(sensor_recv.data.LED.LED2 == GLOW){
                            printf("LED2 ON\n");
                            set_LED1("0");
                            set_LED2("1");
                            // set_LED3("0");
                            // set_LED4("0");
                     }
                     
                     if(sensor_recv.data.LED.LED3 == GLOW){
                            printf("LED3 ON\n");
                            // set_LED1("0");
                            // set_LED2("0");
                            set_LED3("1");
                            set_LED4("0");
                     }else if(sensor_recv.data.LED.LED4 == GLOW){
                            printf("LED4 ON\n");
                            // set_LED1("0");
                            // set_LED2("0");
                            set_LED3("0");
                            set_LED4("1");
                     }
                     
                     
              }
              
       }
       
       
}