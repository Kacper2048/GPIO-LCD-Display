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
void close_file(int fd);
void close_request(gpiohandle_request &rq);

int init_chip(int fd, gpiochip_info & info); //uses the ioctl to check if the chip is avaiable and then to get info form it
void show_info(int fd, gpiochip_info const & info); //It return list of pins's layout 
int init_4pins(int fd, gpiohandle_request &rq); //it inits 4pins as output, 
int send_4bit(int fd,gpiohandle_request &rq);


int main(int argc, char *argv[])
{
    
    struct gpiochip_info info;
    struct gpioline_info line_info;
    struct gpiohandle_request rq;

    int fd = open_file();

    if(fd > -1)
    {
        if(init_chip(fd,info) > -1)
        {
            show_info(fd,info);
            init_4pins(fd,rq);
            send_4bit(fd,rq);

            close_file(fd);
        }
    }
    

    return 0;
}

int open_file()
{
    int fd;
    fd = open(DEV_NAME,O_RDONLY);

    if(fd<0)
    {
        printf("Unabled to open %s: %s", DEV_NAME, strerror(errno));
        return -1;
    }
    else
    {
        printf("the File opened successfully %s (~^-^)~\n", DEV_NAME);
    }

    return fd;
}

int init_chip(int fd, gpiochip_info & info)
{
    int ret;
    ret = ioctl(fd, GPIO_GET_CHIPINFO_IOCTL, &info);

    if (ret == -1)
    {
        printf("Unable to get chip info from ioctl: %s", strerror(errno));
        close(fd);
        return -1;
    }
    else
    {
        printf("the chip opened successfully (~^-^)~\n");
        printf("Chip name: %s\n", info.name);
        printf("Chip label: %s\n", info.label);
        printf("Number of lines: %d\n", info.lines);
    }

    return ret;
}

void show_info(int fd, gpiochip_info const &info) //It return list of pins's layout 
{
    struct gpioline_info line_info;
    int ret;
    
    for (int i = 0; i < info.lines; i++)
    {
        line_info.line_offset = i;

        ret = ioctl(fd, GPIO_GET_LINEINFO_IOCTL, &line_info);

        if (ret == -1)
        {
            printf("Unable to get line info from offset %d: %s", i, strerror(errno));
        }
        else
        {
            printf("offset: %d, name: %s, consumer: %s. Flags:\t[%s]\t[%s]\t[%s]\t[%s]\t[%s]\n",
                i,
                line_info.name,
                line_info.consumer,
                (line_info.flags & GPIOLINE_FLAG_IS_OUT) ? "OUTPUT" : "INPUT",
                (line_info.flags & GPIOLINE_FLAG_ACTIVE_LOW) ? "ACTIVE_LOW" : "ACTIVE_HIGHT",
                (line_info.flags & GPIOLINE_FLAG_OPEN_DRAIN) ? "OPEN_DRAIN" : "...",
                (line_info.flags & GPIOLINE_FLAG_OPEN_SOURCE) ? "OPENSOURCE" : "...",
                (line_info.flags & GPIOLINE_FLAG_KERNEL) ? "KERNEL" : "");
        }
    }
}

int init_4pins(int fd, gpiohandle_request &rq)
{
    int ret;

    //setting the output pins 
    rq.lineoffsets[0] = 17; //11
    rq.lineoffsets[1] = 27; //13
    rq.lineoffsets[2] = 22; //15
    rq.lineoffsets[3] = 23; //16
    rq.lines = 4;
    rq.flags = GPIOHANDLE_REQUEST_OUTPUT;

    ret = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &rq);
    if (ret == -1)
    {
        printf("Unable to set line value using ioctl : %s", strerror(errno));
    }

    return ret;
}

int send_4bit(int fd,gpiohandle_request &rq)
{
    int ret;
    gpiohandle_data data;
    bool tab[4]{1,1,1,1};

    data.values[0] = 1; // HIGH offset 11
    data.values[1] = 1; // LOW offset 13
    data.values[2] = 1; // LOW offset 15
    data.values[3] = 1; // LOW offset 16

    for(int i=0; i<100;i++)
    {
        if(i%4==0)
        {
            data.values[0] = (int)tab[0]; // HIGH offset 11
            tab[0] = !tab[0];
        }
        else if(i%4==1)
        {
            data.values[1] = (int)tab[1]; // HIGH offset 11
            tab[1] = !tab[1];
        }
        else if(i%4==2)
        {
            data.values[2] = (int)tab[2]; // HIGH offset 11
            tab[2] = !tab[2];
        }
        else if(i%4==3)
        {
            data.values[3] = (int)tab[3]; // HIGH offset 11
            tab[3] = !tab[3];
        }

        ret = ioctl(rq.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);

        if (ret == -1)
        {
            printf("Unable to set line value using ioctl : %s", strerror(errno));
        }

        usleep(200000);
    }
    
    return ret;
}

void close_request(gpiohandle_request &rq)
{
    close(rq.fd);
}

void close_file(int fd)
{
    close(fd);
}