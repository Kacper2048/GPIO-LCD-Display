#include "own_gpio.h"

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
    rq.lineoffsets[1] = 18; //12
    rq.lineoffsets[2] = 27; //13
    rq.lineoffsets[3] = 22; //15

    rq.lines = 4;
    rq.flags = GPIOHANDLE_REQUEST_OUTPUT;

    ret = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &rq);
    if (ret == -1)
    {
        printf("Unable to set line value using ioctl : %s", strerror(errno));
    }

    return ret;
}

int send_4bit(int fd,gpiohandle_request &rq,bool *tab)
{
    int ret;
    gpiohandle_data data;

    //set the specific bit value onto the specific pin
    for(int i=0;i<4;i++)
    {
        data.values[i] = (int)tab[i];
    }

    ret = ioctl(rq.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);

    if (ret == -1)
    {
        printf("Unable to set line value using ioctl : %s", strerror(errno));
    }

    return ret;
}

int init_8pins(int fd, gpiohandle_request &rq)
{
    int ret;

    //setting the output pins 
    rq.lineoffsets[0] = 17; //11
    rq.lineoffsets[1] = 18; //12
    rq.lineoffsets[2] = 27; //13
    rq.lineoffsets[3] = 22; //15
    rq.lineoffsets[4] = 23; //16
    rq.lineoffsets[5] = 24; //18
    rq.lineoffsets[6] = 25; //22
    rq.lineoffsets[7] = 5;  //29

    rq.lines = 8;
    rq.flags = GPIOHANDLE_REQUEST_OUTPUT;

    ret = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &rq);
    if (ret == -1)
    {
        printf("Unable to set line value using ioctl : %s", strerror(errno));
    }

    return ret;
}

int send_8bit(int fd,gpiohandle_request &rq,bool *tab)
{
    int ret;
    gpiohandle_data data;

    //set the specific bit value onto the specific pin
    for(int i=0;i<8;i++)
    {
        data.values[i] = (int)tab[i];
    }
 
    ret = ioctl(rq.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);

    if (ret == -1)
    {
        printf("Unable to set line value using ioctl : %s", strerror(errno));
    }

    return ret;
}

void to_bits8(int val,bool * tab,int size)
{
    //int i = size-1; //-1 because this will be index number without swap
    int temp = 0;
    int lenght = 0;
    int i = 0; //with swap
    //without swap
    /*
    while(val>0)
    {
        tab[i] = val%2;
        val = val / 2;
        lenght++;
        i--;
        
    }
    */
    //with swap
    while(val>0)
    {
        tab[i] = val%2;
        val = val / 2;
        lenght++;
        i++;  
    }
}

void close_request(gpiohandle_request &rq)
{
    close(rq.fd);
}

void close_file(int fd)
{
    close(fd);
}