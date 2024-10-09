/*
 * Smart_Watch.c
 *
 * Created: 02/05/2024 03:56:18 ص
 *  Author: FREINDS
 */ 

#include <avr/io.h>
//http://www.avrfreaks.net/forum/ssd1306-lcd-initialization-commands


#include <math.h>
#include <string.h>
#include "I2C.h"
#include "ssd1306.h"
#include "timer.h"
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/pgmspace.h>
#define NOTPRESSED 0X01
#define PRESSED 0X00
unsigned short temp = 0;
volatile unsigned char seconds_counter = 0;
unsigned char x_of_temp = 0, y_of_temp = 3;
unsigned char x_of_input = 0, y_of_input = 3;
unsigned char x_of_time = 2, y_of_time = 1;
unsigned char minutes_counter = 0, hours_counter = 0;
char first_digit = NOTPRESSED, second_digit = NOTPRESSED,
 button1 = NOTPRESSED,button2 = NOTPRESSED,button3 = NOTPRESSED,button4 = NOTPRESSED,button5 = NOTPRESSED,button6=NOTPRESSED;


unsigned char _i2c_address;
unsigned char display_buffer[1024];
enum state{init,st1,st2,st3,st4,st5};
char state=init;



///////////////////////////////////////////////////////////
// Transfers the local buffer to the CGRAM in the SSD1306
void TransferBuffer()
{
	unsigned char j=0;
	
	// set the Column and Page addresses to 0,0
	setColAddress();
	setPageAddress();
	
	I2C_start();
	//I2C_Write(_i2c_address);
	I2C_write_address(_i2c_address);
	I2C_write_data(0X40); // data not command
	for(j=0;j<1024;j++)
	{
		I2C_write_data(display_buffer[j]);
	}

	I2C_stop();
}

