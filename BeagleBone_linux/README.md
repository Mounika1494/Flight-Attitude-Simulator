# BeagleBone Side #

This firmware package is for simulating flight attitude simulator with IMU(MPU 9250) and remote logging, processing of IMU data is performed on BeagleBone Green. The communication between TIVA and BBG is through UART. This system is implemented with heartbeat on every functionality operational then LED3 will be ON. If any error LED4 will be ON. If BBG processing results in FATAL condition we are logging state of the system in EEPROM(Black box) which in reality can be more important data.

## Required Components ##
* Code Composer Studio (v6.0.1 used for this project)
* [TM4C1294 Connected Launchpad](http://www.ti.com/tool/ek-tm4c1294xl)
* [MPU 9250](https://www.sparkfun.com/products/13762)
* [Beagle Bone Green](https://beagleboard.org/green)

## Expected Output ##
* Code will build with no errors or warnings
* Usr LED 0 is the hearbeat LED
* Actuation LEDs are connected to GPIO 66,67,68,69
* UART is connected to GPIO 24 (TX) and GPIO 26 (RX)

## Notes ##
* To enable UART on BeagleBone, do : root@beaglebone:~# config-pin P9.24 uart and root@beaglebone:~# config-pin P9.26 uart
* beaglebone:green:usr0 is the heart beat LED. To enable it go to /sys/class/leds/ , cd into beaglebone:green:usr0, do 'echo gpio > trigger'
* To load the kernel device driver, cd into the device driver folder and run make. Then do 'sudo insmod led_module.ko' to load the kernel driver.
* That's all there is to do. Now just run make from the root directory and run ./APES <logger_file_name.txt>
