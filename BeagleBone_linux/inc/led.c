#include <stdio.h>


int LED_init(){
       FILE *fp;
       
       if((fp = fopen("/sys/class/gpio/gpio68/direction", "w+"))!=NULL){
               fwrite("out", 1, 1 , fp);
               fclose(fp);
       }else{
               perror("File Open Error:");
               return -1;
       }
       
}

int set_LED(char* val){
       
       FILE *fp;
       
       if((fp = fopen("/sys/class/gpio/gpio68/value", "w+"))!=NULL){
               fwrite(val, 1, 1 , fp);
               fclose(fp);
       }else{
               perror("File Open Error:");
               return -1;
       }
       
}

int main(){
       
       LED_init();
       set_LED("0");
       
       
       
       return 0;
       
       
}