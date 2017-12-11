#include "gpio.h"
/**************************************************************************************
*@Filename:gpio.c
*
*@Description: Implementation to make a gpio pin interrupt and write 1 to a GPIO
*
*@Author:Mounika Reddy Edula
*        JayaKrishnan H.J
*@Date:12/11/2017
*@compiler:gcc
*@debugger:gdb
**************************************************************************************/
//Initialise GPIO for alarm and GPIO Interrupt for MPU9250
void GPIO_Init(void){
    /*GPIO setup for alarm LEDs*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    /*GPIO setup for MPU9250 interrupt pin*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

//function to turn on the alarm
void GPIO_on(uint8_t pins){
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3, pins);
}
