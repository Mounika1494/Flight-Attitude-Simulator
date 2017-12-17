/*****************************************************************************
​ ​* ​ ​Copyright ​ ​(C) ​ ​2017 ​ ​by ​ Jayakrishnan HJ
​ ​*
​ ​* ​ ​Redistribution, ​ ​modification ​ ​or ​ ​use ​ ​of ​ ​this ​ ​software ​ ​in ​ ​source ​ ​or ​ ​binary
​ ​* ​ ​forms ​ ​is ​ ​permitted ​ ​as ​ ​long ​ ​as ​ ​the ​ ​files ​ ​maintain ​ ​this ​ ​copyright. ​ ​Users ​ ​are
​ ​* ​ ​permitted ​ ​to ​ ​modify ​ ​this ​ ​and ​ ​use ​ ​it ​ ​to ​ ​learn ​ ​about ​ ​the ​ ​field ​ ​of ​ ​embedded
​ ​* ​ ​software. ​ ​Jayakrishnan  HJ ​ ​and ​ ​the ​ ​University ​ ​of ​ ​Colorado ​ ​are ​ ​not ​ ​liable ​ ​for
​ ​* ​ ​any ​ ​misuse ​ ​of ​ ​this ​ ​material.
​ ​*
*****************************************************************************/
/**
​ ​* ​ ​@file ​ full_driver.c
​ ​* ​ ​@brief ​ ​Linux userspace application to access the kernel device driver 
​ ​*
​ ​* ​ ​This  ​file ​ ​provides ​ ​an ​ ​abstraction ​ ​of ​ ​reading ​ ​and
​ ​* ​ ​writing ​ ​to ​ various  states  of  an  LED.
​ ​*
​ ​* ​ ​@author ​ Jayakrishnan HJ
​ ​* ​ ​@date ​ ​November ​ 20 ​ ​2017
​ ​* ​ ​@version ​ ​1.0
​ ​*
​ ​*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAXBUFF 5

// "0" : Periodic mode
// "1" : Latch mode
int8_t setMode(char* mode){

    FILE *fp;

    if((fp = fopen("/sys/ebb/gpio53/mode", "w+"))!=NULL){
        fwrite(mode, 1, sizeof(char), fp);
        fclose(fp);
    }else{
        perror("File Open Error:");
        return -1;
    }
    return 0;
}

// "1" : Latch LED ON
// "0" : Latch LED OFF
// Ensure mode is set to Latch mode

int8_t setLED(char* state){

    FILE *fp;

    if((fp = fopen("/sys/ebb/gpio53/ledState", "w+"))!=NULL){
        fwrite(state, 1, sizeof(char), fp);
        fclose(fp);
    }else{
        perror("File Open Error:");
        return -1;
    }
    return 0;
}

// Set TIme period of LED flashing.
// Unit milliseconds
// Ensure mode is set to Periodic mode
int8_t setPeriod(char* period){

    FILE *fp;

    if((fp = fopen("/sys/ebb/gpio53/period", "w+"))!=NULL){
        fwrite(period, 1, strlen(period)+1 , fp);
        fclose(fp);
    }else{
        perror("File Open Error:");
        return -1;
    }
    return 0;
}


// Set duty cycle of LED flashing.
// (ON period)/period, input in percentage
// Ensure mode is set to Periodic mode
int8_t setDutycycle(char* dutycycle){

    FILE *fp;

    if((fp = fopen("/sys/ebb/gpio53/dutycycle", "w+"))!=NULL){
        fwrite(dutycycle, 1, strlen(dutycycle)+1 , fp);
        fclose(fp);
    }else{
        perror("File Open Error:");
        return -1;
    }
    return 0;
}

// "0" : Periodic mode
// "1" : Latch mode
int8_t readMode(char* mode){

    FILE *fp;

    if((fp = fopen("/sys/ebb/gpio53/mode", "r"))!=NULL){
        fread(mode, 1, MAXBUFF*sizeof(char), fp);
        fclose(fp);
    }else{
        perror("File Open Error:");
        return -1;
    }
    return 0;
}

// "1" :  LED ON
// "0" :  LED OFF

int8_t readLED(char* state){

    FILE *fp;

    if((fp = fopen("/sys/ebb/gpio53/ledState", "r"))!=NULL){
        fread(state, 1, MAXBUFF*sizeof(char), fp);
        fclose(fp);
    }else{
        perror("File Open Error:");
        return -1;
    }
    return 0;
}

// Read Time period of LED flashing.
// Unit milliseconds
// Ensure mode is set to Periodic mode
int8_t readPeriod(char* period){

    FILE *fp;

    if((fp = fopen("/sys/ebb/gpio53/period", "r"))!=NULL){
        fread(period, 1, MAXBUFF*sizeof(char) , fp);
        fclose(fp);
    }else{
        perror("File Open Error:");
        return -1;
    }
    return 0;
}


// Read duty cycle of LED flashing.
// (ON period)/period, input in percentage
// Ensure mode is set to Periodic mode
int8_t readDutycycle(char* dutycycle){

    FILE *fp;

    if((fp = fopen("/sys/ebb/gpio53/dutycycle", "r"))!=NULL){
        fread(dutycycle, 1, MAXBUFF*sizeof(char) , fp);
        fclose(fp);
    }else{
        perror("File Open Error:");
        return -1;
    }
    return 0;
}


int8_t query_all_states(char *mode, char *state,  char *dutycycle, char *period){

    readMode(mode);
    readLED(state);
    readPeriod(period);
    readDutycycle(dutycycle);
    return 0;
}



int main(){

    char period[MAXBUFF] = "1000";
    char dutycycle[MAXBUFF] = "50";
    char outmode[MAXBUFF];
    char outstate[MAXBUFF];
    char outperiod[MAXBUFF];
    char outdutycycle[MAXBUFF];

    memset(outmode, 0, MAXBUFF*sizeof(char));
    memset(outstate, 0, MAXBUFF*sizeof(char));
    memset(outperiod, 0, MAXBUFF*sizeof(char));
    memset(outdutycycle, 0, MAXBUFF*sizeof(char));

    setMode("0");
    //setLED("0");
    setPeriod(period);
    setDutycycle(dutycycle);
    readMode(outmode);
    readLED(outstate);
    readPeriod(outperiod);
    readDutycycle(outdutycycle);
    printf("mode: %s\n",outmode);
    printf("LED state: %s\n",outstate);
    printf("LED period: %s\n",outperiod);
    printf("Duty cycle: %s\n",outdutycycle);

    query_all_states(outmode, outstate, outdutycycle, outperiod);
    printf("mode: %s\n",outmode);
    printf("LED state: %s\n",outstate);
    printf("LED period: %s\n",outperiod);
    printf("Duty cycle: %s\n",outdutycycle);
}