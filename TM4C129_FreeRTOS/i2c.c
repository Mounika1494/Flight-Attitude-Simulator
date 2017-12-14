#include "i2c.h"
/**************************************************************************************
*@Filename:i2c.c
*
*@Description: Implementation of i2c driver for I2C2. For initializing i2c,read one byte
*read n bytes,write one byte,write n bytes are implemented
*
*@Author:Mounika Reddy Edula
*        JayaKrishnan H.J
*@Date:12/11/2017
*@compiler:gcc
*@debugger:gdb
**************************************************************************************/
//Mutex lock to disable preemption
SemaphoreHandle_t i2c_mutex;

//I2C initialisation enabling I2C,clock and GPIO
void I2C_Init(void){
    uint32_t sys_Clock;
        //enable GPIO peripheral that contains I2C 0
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));

        // Configure the pin muxing for I2C0 functions on port B2 and B3.
        GPIOPinConfigure(GPIO_PN4_I2C2SDA);
        GPIOPinConfigure(GPIO_PN5_I2C2SCL);

        // Select the I2C function for these pins.
        GPIOPinTypeI2C(GPIO_PORTN_BASE, GPIO_PIN_4);
        GPIOPinTypeI2CSCL(GPIO_PORTN_BASE, GPIO_PIN_5);

        // Set the clocking to run directly from the external crystal/oscillator.
        sys_Clock =    SysCtlClockFreqSet((SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_XTAL_25MHZ |
            SYSCTL_CFG_VCO_480), 120000000);
        SysCtlPeripheralDisable(SYSCTL_PERIPH_I2C2);
        SysCtlPeripheralReset(SYSCTL_PERIPH_I2C2);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);

        //
        // Wait for the Peripheral to be ready for programming
        //
        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C2));

        // Enable and initialize the I2C2 master module.  Use the system clock for
        // the I2C0 module.  The last parameter sets the I2C data transfer rate.
        // If false the data rate is set to 100kbps and if true the data rate will
        // be set to 400kbps.
        I2CMasterInitExpClk(I2C2_BASE, sys_Clock, true);

        i2c_mutex = xSemaphoreCreateMutex();
        if(i2c_mutex != NULL)
        {

        }

    //I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), true);
}


//Write n bytes to specified device address
void I2C_WriteBytes(uint8_t device_address, uint8_t device_register, uint8_t *data, uint8_t lenght){

    if(xSemaphoreTake(i2c_mutex,portMAX_DELAY) != pdTRUE)
    {
        //error
    }
    I2CMasterSlaveAddrSet(I2C2_BASE, device_address, false);
    I2CMasterDataPut(I2C2_BASE, device_register);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    while(I2CMasterBusy(I2C2_BASE));

    uint8_t i;
    for(i = 0; i < lenght - 1; i++){
        I2CMasterDataPut(I2C2_BASE, data[i]);
        I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
        while(I2CMasterBusy(I2C2_BASE));
    }

    I2CMasterDataPut(I2C2_BASE, data[lenght - 1]);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);

    while(I2CMasterBusy(I2C2_BASE));
    xSemaphoreGive(i2c_mutex);

}

//Write 1 byte internally using n bytes with 1
void I2C_WriteByte(uint8_t device_address, uint8_t device_register, uint8_t data){
    I2C_WriteBytes(device_address, device_register, &data, 1);
}

//Read n bytes from device specified
void I2C_ReadBytes(uint8_t device_address, uint8_t device_register, uint8_t *data, uint8_t lenght){
    if(xSemaphoreTake(i2c_mutex,portMAX_DELAY) != pdTRUE)
        {
            //error
        }
    uint8_t i;
    I2CMasterSlaveAddrSet(I2C2_BASE, device_address, false);
    I2CMasterDataPut(I2C2_BASE, device_register);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);

    while(I2CMasterBusy(I2C2_BASE));

    I2CMasterSlaveAddrSet(I2C2_BASE, device_address, true);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);

    while(I2CMasterBusy(I2C2_BASE));

    data[0] = I2CMasterDataGet(I2C2_BASE);

    for(i = 1; i < lenght - 1; i++){
        I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
        while(I2CMasterBusy(I2C2_BASE));
        data[i] = I2CMasterDataGet(I2C2_BASE);
    }

    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    while(I2CMasterBusy(I2C2_BASE));
    data[lenght - 1] = I2CMasterDataGet(I2C2_BASE);
    xSemaphoreGive(i2c_mutex);
}

//Read Single byte from device address specified and returns the register value read
uint8_t I2C_ReadByte(uint8_t device_address, uint8_t device_register){
    if(xSemaphoreTake(i2c_mutex,portMAX_DELAY) != pdTRUE)
    {
        //error
    }
    uint8_t data;
    I2CMasterSlaveAddrSet(I2C2_BASE, device_address, false);
    I2CMasterDataPut(I2C2_BASE, device_register);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);

    while(I2CMasterBusy(I2C2_BASE));

    I2CMasterSlaveAddrSet(I2C2_BASE, device_address, true);
    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

    while(I2CMasterBusy(I2C2_BASE));

    data = I2CMasterDataGet(I2C2_BASE);
    xSemaphoreGive(i2c_mutex);
    return data;
}


