#include "own_gpio.h"
#include <iostream>
#include <math.h>

using namespace std;

void LCD_init();
void LCD_DATA(int);
void LCD_CMD(unsigned char CMD)

#define LCD_EN_Delay 100000

bool tab[8]{0,0,0,0,0,0,0,0};
int fd;
struct gpiochip_info info;
struct gpioline_info line_info;
struct gpiohandle_request rq;

int main(int argc, char *argv[])
{
    //these structures are needed to all operations involved with gpio 

    fd = open_file();
    
    if(fd > -1)
    {
       if(init_chip(fd,info) > -1)
        {
            //show_info(fd,info);
            init_8pins(fd,rq);

            LCD_Init();

            close_file(fd);
        }
        

    }
    return 0;
}

void LCD_DATA(int val)
{
    for(int i=0;i<4;i++)
    {
        (val & (pow(2,i)))? tab[i] = 1 : tab[i] = 0;
    }

}

void LCD_CMD(unsigned char CMD)
{
    // Select Command Register
    //RS = 0;
    tab[5] = 0;

    // Move The Command Data To LCD
    LCD_DATA(CMD);

    // Send The EN Clock Signal
    //EN = 1;
    tab[4] = 1;
    send_8bit(fd,rq,tab);    
    usleep(LCD_EN_Delay);
    //EN = 0;
    tab[4] = 0;
    send_8bit(fd,rq,tab);
}

void LCD_Init()
{
  // IO Pin Configurations
  LCD_DATA_PORT_D = 0x00;
  LCD_RS_D = 0;
  LCD_EN_D = 0;
  // The Init. Procedure
  LCD_DATA(0x00);
  __delay_ms(30);
  __delay_us(LCD_EN_Delay);
  LCD_CMD(0x03);
  __delay_ms(5);
  LCD_CMD(0x03);
  __delay_us(150);
  LCD_CMD(0x03);
  LCD_CMD(0x02);
  LCD_CMD(0x02);
  LCD_CMD(0x08);
  LCD_CMD(0x00);
  LCD_CMD(0x0C);
  LCD_CMD(0x00);
  LCD_CMD(0x06);
}
