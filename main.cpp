#include "own_gpio.h"
#include <iostream>

void to_bits8(int val,bool * tab,int size);
using namespace std;

int main(int argc, char *argv[])
{
    //these structures are needed to all operations involved with gpio 
    struct gpiochip_info info;
    struct gpioline_info line_info;
    struct gpiohandle_request rq;

    int fd = open_file();
    bool tab[8]{0,0,0,0,0,0,0,0};

    if(fd > -1)
    {
       if(init_chip(fd,info) > -1)
        {
            show_info(fd,info);
            init_8pins(fd,rq);

            for(int i=0;i<100;i++)
            {
                send_8bit(fd,rq,tab);

                for(int z=0;z<8;z++)
                {
                    tab[z] = !tab[z];
                }
                usleep(200000);
            }
            
            close_file(fd);
        }
        

    }
 
    return 0;
}

