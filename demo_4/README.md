#CS244P IoT Demo 4

### Designing a step counter

Your task is to build a step counter using Arduino Uno board and send the number of steps to the
cell phone using NRF, whenever it changed.

### A) Connecting the accelerometer to Arduino

The LSM6DS3 is an accelerometer and gyroscope sensor with an 8 Kbyte buffer and embedded processing interrupt functions. You can use either SPI or i2c to get data from this board.

**CAUTION**: this breakout works only with 3.3 v. more voltage will cause permanent damage to this
board. You can find more information here: https://learn.sparkfun.com/tutorials/lsm6ds3-breakout-hookupguide. Since in this session, you will use two 3.3v breakouts, you might use breadboard to connect them to the 3.3v pin of Arduino. Check the figure. 

### B) Connecting NrfnRF24L01+ to Arduino:

Connect your nRF24L01+ to the Arduino. (The voltage range of this device is 3.3 - 7, but it is apractice to get familiar with breadboard.)

- 3.3V → VCC (breadboard)
- GND → GND (breadboard)
- D8 → IRQ
- D9 → CE
- D10 → CSN
- D11 → MOSI
- D12 → MISO
- D13 → SCK

Download the library from https://cdn.sparkfun.com/assets/learn_tutorials/2/9/0/RF24.zip and
install it in your Arduino IDE. You can find more information here:
https://learn.sparkfun.com/tutorials/nrf24l01-transceiver-hookup-guide

### C) Sending BLE packet with nRF24L01+

You need to install NRF Master Control Panel (BLE) on your cell phone. This application is available for both Android and iOS. Download the BTLE package from https://github.com/floe/BTLE, and run the “send” example from this package. In send example, change the “foobar” to a random name. You should see the data on your phone with this name. Note that you cannot bind your device to NRF because the NRF is broadcasting data. Therefore, until your NRF application is scanning, you can see the data. You can change the scanning time in device->setting-> scanner.

### D) Develop a very simple step counter:

1. You need to calibrate the sensor at first. (Remember how you calibrate a sensor and
   mapped it).
2. In the loop, continuously read the data from analog pin (for example each 20 ms). You can
   just read the data from one of the axis, or read both x and y, and calculate their root mean
   square. The later method is more accurate.
3. To count the number of steps, you need to set a threshold. To do so, use Arduino IDE
   serial plotter (cntl+sht+L) to plot the result from step 2. Move the accelerometer to mimic
   walking. In the plotter, find a proper threshold that can be used as step detector.
4. Count the number of steps using this threshold. Note that for each step, there might be
   more than one value over threshold. You might use proper delaying to solve this problem.
5. Send the number of taken steps to the cell phone using the NrfnRF24L01.
6. Explain what is the security issue for sending data using the described method.
7. Search about how cell phone can detect if a person is running or walking.