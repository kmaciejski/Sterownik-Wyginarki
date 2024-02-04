
/** Put this in the src folder **/

#include "i2c-lcd.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly

#define SLAVE_ADDRESS_LCD 0x7E // change this according to ur setup


/*
 * 					LCD driver
 */
void lcd_send_cmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_init (void)
{
	uint8_t i=0;
	HAL_Delay(100);
	for(i=0;i<3;i++)//sending 3 times: select 4-bit mode
	{
		lcd_send_cmd(0x03);
		HAL_Delay(45);
	}
	lcd_send_cmd (0x02);
	HAL_Delay(100);
	lcd_send_cmd (0x28);
	HAL_Delay(1);
	lcd_send_cmd (0x0c);
	HAL_Delay(1);
	lcd_send_cmd (0x80);
	HAL_Delay(1);

	lcd_clear();

}

void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}

void lcd_xy(uint8_t x, uint8_t y)
{

	switch(y)
	{
		case 0:
			lcd_send_cmd( LCDC_SET_DDRAM | (LCD_LINE1 + x) );
			break;

		case 1:
			lcd_send_cmd( LCDC_SET_DDRAM | (LCD_LINE2 + x) );
			break;
	}

}

void lcd_clear (void){ // lcd clear

	lcd_send_cmd(LCD_CLEAR);
	HAL_Delay(2);

}

/*
 * 					LCD printf
 */
void vprint(const char *fmt, va_list argp)
{
    char string[200];
    int i = 0, len;
    if(0 < vsprintf(string,fmt,argp)) // build string
    {
    	len = strlen(string);
        while(i < len)
        	lcd_send_data(string[i++]);
    }
}

void lcd_printf(const char *fmt, ...) // custom printf() function
{
    va_list argp;
    va_start(argp, fmt);
    vprint(fmt, argp);
    va_end(argp);
}

