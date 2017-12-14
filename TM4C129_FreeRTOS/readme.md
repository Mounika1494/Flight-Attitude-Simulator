# Tiva TM4C1294 Launchpad #

This firmware package is for simulating flight attitude simulator with IMU(MPU 9250) and remote logging, processing of IMU data is performed on BeagleBone Green. The communication between TIVA and BBG is through UART. This system is implemented with heartbeat on every functionality operational then LED3 will be ON. If any error LED4 will be ON. If BBG processing results in FATAL condition we are logging state of the system in EEPROM(Black box) which in reality can be more important data.

## Required Components ##
* Code Composer Studio (v6.0.1 used for this project)
* [TM4C1294 Connected Launchpad](http://www.ti.com/tool/ek-tm4c1294xl)
* [MPU 9250](https://www.sparkfun.com/products/13762)
* [Beagle Bone Green](https://beagleboard.org/green)

## Expected Output ##
* Code will build with no errors or warnings
* Green LEDs D3 - ON on all functionality working
* Green LEDs D2 - UART transmission
* Serial port via Stellaris Virtual Serial Port will output sensor data

## Notes ##
If you want to test your IMU working then comment main in main.c and uncomment in IMU_test.c and look out in the serial terminal.
If you want to test your UART then connect in loop back and comment main() in main.c and uncomment in UART_test.c and look out in the serial terminal.
