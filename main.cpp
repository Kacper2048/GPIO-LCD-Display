#include "own_gpio.h"
#include <string>
#include <iostream>
#include <math.h>

using namespace std;

void LCD_Init();
void LCD_DATA(int);
void LCD_CMD(unsigned char CMD);
void LCD_String(string str);

#define LCD_EN_Delay 500

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
            LCD_String("hej");
            close_file(fd);
        }
        

    }
    return 0;
}

void LCD_String(string str)
{
    for(int i=0;i<str.length(); i++)
    {
        LCD_DATA(static_cast<int>(str[i]));
    }
}

void LCD_DATA(int val) //The main goal of that function is to change int value to binary form 
{
    // Select Command Register
    //RS = 0;
    tab[5] = 0;

    int upper = val & 0xF0;
    int lower = val & 0x0F;

    //sending upper part of data to lcd
    for(int i=0;i<4;i++)
    {
        (upper & static_cast<int>(pow(2,i)))? tab[i] = 1 : tab[i] = 0;
    }
    // Send The EN Clock Signal
    //EN = 1;
    tab[4] = 1;

    usleep(LCD_EN_Delay);
    send_8bit(fd,rq,tab); 
    usleep(LCD_EN_Delay);
    tab[4] = 0;
    send_8bit(fd,rq,tab);
    



    //sending lower part of data to lcd
    for(int i=0;i<4;i++)
    {
        (lower & static_cast<int>(pow(2,i)))? tab[i] = 1 : tab[i] = 0;
    }
    // Send The EN Clock Signal
    //EN = 1;
    tab[4] = 1;

    usleep(LCD_EN_Delay);
    send_8bit(fd,rq,tab); 

    tab[4] = 0;
    send_8bit(fd,rq,tab); 

}

void LCD_CMD(unsigned char CMD) //4bit mode
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
  // The Init. Procedure
  LCD_DATA(0x00);
  usleep(30);
  usleep(LCD_EN_Delay);
  LCD_CMD(0x03);
  usleep(5);
  LCD_CMD(0x03);
  usleep(150);
  LCD_CMD(0x03);
  LCD_CMD(0x02);
  LCD_CMD(0x02);
  LCD_CMD(0x08);
  LCD_CMD(0x00);
  LCD_CMD(0x0C);
  LCD_CMD(0x00);
  LCD_CMD(0x06);
}
