#include "own_gpio.h"
#include <string>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


/*      LAYOUT OF THE 4BIT DATA ("tab" array)
 *
 *      PO4 PO5 PO6 PO7 EN RS
 *
 */

using namespace std;

void LCD_Init();
void LCD_CMD(unsigned char command); //This function send data to lcd in 4bit mode
void LCD_String(string str);
void LCD_xxx(int val);  //The main goal of that function is to change int value to binary form
void LCD_Clear();
void LCD_Set_Cursor(int row); //Set line (r=1 line=1, r=2 line=2, the bound of column is 1-16)
void LCD_Blinking_Cursor(bool state); //Turn on(1) or turn off(0) blinking cursor



#define LCD_EN_Delay 500
#define LCD_EN_Pulse 500

bool tab[8]{0,0,0,0,0,0,0,0};
int fd;

//these structures are needed to all operations involved with gpio 
struct gpiochip_info info;
struct gpioline_info line_info;
struct gpiohandle_request rq;

int main(int argc, char *argv[])
{
    fd = open_file();
    string str;

    if(fd > -1)
    {
       if(init_chip(fd,info) > -1)
        {
            init_8pins(fd,rq);

            LCD_Init();
            /*
            for(int i=0;i<1;)
            {
                //send data to first line
                LCD_Set_Cursor(1);
                getline(cin,str,'\n');
                LCD_Clear();
                LCD_String(str);

                //send data to second line
                LCD_Set_Cursor(2);
                getline(cin,str,'\n');
                LCD_String(str);

                if(str[0] == 'q' && str.length() == 1)
                {
                    break;
                }
            }
            */

            for(int i=0;i<1;)
            {
                //send data to first line
                LCD_Set_Cursor(1);
                str = system("vcgencmd measure_temp");
                LCD_Clear();
                LCD_String(str);

                getline(cin,str,'\n');
                if(str[0] == 'q' && str.length() == 1)
                {
                    break;
                }
            }

            
            cout << "Closing the program..." << endl;
            close_file(fd);
        }
        

    }
    return 0;
}


void LCD_Blinking_Cursor(bool state)
{
    if(state)
    {
        LCD_CMD(0x00);
        LCD_CMD(0x0F);
    }
    else
    {
        LCD_CMD(0x00);
        LCD_CMD(0x0C);
    }
}

void LCD_Set_Cursor(int row)
{
    unsigned char Temp,Low4,High4;
    if(row == 1)
    {
        LCD_CMD(0x80 >> 4);
        LCD_CMD(0x00);
    }

    if(row == 2)
    {
        LCD_CMD(0xC0 >> 4);
        LCD_CMD(0x00);
    }
}

void LCD_Write_Char(char Data)
{
    char Low4,High4;
    High4 = Data & 0xF0;
    Low4 = Data & 0x0F;


    LCD_xxx(static_cast<int>(High4>>4));
    LCD_xxx(static_cast<int>(Low4));
}

void LCD_Clear()
{
    string str = "                "; // 16x " "
    LCD_Set_Cursor(1);
    LCD_String(str);

    LCD_Set_Cursor(2);
    LCD_String(str);

    LCD_Set_Cursor(1);
}

void LCD_String(string str)
{
    for(int i=0;i<str.length(); i++)
    {
        LCD_Write_Char(str[i]);
    }
}

void LCD_xxx(int val)
{
    // Select Data Register
    tab[5] = 1;

    //Formating data to specyfic index
    for(int i=0;i<4;i++)
    {
        (val & static_cast<int>(pow(2,i)))? tab[i] = 1 : tab[i] = 0;
    }
   
    // Send The EN Clock Signal
    tab[4] = 1;
    send_8bit(fd,rq,tab); 
    usleep(LCD_EN_Delay);

    // Stop Sending The EN Clock Signal
    tab[4] = 0;
    send_8bit(fd,rq,tab);
    usleep(LCD_EN_Delay);

}

void LCD_CMD(unsigned char command) //4bit mode
{
    // Select Command Register
    tab[5] = 0;

    for(int i=0;i<4;i++)
    {
        (command & static_cast<int>(pow(2,i)))? tab[i] = 1 : tab[i] = 0;
    }

    // Send The EN Clock Signal
    tab[4] = 1;
    send_8bit(fd,rq,tab);
    usleep(LCD_EN_Pulse);

    // Stop Sending The EN Clock Signal
    tab[4] = 0;
    send_8bit(fd,rq,tab);
    usleep(LCD_EN_Pulse);
}

void LCD_Init()
{
    // The Init. Procedure //1st option
  
    LCD_CMD(0x00);
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

    LCD_CMD(0x00);
    LCD_CMD(0x01);
    
}
