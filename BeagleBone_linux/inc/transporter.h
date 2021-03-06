/**********************************************************************************************************
* Copyright (C) 2017 by JayaKrishnan HJ
*
*Redistribution,modification or use of this software in source or binary fors is permitted as long 
*as the files maintain this copyright. JayaKrishnan HJ is not liable for any misuse of this material
*
*********************************************************************************************************/
/**
* @file transporter.h
* @brief all the structures used by system
*
*This header file contains the interrupt handler and the enums for log and data
*
* @author Mounika Reddy Edula
* @       JayaKrishnan HJ
* @date December 11 2017
* @version 1.0
*
*/


#ifndef _TRANSPORTER_H_
#define _TRANSPORTER_H_

enum{
       DATA = 1,
       LOG
};

void int_handler();

#endif