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
void  close_file(int fd);

int init_chip(int fd, gpiochip_info & info); //uses the ioctl to check if the chip is avaiable and then to get info form it
void show_info(int fd, gpiochip_info const & info); //It return list of pins's layout 

int main(int argc, char *argv[])
{
    
    struct gpiochip_info info;
    struct gpioline_info line_info;
    int fd = open_file();

    init_chip(fd,info);
    show_info(fd,info);

    close_file(fd);

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
        printf("the File opened successfully %s (~^-^)~", DEV_NAME);
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
        printf("the chip opened successfully (~^-^)~");
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

void close_file(int fd)
{
    close(fd);
}