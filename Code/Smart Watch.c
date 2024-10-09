/*
 * Smart_Watch.c
 *
 * Created: 02/05/2024 03:56:18 Õ
 *  Author: Hazem Ragab Elsayed
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
#include<avr/interrupt.h>
#include <avr/pgmspace.h>
#define NOTPRESSED 0X01
#define PRESSED 0X00

#define X_OF_INPUT 3
#define Y_OF_INPUT 3
#define X_OF_TEMP 0
#define Y_OF_TEMP 3

unsigned char x_of_time = 3, y_of_time = 4;
unsigned char x_of_date = 5, y_of_date = 3;
unsigned short temp = 0;
unsigned char i = 0;
volatile unsigned char seconds_counter = 0;
unsigned char minutes_counter = 0, hours_counter = 0,day_counter = 16, month_counter = 5; 
unsigned short year_counter = 2024;
unsigned char first_digit = 0, second_digit = 0, third_digit = 0, fourth_digit = 0,changeplace = 0,tempcelsius = 1;
button1 = NOTPRESSED,button2 = NOTPRESSED,button3 = NOTPRESSED,button4 = NOTPRESSED,
button5 = NOTPRESSED,button6=NOTPRESSED, button7 = NOTPRESSED ,button8 = NOTPRESSED;

unsigned char _i2c_address;

int main()
{
	  
      
	    I2C_Master_init(100000); 
	    timer2_overflow_init_interrupt();
	    _i2c_address = 0X78; // this works  or 0X3D does not
        ADC_init();
		button_init('B',0);
		button_init('B',1);
		button_init('B',2);
		button_init('B',3);
		
		button_init('B',6);
		
		DIO_vconnectpullup('B',0,1);
		DIO_vconnectpullup('B',1,1);
		DIO_vconnectpullup('B',2,1);
		DIO_vconnectpullup('B',3,1);
		DIO_vconnectpullup('B',6,1);
		
		InitializeDisplay();
		
		
		// commented because of memory overflow
		 
		button_init('B',4);        
		button_init('B',5);
		button_init('B',7);
		DIO_vconnectpullup('B',4,1);
		DIO_vconnectpullup('B',5,1);
		DIO_vconnectpullup('B',7,1); 
		
		//reset_display();
		//_delay_ms(10);
		
		
		tempchange:
		clear_display();
		sendStrXY("TEMP:",X_OF_TEMP,Y_OF_TEMP);
		while (1){
			do
			 {  
				 
				 if(tempcelsius == 1)
				 {
					 temp = .25 * ADC_read();
				 }
				 else if(tempcelsius == 0)
				 {
					 temp = .25 * ADC_read() * ((float)9/5) + 32;
				 }
				 else
				 {
					 
				 }
				 
				 if(temp < 10)
				 {
					 sendCharXY(temp+48,X_OF_TEMP,Y_OF_TEMP+5);
					 if(tempcelsius == 1)
					 {
						 sendCharXY('C',X_OF_TEMP,Y_OF_TEMP+6);
					 }
					 else if(tempcelsius == 0)
					 {
						 sendCharXY('F',X_OF_TEMP,Y_OF_TEMP+6);
					 }
					 else
					 {
						 
					 }
					 
					 sendStr("   ");
				 }
				 else if(temp < 100 && temp >= 10)
				 {
					 sendCharXY((temp/10)+48,X_OF_TEMP,Y_OF_TEMP + 5);
					 sendCharXY((temp%10)+48,X_OF_TEMP,Y_OF_TEMP + 6);
					  if(tempcelsius == 1)
					  {
						  sendCharXY('C',X_OF_TEMP,Y_OF_TEMP + 7);
					  }
					  else if(tempcelsius == 0)
					  {
						  sendCharXY('F',X_OF_TEMP,Y_OF_TEMP + 7);
					  }
					 
					 sendStr(" ");
				 }
				 else if(temp<1000 && temp >= 100)
				 {
					 
					 sendCharXY((temp/100)+48,X_OF_TEMP,Y_OF_TEMP + 5);
					 sendCharXY((temp / 10 % 10) + 48,X_OF_TEMP,Y_OF_TEMP + 6);
					 sendCharXY((temp % 10) + 48,X_OF_TEMP,Y_OF_TEMP + 7);
					 if(tempcelsius == 1)
					 {
						 sendCharXY('C',X_OF_TEMP,Y_OF_TEMP + 8);
					 }
					 else if(tempcelsius == 0)
					 {
						 sendCharXY('F',X_OF_TEMP,Y_OF_TEMP + 8);
					 }
					 
				 }
				 else
				 {
					 
				 }
				 if(60 <= seconds_counter)
				 {
					 minutes_counter++;
					 seconds_counter = 0;
				 } 
				 if(60 <= minutes_counter)
				 {
					 hours_counter++;
					 minutes_counter = 0;
				 }
				 if(24 <= hours_counter)
				 {
					 hours_counter = 0;
					 day_counter++;
				 }
				 if(day_counter >= 31 )
				 {
					 day_counter = 1;
					 month_counter++;
				 }
				 if(month_counter >= 12)
				 {
					 month_counter = 1;
					 year_counter++;
				 }
				 
				 
				 // time displaying
				 sendCharXY((hours_counter/10) + 48,x_of_time,y_of_time);
				 sendCharXY((hours_counter%10) + 48,x_of_time,y_of_time+1);
				 sendCharXY(':',x_of_time,y_of_time+2);
				 sendCharXY((minutes_counter/10) + 48,x_of_time,y_of_time + 3);
				 sendCharXY((minutes_counter%10) + 48,x_of_time,y_of_time + 4);
				 sendCharXY(':',x_of_time,y_of_time+5);
				 sendCharXY((seconds_counter/10) + 48,x_of_time,y_of_time + 6);
				 sendCharXY((seconds_counter%10) + 48,x_of_time,y_of_time + 7);
				 
				 // date displaying
				 sendCharXY((year_counter/1000) + 48,x_of_date,y_of_date);
				 sendCharXY(((year_counter/100) % 10) + 48,x_of_date,y_of_date+1);
				 sendCharXY(((year_counter/10) % 10) + 48,x_of_date,y_of_date+2);
				 sendCharXY((year_counter % 10) + 48,x_of_date,y_of_date+3);
				 sendCharXY('/',x_of_date,y_of_date+4);
				 sendCharXY((month_counter/10) + 48,x_of_date,y_of_date + 5);
				 sendCharXY((month_counter%10) + 48,x_of_date,y_of_date + 6);
				 sendCharXY('/',x_of_date,y_of_date+7);
				 sendCharXY((day_counter/10) + 48,x_of_date,y_of_date + 8);
				 sendCharXY((day_counter%10) + 48,x_of_date,y_of_date + 9);
				 
				 
				 button1 = button_read('B',0);
				 button2 = button_read('B',1);
				 button3 = button_read('B',2);
				 button4 = button_read('B',3);
				 button7 = button_read('B',6);
				 
				 
				 // I decided to delete some feutures because of memory overflow
				
				 button5 = button_read('B',4);
				 button6 = button_read('B',5);
				 button8 = button_read('B',7);
				 
				while (button_read('B',4) == PRESSED && button_read('B',5) == PRESSED && button_read('B',7) == PRESSED);
				
				if(button5 == PRESSED && tempcelsius == 1)
				{
					tempcelsius = 0;
				}
				else if(button5 == PRESSED && tempcelsius == 0)
				{
					tempcelsius = 1;
				}
				
				
				 if(button8 == PRESSED && changeplace == 0)
				 {
					changeplace = 1;
				 }
				 else if(button8 = PRESSED && changeplace == 1)
				 {
					 changeplace = 0;
				 }
				 _delay_ms(300);
				 if(button_read('B',6) == PRESSED && y_of_time > 0 && y_of_date > 0 && changeplace == 1)
				 {
					 while(button_read('B',6) == PRESSED);
					 _delay_ms(250);
					 sendCharXY(' ',x_of_time,y_of_time+7);
					 sendCharXY(' ',x_of_date,y_of_date+9);
					 y_of_time--;
					 y_of_date--;
				 }
				 if(button_read('B',5) == PRESSED && y_of_time < 7 && y_of_date < 7 && changeplace == 1)
				 {
					 while(button_read('B',5) == PRESSED);
					 _delay_ms(250);
					 
					 sendCharXY(' ',x_of_time,y_of_time);
					 sendCharXY(' ',x_of_date,y_of_date);
					 y_of_time++;
					 y_of_date++;
				 }
				 if(button_read('B',2) == PRESSED && x_of_time > 1 && x_of_date > 1 && changeplace == 1)
				 {
					 while(button_read('B',2) == PRESSED);
					 _delay_ms(250);
					 // add loops here
					 for(i = y_of_date; i <= y_of_date + 9;i++)
					 {
						 sendCharXY(' ',x_of_time,i);
						 sendCharXY(' ',x_of_date,i);
					 }
					 x_of_time--;
					 x_of_date--;
				 }
				 if(button_read('B',3) == PRESSED && x_of_time < 7 && x_of_date < 7 && changeplace == 1)
				 {
					 while(button_read('B',3) == PRESSED);
					 _delay_ms(250);
				// add loops here
				for(i = y_of_date; i <= y_of_date + 9;i++)
				{
					sendCharXY(' ',x_of_time,i);
					sendCharXY(' ',x_of_date,i);
				}
					 x_of_time++;
					 x_of_date++;
				 }
				 					 									 
				           
			 } while (button1 == NOTPRESSED && button2 == NOTPRESSED);
			if(button1 == PRESSED)
			 {
				     clear_display();
				     while (button_read('B',0) == PRESSED); // stay until it your button is not pressed
				    // _delay_ms(250);
					 first_digit = 0;
					 second_digit = 0;
					 //_delay_ms(300);
					 sendStrXY("hours:--",X_OF_INPUT,Y_OF_INPUT + 1);
					 firstdigithours:
					 do
					 { 
					 sendCharXY(first_digit + 48,X_OF_INPUT,Y_OF_INPUT+7);
					 do{
						 button1 = button_read('B',0);
						 button3 = button_read('B',2);
						 button4 = button_read('B',3);
					 }while(button3 == NOTPRESSED && button4 == NOTPRESSED && button1 == NOTPRESSED);
					 while(button_read('B',3) == PRESSED || button_read('B',2) == PRESSED);
					// _delay_ms(250);
					 if(button3 == PRESSED && ((first_digit < 2 && second_digit < 5) ||  first_digit < 1 ))
					 {
						 first_digit++;
					 }
					 else if(button4 == PRESSED && first_digit > 0)
					 {
						 first_digit--;
					 }	 
					 }while(button_read('B',0) == NOTPRESSED);
					 while(button_read('B',0) == PRESSED);   // stay until it your button is not pressed
					// _delay_ms(250);
					 do{
					 sendCharXY(second_digit + 48,X_OF_INPUT,Y_OF_INPUT+8);
					 do{
						 button1 = button_read('B',0);
						 button3 = button_read('B',2);
						 button4 = button_read('B',3);
						 button7 = button_read('B',6);
					 }while(button3 == NOTPRESSED && button4 == NOTPRESSED && button1 == NOTPRESSED && button7 == NOTPRESSED);
					 while(button_read('B',3) == PRESSED || button_read('B',2) == PRESSED || button_read('B',6) == PRESSED);
					// _delay_ms(250);
					  if(button3 == PRESSED && ((first_digit == 2 && second_digit < 4) || (first_digit < 2 && second_digit < 9)))
					  {
						  second_digit++;
					  }
					  else if(button4 == PRESSED && second_digit > 0)
					  {
						  second_digit--;
					  }
					  else if(button7 == PRESSED)
					  {
						  sendCharXY('-',X_OF_INPUT,Y_OF_INPUT+8);
						  goto firstdigithours;
					  }						  
				    }while(button_read('B',0) == NOTPRESSED);
					hours_counter = (first_digit * 10) + second_digit;
				    while(button_read('B',0) == PRESSED);   // stay until it your button is not pressed
				   // _delay_ms(250);
					
					 clear_display();
					 first_digit = 0;
					 second_digit = 0;
					// _delay_ms(300);
					 sendStrXY("minutes:--",X_OF_INPUT,Y_OF_INPUT);	
					 firstdigitminutes: 
					 do
					 {
						 
						 sendCharXY(first_digit + 48,X_OF_INPUT,Y_OF_INPUT+8);
						 do{
							 button1 = button_read('B',0);
							 button3 = button_read('B',2);
							 button4 = button_read('B',3);
						 }while(button3 == NOTPRESSED && button4 == NOTPRESSED && button1 == NOTPRESSED);
						 while(button_read('B',3) == PRESSED || button_read('B',2) == PRESSED);
						// _delay_ms(250);
						  if(button3 == PRESSED && first_digit < 5)
						  {
							  first_digit++;
						  }
						  else if(button4 == PRESSED && first_digit > 0)
						  {
							  first_digit--;
						  }
					 }while(button1 == NOTPRESSED);
					 while(button_read('B',0) == PRESSED);   // stay until it your button is not pressed
					// _delay_ms(250);
					do{
					 
					 sendCharXY(second_digit + 48,X_OF_INPUT,Y_OF_INPUT+9);
					do{
						button1 = button_read('B',0);
						button3 = button_read('B',2);
						button4 = button_read('B',3);
						button7 = button_read('B',6);
					}while(button3 == NOTPRESSED && button4 == NOTPRESSED && button1 == NOTPRESSED && button7 == NOTPRESSED);
					 while(button_read('B',3) == PRESSED || button_read('B',2) == PRESSED || button_read('B',6) == PRESSED);
					// _delay_ms(250);
					  if(button3 == PRESSED && second_digit < 9)
					  {
						  second_digit++;
					  }
					  else if(button4 == PRESSED && second_digit > 0)
					  {
						  second_digit--;
					  }
					  else if(button7 == PRESSED)
					  {
						  sendCharXY('-',X_OF_INPUT,Y_OF_INPUT+9);
						  goto firstdigitminutes;
					  }
				 }while(button1 == NOTPRESSED);
				 minutes_counter = (first_digit * 10) + second_digit;
				 while(button_read('B',0) == PRESSED);   // stay until it your button is not pressed
				// _delay_ms(250);
				 clear_display();
				 first_digit = 0;
				 second_digit = 0;
				// _delay_ms(300);
				 sendStrXY("seconds:--",X_OF_INPUT,Y_OF_INPUT);
				 firstdigitseconds:
				 do{
					  sendCharXY(first_digit + 48,X_OF_INPUT,Y_OF_INPUT+8);
					 do{
						 button1 = button_read('B',0);
						 button3 = button_read('B',2);
						 button4 = button_read('B',3);
					 }while(button3 == NOTPRESSED && button4 == NOTPRESSED && button1 == NOTPRESSED);
					 while(button_read('B',3) == PRESSED || button_read('B',2) == PRESSED);
					// _delay_ms(250);
					  if(button3 == PRESSED && first_digit < 5)
					  {
						  first_digit++;
					  }
					  else if(button4 == PRESSED && first_digit > 0)
					  {
						  first_digit--;
					  }
				}while(button1 == NOTPRESSED);
				while(button_read('B',0) == PRESSED);   // stay until it your button is not pressed
			//	_delay_ms(250);
				do{
				 sendCharXY(second_digit + 48,X_OF_INPUT,Y_OF_INPUT+9);
				do{
					button1 = button_read('B',0);
					button3 = button_read('B',2);
					button4 = button_read('B',3);
					button7 = button_read('B',6);
					
				}while(button3 == NOTPRESSED && button4 == NOTPRESSED && button1 == NOTPRESSED && button7 == NOTPRESSED);
				 while(button_read('B',3) == PRESSED || button_read('B',2) == PRESSED || button_read('B',6) == PRESSED);
			//	 _delay_ms(250);
				  if(button3 == PRESSED && second_digit < 9)
				  {
					  second_digit++;
				  }
				  else if(button4 == PRESSED && second_digit > 0)
				  {
					  second_digit--;
				  }
				  else if(button7 == PRESSED)
				  {
					  sendCharXY('-',X_OF_INPUT,Y_OF_INPUT+9);
					  goto firstdigitseconds;  
				  }
				 }while(button_read('B',0) == NOTPRESSED);
				 seconds_counter = (first_digit * 10) + second_digit;
			 while(button_read('B',0) == PRESSED);   // stay until it your button is not pressed
			// _delay_ms(250);
			 goto tempchange;
				}		 
				if(button2 == PRESSED)
				{
					clear_display();
					while (button_read('B',1) == PRESSED); // stay until it your button is not pressed
					//_delay_ms(250);
					
					first_digit = 2;
					second_digit = 0;
					third_digit = 2;
					fourth_digit = 4;
				//	_delay_ms(300);
					sendStrXY("year:----",X_OF_INPUT,Y_OF_INPUT + 1);
					firstdigityear:
					do
					{
						
						sendCharXY(first_digit + 48,X_OF_INPUT,Y_OF_INPUT + 6);
						do{
							button2 = button_read('B',1);
							button3 = button_read('B',2);
							button4 = button_read('B',3);
						}while(button3 == NOTPRESSED && button4 == NOTPRESSED && button2 == NOTPRESSED);
						while(button_read('B',3) == PRESSED || button_read('B',2) == PRESSED);
						//_delay_ms(250);
						if(button3 == PRESSED && first_digit < 2)
						{
							first_digit++;
						}
						else if(button4 == PRESSED && first_digit > 1)
						{
							first_digit--;
						}

					}while(button_read('B',1) == NOTPRESSED);
					while(button_read('B',1) == PRESSED);   // stay until it your button is not pressed
				//	_delay_ms(250);
					seconddigityear:
					do{
						
						sendCharXY(second_digit + 48,X_OF_INPUT,Y_OF_INPUT + 7);
						do{
							button2 = button_read('B',1);
							button3 = button_read('B',2);
							button4 = button_read('B',3);
							button7 = button_read('B',6);
						}while(button3 == NOTPRESSED && button4 == NOTPRESSED && button2 == NOTPRESSED && button7 == NOTPRESSED);
						while(button_read('B',3) == PRESSED || button_read('B',2) == PRESSED || button_read('B',6) == PRESSED);
					//	_delay_ms(250);
						if(button3 == PRESSED && second_digit < 9)
						{
							second_digit++;
						}
						else if(button4 == PRESSED && second_digit > 0)
						{
							second_digit--;
						}
						else if(button7 == PRESSED)
						{
							sendCharXY('-',X_OF_INPUT,Y_OF_INPUT + 7);
							goto firstdigityear;
						}
						
					}while(button_read('B',1) == NOTPRESSED);
					while(button_read('B',1) == PRESSED);   // stay until it your button is not pressed
				//	_delay_ms(250);
					thirddigityear:
					do
					{
						
						sendCharXY(third_digit + 48,X_OF_INPUT,Y_OF_INPUT + 8);
						do{
							button2 = button_read('B',1);
							button3 = button_read('B',2);
							button4 = button_read('B',3);
							button7 = button_read('B',6);
						}while(button3 == NOTPRESSED && button4 == NOTPRESSED && button2 == NOTPRESSED && button7 == NOTPRESSED);
						while(button_read('B',3) == PRESSED || button_read('B',2) == PRESSED || button_read('B',6) == PRESSED);
					//	_delay_ms(250);
						if(button3 == PRESSED && third_digit < 9)
						{
							third_digit++;
						}
						else if(button4 == PRESSED && third_digit > 0)
						{
							third_digit--;
						}
						else if(button7 == PRESSED)
						{
							sendCharXY('-',X_OF_INPUT,Y_OF_INPUT + 8);
							goto seconddigityear;
						}
					}while(button_read('B',1) == NOTPRESSED);
					while(button_read('B',1) == PRESSED);   // stay until it your button is not pressed
				//	_delay_ms(250);
					
					do
					{
						sendCharXY(fourth_digit + 48,X_OF_INPUT,Y_OF_INPUT + 9);
						do{
							button2 = button_read('B',1);
							button3 = button_read('B',2);
							button4 = button_read('B',3);
							button7 = button_read('B',6);
						}while(button3 == NOTPRESSED && button4 == NOTPRESSED && button2 == NOTPRESSED && button7 == NOTPRESSED);
						while(button_read('B',3) == PRESSED || button_read('B',2) == PRESSED || button_read('B',6) == PRESSED);
					//	_delay_ms(250);
						if(button3 == PRESSED && fourth_digit < 9)
						{
							fourth_digit++;
						}
						else if(button4 == PRESSED && fourth_digit > 0)
						{
							fourth_digit--;
						}
						else if(button7 == PRESSED)
						{
							sendCharXY('-',X_OF_INPUT,Y_OF_INPUT + 9);
							goto thirddigityear;
						}
						
					}while(button_read('B',1) == NOTPRESSED);
					while(button_read('B',1) == PRESSED);   // stay until it your button is not pressed
					//_delay_ms(250);
					year_counter = (first_digit * 1000) + (second_digit * 100) + (third_digit * 10) + fourth_digit;
					
					
					clear_display();
					first_digit = 0;
					second_digit = 1;
					//_delay_ms(300);
					sendStrXY("month:--",X_OF_INPUT,Y_OF_INPUT + 1);
					do
					{
						firstdigitmonth:
						sendCharXY(first_digit + 48,X_OF_INPUT,Y_OF_INPUT + 7);
						do{
							button2 = button_read('B',1);
							button3 = button_read('B',2);
							button4 = button_read('B',3);
						}while(button3 == NOTPRESSED && button4 == NOTPRESSED && button2 == NOTPRESSED);
						while(button_read('B',3) == PRESSED || button_read('B',2) == PRESSED);
					//	_delay_ms(250);
						if(button3 == PRESSED && first_digit < 1)
						{
							first_digit++;
						}
						else if(button4 == PRESSED && ((first_digit > 0 && second_digit > 0) || first_digit > 1))
						{
							first_digit--;
						}
						
					}while(button2 == NOTPRESSED);
					while(button_read('B',1) == PRESSED);   // stay until it your button is not pressed
					//_delay_ms(250);
					do{
						
						sendCharXY(second_digit + 48,X_OF_INPUT,Y_OF_INPUT + 8);
						do{
							button2 = button_read('B',1);
							button3 = button_read('B',2);
							button4 = button_read('B',3);
							button7 = button_read('B',6);
						}while(button3 == NOTPRESSED && button4 == NOTPRESSED && button2 == NOTPRESSED && button7 == NOTPRESSED);
						while(button_read('B',3) == PRESSED || button_read('B',2) == PRESSED || button_read('B',6) == PRESSED);
					//	_delay_ms(250);
						if(button3 == PRESSED && ((second_digit < 9 && first_digit == 0) || (second_digit<2 && first_digit == 1)) )
						{
							second_digit++;
						}
						else if(button4 == PRESSED && ((second_digit > 0 && first_digit == 1) || (second_digit > 1 && first_digit == 0)))
						{
							second_digit--;
						}
						else if(button7 == PRESSED)
						{
							sendCharXY('-',X_OF_INPUT,Y_OF_INPUT + 8);
							goto firstdigitmonth;
						}
					}while(button2 == NOTPRESSED);
					month_counter = (first_digit * 10) + second_digit;
					while(button_read('B',1) == PRESSED);   // stay until it your button is not pressed
				//	_delay_ms(250);
					clear_display();
					first_digit = 0;
					second_digit = 1;
				//	_delay_ms(300);
					sendStrXY("day:--",X_OF_INPUT,Y_OF_INPUT + 2);
					do{
						firstdigitday:
						sendCharXY(first_digit + 48,X_OF_INPUT,Y_OF_INPUT + 6);
						do{
							button2 = button_read('B',1);
							button3 = button_read('B',2);
							button4 = button_read('B',3);
						}while(button3 == NOTPRESSED && button4 == NOTPRESSED && button2 == NOTPRESSED);
						while(button_read('B',3) == PRESSED || button_read('B',2) == PRESSED);
					//	_delay_ms(250);
						if(button3 == PRESSED && first_digit < 3)
						{
							first_digit++;
						}
						else if(button4 == PRESSED && ((first_digit > 0 && second_digit == 1) || (first_digit > 1)))
						{
							first_digit--;
						}
					}while(button2 == NOTPRESSED);
					while(button_read('B',1) == PRESSED);   // stay until it your button is not pressed
				//	_delay_ms(250);
					do{
						sendCharXY(second_digit + 48,X_OF_INPUT,Y_OF_INPUT + 7);
						do{
							button2 = button_read('B',1);
							button3 = button_read('B',2);
							button4 = button_read('B',3);
							button7 = button_read('B',6);
						}while(button3 == NOTPRESSED && button4 == NOTPRESSED && button2 == NOTPRESSED && button7 == NOTPRESSED);
						while(button_read('B',3) == PRESSED || button_read('B',2) == PRESSED || button_read('B',6) == PRESSED);
				//		_delay_ms(250);
						if(button3 == PRESSED && ((second_digit < 9 && first_digit < 3) || (second_digit<1 && first_digit == 3)))
						{
							second_digit++;
						}
						else if(button4 == PRESSED && ((second_digit > 0 && first_digit > 0) || (second_digit > 1 && first_digit == 0)))
						{
							second_digit--;
						}
						else if(button7 == PRESSED)
						{
							sendCharXY('-',X_OF_INPUT,Y_OF_INPUT + 7);
							goto firstdigitday;
						}
					}while(button_read('B',1) == NOTPRESSED);
					day_counter = (first_digit * 10) + second_digit;
					while(button_read('B',1) == PRESSED);   // stay until it your button is not pressed
			//		_delay_ms(250);
					goto tempchange;
				}
				}			 					
return 0;
}									 		

ISR(TIMER2_OVF_vect)
{
	seconds_counter++;
}