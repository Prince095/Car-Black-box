/*
 * File:   view_log.h
 * Author: Prince kumar
 *
 * Created on March 10, 2024, 5:33 PM
 */


#include <xc.h>
#include "menu.h"
#include "ext_eeprom.h"
#include "clcd.h"
#include "main.h"
#include "matrix_keypad.h"

extern unsigned char lap;
extern unsigned char temp_arr[];
extern char overflow;
unsigned char serial_no = 0;
extern char *event[];
extern unsigned char flag;
extern unsigned char once;
extern unsigned char time[9];

void view_log(unsigned char key)
{    
    clcd_print("Logs:", LINE1(0));
    
    static unsigned char address = 0, key2;
    static unsigned int long_press = 0;
    
    if(overflow && address == 0)
    {
        address = (lap * 5) + 5;
    }
    
    if(key == MK_SW6 && once)
    {
        key2 = key;
        long_press++;
        if(long_press > 200)
        {
            flag = MENU;
            long_press = 0;
            CLEAR_DISP_SCREEN;
            once = 0;
            return;
        }
    }
    else if(key == MK_SW5 && once)
    {
        if(overflow && serial_no < 9) 
        {
            serial_no++;
            address = (address + 5) % 50;
        }
        else if(serial_no < lap - 1)
        {
            serial_no++;
            address = address + 5;
        }
        once = 0;
    }
    else if(key2 == MK_SW6 && long_press <= 200 && long_press > 0)
    {
        if(serial_no > 0)
        {
            serial_no--;
            address = (address - 5) % 50;
        }
        long_press = 0;
        once = 0;
    }
    
    read_eeprom_data(address);
    clcd_putch(serial_no + '0', LINE2(0));
   
    clcd_print(temp_arr, LINE2(2));
  
    return;
}

