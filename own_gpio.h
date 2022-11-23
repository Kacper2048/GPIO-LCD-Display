#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>
#include <cstdio>
#include <linux/gpio.h>

#define DEV_NAME "/dev/gpiochip0"

int open_file(); //return fd or -1 after opening the file
int init_chip(int fd, gpiochip_info & info); //uses the ioctl to check if the chip is avaiable and then to get info form it
void close_file(int fd);
void close_request(gpiohandle_request &rq);

void show_info(int fd, gpiochip_info const & info); //It return list of pins's layout 

int init_4pins(int fd, gpiohandle_request &rq); //it inits 4pins as output, 
int send_4bit(int fd,gpiohandle_request &rq,bool *tab); //send 4bit info to the reciving device

int init_8pins(int fd, gpiohandle_request &rq); //it inits 8pins as output, 
int send_8bit(int fd,gpiohandle_request &rq,bool *tab); //send 8bit info to the reciving device

void to_bits8(int val,bool * tab,int size); //change the int val to bits (from 0 to up to 255)
