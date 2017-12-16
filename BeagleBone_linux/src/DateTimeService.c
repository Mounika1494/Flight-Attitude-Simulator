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