/**********************************************************************************************************
* Copyright (C) 2017 by JayaKrishnan HJ
*
*Redistribution,modification or use of this software in source or binary fors is permitted as long 
*as the files maintain this copyright. JayaKrishnan HJ is not liable for any misuse of this material
*
*********************************************************************************************************/
/**
* @file DateTimeService.h
* @brief all the structures used by system
*
*This file provides the API to get the timestamp in the required format
*
* @author Mounika Reddy Edula
* @       JayaKrishnan HJ
* @date December 11 2017
* @version 1.0
*
*/

#include "DateTimeService.h"
#include <time.h>
#include <stdlib.h>


//get date and time in proper format for logger
char* getDateString() {
    // Initialize and get current time
    time_t t = time( NULL );

    // Allocate space for date string
    char* date = (char*)malloc( 100 );

    // Format the time correctly
    strftime(date, 100, "[%F %T]", localtime(&t));

    return date;
}