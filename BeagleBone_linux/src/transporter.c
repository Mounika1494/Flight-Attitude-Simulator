#include "system.h"
#include "communication.h"
#include "transporter.h"
#include <signal.h>

static float Acceleration[3], Degrees[3];
static char str[100];

void int_handler(){
       
       close(fd);
       exit(0);
       
}



void *uartThread(void *threadp){
       
       printf("uart thread\n");
       char *device = "/dev/ttyO1";
       message_t sensor_recv;
       int nbytes;
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
              
              if(tcsetattr(fd, TCSAFLUSH, configure) < 0){
	    
		perror("ERROR in set attr\n");
		
	       }
              read_bytes(fd, str);
              strcpy(debug, str);
              temp = strtok(str, " ");
       
              if(strcmp(temp,"D")==0){
                     
                     while (temp != NULL)
                     {
                            printf ("%s\n",temp);
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
                            printf("Sending IMU data to process MQ %d bytes: message successfully sent\n", nbytes);
                     }
                     
                     
              }else {
                     
                     strcpy(sensor_recv.data.logger_data, debug);
                     if((nbytes = mq_send(log_mq, (char *)&sensor_recv, sizeof(sensor_recv), 30)) == ERROR)
                     {
                            perror("mq_send");
                     }
                     else
                     {
                            printf("Sending IMU data to LOG MQ %d bytes: message successfully sent\n", nbytes);
                     }
                     
                     }
                     
                     
              }
       
       
}