/**********************************************************************************************************
* Copyright (C) 2017 by JayaKrishnan HJ
*
*Redistribution,modification or use of this software in source or binary fors is permitted as long 
*as the files maintain this copyright. JayaKrishnan HJ is not liable for any misuse of this material
*
*********************************************************************************************************/
/**
* @file communication.h
* @brief all the structures used by system
*
*This header file provides the communication APIs to communicate with the Tiva
*
* @author Mounika Reddy Edula
* @       JayaKrishnan HJ
* @date December 11 2017
* @version 1.0
*
*/




#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>

int fd;
struct termios *configure;

/***
* @brief Initialises the serial driver 
*
*
*@param con - pointer to the termios struct
*       descriptor -   file decriptor
*       
*
*@return int8_t fail - any error in initialization
*               success - successfull initialisation
*/

int8_t tty_config(struct termios *con, int descriptor);



/***
* @brief reads one byte
*
*
*@param fileHandle - file handle for that slave used for rest communication
*       filename -   pointer for the specific i2c-2/i2c-1
*       slaveAddress - address of the slave you want to communicate
*
*@return int8_t fail - any error in connecting to the device
*               success - successfull initialisation
*/
char read_one_byte(int fd);



/***
* @brief read multiple bytes
*
*
*@param fd - file handle to read from
*       str - pointer to a string to store the bytes*       
*
*@return int8_t fail - any error in accessing the file descriptor
*               success - successfull initialisation
*/
int8_t read_bytes(int fd, char *str);



/***
* @brief Send one byte
*
*
*@param fd - file handle to write to 
*     
*
*@return int8_t fail - any error in accessing the file descriptor
*               success - successfull initialisation
*/
int8_t transfer_one_byte(int fd);



/***
* @brief Send multiple bytes
*
*
*@param fd - file handle to write to 
*       filename -   pointer for the specific i2c-2/i2c-1
*       slaveAddress - address of the slave you want to communicate
*
*@return int8_t fail - any error in accessing the file descriptor
*               success - successfull initialisation
*/
int8_t transfer_bytes(int fd, char *str);


#endif