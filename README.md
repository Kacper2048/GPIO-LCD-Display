# GPIO LCD - Linux (User Space)


### Description

 Pertfolio Project to get a job. The main goal of that is showing the temp of cpu and percents of memory usage. 
 It refresh itself every 0.5sec.

 Here is a link to get a model of lcd case: https://cults3d.com/en/3d-model/gadget/lcd-display-case
   
### Requires

 - Linux with desktop environment
 - Your system has to have a global acces to consecutive libraries: unistd.h, fcntl.h, string.h ,
 errno.h ,sys/ioctl.h, stdint.h, stdlib.h, cstdio, linux/gpio.h, string, iostream, math.h, stdio.h
 - super user permissions
  

### SetUp

 (PC) Just compile the program and run it with sudo permissions

 In order to compile the program you have to use g++ `g++ -0 LCD main.cpp own_gpio.h own_gpio.cpp`
 Run the program by `./LCD.out`

 (LCD) Connect wires according to instruction `instruction.png`

### Limits
 Program run in blocking mode and this is why sometimes It have to be run few times to start work correctly.
