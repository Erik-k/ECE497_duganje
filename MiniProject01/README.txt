Josh Dugan
ECE 497
Mini Project 01
9/12/12

This program uses some of the basic functions of the gpio, pwm,
ADC, i2c and I/O driven interrupts

A 10k potentiameter is connected between 0V and 1.8V with the center tap 
connected to ain0 (pin 39 on P9). 
A push button is connected to gpio1_17 (23, P9).
A red LED is connected to gpio1_16 (15, P9).
A green LED is connected to ehrpwm1 (14, P9).
A CT74 temperature sensor is connnected to VCC, GND and I2C2 (19 and 20, P9).

The pwm output is used to adjust the brightness of the green LED based on the 
temperature read from the TC74. the LED is off for temperatures less than 25
degrees C, and at 31 degrees C the LED reaches full brightness. 
When the push button is depressed, the red LED lights up and the temperature 
and converted voltage are printed.
