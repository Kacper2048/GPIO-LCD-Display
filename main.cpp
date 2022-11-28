#include "own_gpio.h"
#include <string>
#include <iostream>
#include <math.h>

using namespace std;

void LCD_Init();
void LCD_DATA(int);
void LCD_CMD(unsigned char CMD);
void LCD_String(string str);
void LCD_xxx(int val);
void LCD_Clear();
void LCD_Set_Cursor(unsigned char r, unsigned char c);


#define LCD_EN_Delay 500

bool tab[8]{0,0,0,0,0,0,0,0};
int fd;

//these structures are needed to all operations involved with gpio 
struct gpiochip_info info;
struct gpioline_info line_info;
struct gpiohandle_request rq;

int main(int argc, char *argv[])
{
    fd = open_file();
    
    if(fd > -1)
    {
       if(init_chip(fd,info) > -1)
        {
            //show_info(fd,info);
            init_8pins(fd,rq);

            LCD_Init();
            LCD_Clear();
            LCD_Set_Cursor(1,1);
            //LCD_String("hej");
            
            cout << "ok chyba powinno być ok" << endl;
            usleep(5000000);
            close_file(fd);
        }
        

    }
    return 0;
}

void LCD_Set_Cursor(unsigned char r, unsigned char c)
{
  unsigned char Temp,Low4,High4;
  if(r == 1)
  {
    Temp = 0x80 + c - 1; //0x80 is used to move the cursor
    High4 = Temp >> 4;
    Low4 = Temp & 0x0F;
    LCD_CMD(High4);
    LCD_CMD(Low4);
  }

  if(r == 2)
  {
    Temp = 0xC0 + c - 1;
    High4 = Temp >> 4;
    Low4 = Temp & 0x0F;
    LCD_CMD(High4);
    LCD_CMD(Low4);
  }
}

void LCD_Write_Char(char Data)
{
  char Low4,High4;
  Low4 = Data & 0x0F;
  High4 = Data & 0xF0;

  LCD_xxx(static_cast<int>(High4>>4));
  LCD_xxx(static_cast<int>(Low4));
}

void LCD_Clear()
{
    LCD_CMD(0);
    LCD_CMD(1);
}

void LCD_String(string str)
{
    for(int i=0;i<str.length(); i++)
    {
        LCD_Write_Char(str[i]);
    }
}

void LCD_xxx(int val) //The main goal of that function is to change int value to binary form 
{
    // Select Data Register
    //RS = 1;
    tab[5] = 1;

    for(int i=0;i<4;i++)
    {
        (val & static_cast<int>(pow(2,i)))? tab[i] = 1 : tab[i] = 0;
    }
   
    // Send The EN Clock Signal
    //EN = 1;
    tab[4] = 1;
    send_8bit(fd,rq,tab); 
    usleep(LCD_EN_Delay);

    tab[4] = 0;
    send_8bit(fd,rq,tab);
    usleep(LCD_EN_Delay);

}

void LCD_DATA(int val) //The main goal of that function is to change int value to binary form 
{
    // Select Command Register
    //RS = 0;
    tab[5] = 0;

    for(int i=0;i<4;i++)
    {
        (val & static_cast<int>(pow(2,i)))? tab[i] = 1 : tab[i] = 0;
    }
   
    // Send The EN Clock Signal
    //EN = 1;
    tab[4] = 1;
    usleep(LCD_EN_Delay);
    send_8bit(fd,rq,tab); 
    usleep(LCD_EN_Delay);
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
  // The Init. Procedure //1st option
  
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