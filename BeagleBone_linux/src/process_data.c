#include "system.h"
#include "math.h"

void *processThread(void *threadp){
       
       printf("processor thread\n");
       message_t sensor_recv;
       message_t led_send;
       int nbytes, prio;
       float Acceleration[3], Degrees[3];
       float acceleration_bias[3]={0.0,0.0,0.0};
       float degree_bias[3]={0.0,0.0,0.0};
       float Pitch, Roll, Yaw;
       float Acceleration_Pitch, Acceleration_Roll, Acceleration_Yaw;
       
       while(1){
              
              fprintf(stderr,"\n waiting on process_mq \n");
              if((nbytes = mq_receive(proc_mq,(char*)&sensor_recv, MAX_MSG_SIZE, &prio)) == ERROR)
              {
                     perror("mq_receive");
              }
              else{
                     printf("x_ddot : %f\n", sensor_recv.data.IMUdata.x_ddot);
                     
                     Acceleration[0] = sensor_recv.data.IMUdata.x_ddot;
                     Acceleration[1] = sensor_recv.data.IMUdata.y_ddot;
                     Acceleration[2] = sensor_recv.data.IMUdata.z_ddot;
                     Degrees[0] = sensor_recv.data.IMUdata.pitch_dot;
                     Degrees[1] = sensor_recv.data.IMUdata.roll_dot;
                     Degrees[2] = sensor_recv.data.IMUdata.yaw_dot;
                     
                     Acceleration[0] -= acceleration_bias[0];
                     Acceleration[1] -= acceleration_bias[1];
                     //Acceleration[2] -= acceleration_bias[2];
                     
                     Degrees[0] -= degree_bias[0];
                     Degrees[1] -= degree_bias[1];
                     Degrees[2] -= degree_bias[2];
                     
                     Pitch += Degrees[0] * .01;
                     Roll += Degrees[1] * .01;
                     Yaw += Degrees[2] * .01;
                     
                     Acceleration_Pitch = atan2f(Acceleration[1], sqrtf(Acceleration[0] * Acceleration[0] + Acceleration[2] * Acceleration[2]) ) * 180 / M_PI;
                     Acceleration_Roll = atan2f(Acceleration[0], sqrt(Acceleration[1] * Acceleration[1] + Acceleration[2] * Acceleration[2]) ) * 180 / M_PI;
                     Acceleration_Yaw = atan2f(Acceleration[2], sqrt(Acceleration[0] * Acceleration[0] + Acceleration[1] * Acceleration[1]) ) * 180 / M_PI;
                     
                     Pitch = 0.94 * Pitch + 0.06 * Acceleration_Pitch;
                     Roll = 0.94 * Roll + 0.06 * Acceleration_Roll;
                     Yaw = 0.98 * Yaw + 0.02 * Acceleration_Yaw;
                     
                     printf("pitch : %f\n", Pitch);
                     printf("roll : %f\n", Roll);
                     
                     if (Pitch > 20){
                            
                            
                            led_send.data.LED.LED1 = GLOW;
                            led_send.data.LED.LED2 = DARK;
                            // led_send.data.LED.LED3 = DARK;
                            // led_send.data.LED.LED4 = DARK;
                     }else if(Pitch < -20){
                            led_send.data.LED.LED1 = DARK;
                            led_send.data.LED.LED2 = GLOW;
                            // led_send.data.LED.LED3 = DARK;
                            // led_send.data.LED.LED4 = DARK;
                     }
                     
                     if (Roll > 0){
                            // led_send.data.LED.LED1 = DARK;
                            // led_send.data.LED.LED2 = DARK;
                            led_send.data.LED.LED3 = GLOW;
                            led_send.data.LED.LED4 = DARK;
                     }else if(Roll < 0){
                            // led_send.data.LED.LED1 = DARK;
                            // led_send.data.LED.LED2 = DARK;
                            led_send.data.LED.LED3 = DARK;
                            led_send.data.LED.LED4 = GLOW;
                            
                     }
                     
                     if((nbytes = mq_send(led_mq, (char *)&led_send, sizeof(led_send), 30)) == ERROR)
                     {
                            printf("%d\n",nbytes);
                            perror("mq_send");
                     }
                     else
                     {
                            printf("Sending LED data to process MQ %d bytes: message successfully sent\n", nbytes);
                     }
                     
                     
                     
                     
              }
              
       }
       
}