void setup()
{
	// Initialze SSD1306 OLED display
	reset_display();              // Clear screen
	setXY(0,0);              // Set cursor position, start of line 0
	SendChar('1');
	setXY(1,0);              // Set cursor position, start of line 1
	//sendStr(minutes_counter);
	setXY(2,0);              // Set cursor position, start of line 2
	//sendStr();
	setXY(3,0);             // Set cursor position, line 2 10th character
	//sendStr("HHHH");
	setXY(7,15);             // Set cursor position, line 2 10th character
	//sendStr("H");
}
int main()
{
	
// 	output_high(PIN_C1);// keep power ON
// 
// 	output_float(PIN_C3);
// 	output_float(PIN_C4);

    timer2_overflow_init_interrupt();
	// fill buffer with something for test
	memset( display_buffer, 0X00, 1024); // tried other values
	
	_i2c_address = 0X78; // this works  or 0X3D does not
	I2C_Master_init(400000); 
	//InitializeDisplay();
	
	
        ADC_init();
		DIO_vsetPINDir('D',0,1);
		DIO_vsetPINDir('D',1,1);
		button_init('B',0);
		button_init('B',1);
		button_init('B',2);
		button_init('B',3);
		button_init('B',4);
		button_init('B',5);
		button_init('B',6);
		button_init('B',7);
		DIO_vconnectpullup('B',0,1);
		DIO_vconnectpullup('B',1,1);
		DIO_vconnectpullup('B',2,1);
		DIO_vconnectpullup('B',3,1);
		DIO_vconnectpullup('B',4,1);
		DIO_vconnectpullup('B',5,1);
		
		reset_display();
		_delay_ms(10);
		InitializeDisplay();
		DIO_write('D',0,1);
		tempchange:
		sendStrXY("TEMP:",x_of_temp,y_of_temp);
		
		while (1)
		{
			
			 do
			 {
				 temp = .25 * ADC_read();
				 if(temp < 10)
				 {
					 sendCharXY(temp+48,0,y_of_temp+5);
					 sendCharXY('C',0,y_of_temp+6);
					 sendStr("   ");
				 }
				 else if(temp < 100 && temp >= 10)
				 {
					 sendCharXY((temp/10)+48,0,y_of_temp+5);
					 sendCharXY((temp%10)+48,0,y_of_temp+6);
					 sendCharXY('C',0,y_of_temp+7);
					 sendStr(" ");
				 }
				 else if(temp<1000 && temp >= 100)
				 {
					 
					 sendCharXY((temp/100)+48,0,y_of_temp+5);
					 sendCharXY((temp / 10 % 10) + 48,0,y_of_temp+6);
					 sendCharXY((temp % 10) + 48,0,y_of_temp+7);
					 sendCharXY('C',0,y_of_temp+8);
				 }
				 else
				 {
					 
				 }
				 sendCharXY((hours_counter/10) + 48,x_of_time,y_of_temp);
				 sendCharXY((hours_counter%10) + 48,x_of_time,y_of_temp+1);
				 sendCharXY(':',x_of_time,y_of_temp+2);
				 sendCharXY((minutes_counter/10) + 48,x_of_time,y_of_temp + 3);
				 sendCharXY((minutes_counter%10) + 48,x_of_time,y_of_temp + 4);
				 sendCharXY(':',x_of_time,y_of_temp+5);
				 sendCharXY((seconds_counter/10) + 48,x_of_time,y_of_temp + 6);
				 sendCharXY((seconds_counter%10) + 48,x_of_time,y_of_temp + 7);
				 button1 = button_read('B',0);
				 button2 = button_read('B',1);
				 button3 = button_read('B',2);
				 button4 = button_read('B',3);
				 button5 = button_read('B',4);
				 button6 = button_read('B',5);
				 
				 
				 if(60 == seconds_counter)
				 {
					 minutes_counter++;
					 seconds_counter = 0;
					 DIO_write('D',0,1);
				 }
			
				 if(60 == minutes_counter)
				 {
					 hours_counter++;
					 minutes_counter = 0;
					 DIO_write('D',1,1);
				 }
				 if(24 <= hours_counter)
				 {
					 hours_counter = 0;
				 }
				 
				 
				 
			 } while (button1 == NOTPRESSED && button2 == NOTPRESSED && button3 == NOTPRESSED && button4 == NOTPRESSED 
			 && button5 == NOTPRESSED && button6 == NOTPRESSED);
			if(button1 == PRESSED)
			 {
				     clear_display();
				     while (button1 == PRESSED); // stay until it your button is not pressed
				     _delay_ms(250);
					 first_digit = 0;
					 second_digit = 0;
					 _delay_ms(300);
					 sendStrXY("hours:--",x_of_input,y_of_input);
					 do
					 {
					 sendCharXY(first_digit,x_of_input,y_of_input+6);
					 do{
						button3 = button_read('B',2);
						button4 = button_read('B',3);
					 }while(button3 == NOTPRESSED && button4 == NOTPRESSED);
					 
					 if(button3 == PRESSED)
					 {
						 first_digit++;
					 }
					 else if(button4 == PRESSED)
					 {
						 first_digit--;
					 }	 
					  while(button4 == PRESSED || button3 == PRESSED);
					  _delay_ms(250);
					 }while(button1 == NOTPRESSED);
					 while(button1 == NOTPRESSED);   // stay until it your button is not pressed
					 _delay_ms(250);
					 sendCharXY(second_digit,x_of_input,y_of_input+7);
					 do{
						 button3 = button_read('B',2);
						 button4 = button_read('B',3);
					 }while(button3 == NOTPRESSED && button4 == NOTPRESSED);
					 
					 if(button3 == PRESSED)
					 {
						 second_digit++;
					 }
					 else if(button4 == PRESSED)
					 {
						 second_digit--;
					 }
					  while(button4 == PRESSED || button3 == PRESSED);
					  _delay_ms(250);
				 }while(button1 == NOTPRESSED);
				     while(button1 == NOTPRESSED);   // stay until it your button is not pressed
				     _delay_ms(250);
					 
					 while (button1 == PRESSED); // stay until it your button is not pressed
					 _delay_ms(250);
					 first_digit = 0;
					 second_digit = 0;
					 _delay_ms(300);
					 sendStrXY("minutes:--",x_of_input,y_of_input);
					 do
					 {
						 sendCharXY(first_digit,x_of_input,y_of_input+8);
						 do{
							 button3 = button_read('B',2);
							 button4 = button_read('B',3);
						 }while(button3 == NOTPRESSED && button4 == NOTPRESSED);
						 
						 if(button3 == PRESSED)
						 {
							 first_digit++;
						 }
						 else if(button4 == PRESSED)
						 {
							 first_digit--;
						 }
						  while(button4 == PRESSED || button3 == PRESSED);
						  _delay_ms(250);
					 }while(button1 == NOTPRESSED);
					 while(button1 == NOTPRESSED);   // stay until it your button is not pressed
					 _delay_ms(250);
					 sendCharXY(second_digit,x_of_input,y_of_input+9);
					 do{
						 button3 = button_read('B',2);
						 button4 = button_read('B',3);
					 }while(button3 == NOTPRESSED && button4 == NOTPRESSED);
					 
					 if(button3 == PRESSED)
					 {
						 second_digit++;
					 }
					 else if(button4 == PRESSED)
					 {
						 second_digit--;
					 }
					 while(button4 == PRESSED || button3 == PRESSED);
					 _delay_ms(250);
				 }while(button1 == NOTPRESSED);
				 while(button1 == NOTPRESSED);   // stay until it your button is not pressed
				 _delay_ms(250);
					 
					 
					 
					 
				    /*  do{
						 second_digit = keypad_u8check_press();
					 }while(NOTPRESSED == second_digit);
					 _delay_ms(300);
					 LCD_vSend_char(second_digit);
					 _delay_ms(180);
					 hours_counter = (first_digit-48)*10 + (second_digit-48);
					 LCD_clearscreen();
					 LCD_vSend_string("minutes:--");
					 LCD_movecursor(1,9);
					 do{
						 first_digit = keypad_u8check_press();
						 
					 }while(NOTPRESSED == first_digit);
					 _delay_ms(300);
					 LCD_vSend_char(first_digit);
					 do{
						 second_digit = keypad_u8check_press();
					 }while(NOTPRESSED == second_digit);
					 _delay_ms(300);
					 LCD_vSend_char(second_digit);
					 _delay_ms(180);
					 minutes_counter = (first_digit-48)*10 + (second_digit-48);
					 LCD_clearscreen();
					 LCD_vSend_string("seconds:--");
					 LCD_movecursor(1,9);
					 do{
						 first_digit = keypad_u8check_press();
						 
					 }while(NOTPRESSED == first_digit);
					 _delay_ms(300);
					 LCD_vSend_char(first_digit);
					 do{
						 second_digit = keypad_u8check_press();
					 }while(NOTPRESSED == second_digit);
					 _delay_ms(300);
					 LCD_vSend_char(second_digit);
					 seconds_counter = (first_digit-48)*10 + (second_digit-48);
					 _delay_ms(180);
					 LCD_clearscreen();
				 
				 LCD_vSend_string("Press 1 to");
				 LCD_movecursor(2,1);
			
				 LCD_vSend_string("set time:");*/
						return 0;
			}				 		
	
			 
ISR(TIMER2_OVF_vect)
{
	seconds_counter++;
}