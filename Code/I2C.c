/*
 * I2C.c
 *
 */ 
	#include <avr/io.h>
	#include "std_macros.h"
	#define F_CPU 8000000UL
	#define prescalar 1
	void I2C_Master_init(unsigned long SCL_Clock)
	{
		TWBR= (unsigned char) (((F_CPU/SCL_Clock)-16) / (2*prescalar));
		if (prescalar==1)
		{
			TWSR=0;
		}
		else if (prescalar==4)
		{
			TWSR=1;
		}
		else if (prescalar==16)
		{
			TWSR=2;
		}
		else if (prescalar==64)
		{
			TWSR=3;
		}
		SET_BIT(TWCR,TWEN);
	}
	
	void I2C_start(void)
	{
		SET_BIT(TWCR,TWSTA);
		while(READ_BIT(TWCR,TWINT)==0); //wait to finish 
		if((TWSR & 0xf8) == 0x08) // start condition has been transmitted
		{
			
		}
		else
		{
			
		}
	}
	
	void I2C_repeated_start(void)
	{
		SET_BIT(TWCR,TWSTA);
		while(READ_BIT(TWCR,TWINT)==0); //wait to finish
		if ((TWSR & 0XF8) == 0x10) // repeated start condition has been transmitted
		{
			
		}
		else
		{
			
		}
	}
	
	void I2C_write_address(unsigned char address)
	{
		TWDR=address;
		SET_BIT(TWCR,TWINT);
		while(READ_BIT(TWCR,TWINT)==0); //wait to finish 
		if ((TWSR & 0XF8) == 0x18) // SLA+write has been transmitted and ACK has been received 
		{
			
		}
		else
		{
			
		}
	}
	
	void I2C_write_data(unsigned char data)
	{
		TWDR=data;
		SET_BIT(TWCR,TWINT);
		while(READ_BIT(TWCR,TWINT)==0); //wait to finish
		if((TWSR & 0XF8) == 0x28) // data has been transmitted and ACK has been received
		{
			
		}
		else
		{
			
		}
	}
	
	void I2C_stop(void)
	{
		TWCR=(1<<TWEN)|(1<<TWSTO)|(1<<TWINT);
	}
	
	void I2C_set_address(unsigned char address)
	{
		TWAR=address;
	}
	
	unsigned char I2C_slave_read(void)
	{
		TWCR|=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);
		while(READ_BIT(TWCR,TWINT)==0);
		if ((TWSR & 0XF8) == 0x60) // own SLA+W has been received and ACK has been returned
		{
			
		}
		else
		{
			
		}
		SET_BIT(TWCR,TWINT);
		while(READ_BIT(TWCR,TWINT)==0);
		if ((TWSR & 0XF8) == 0x80) // previously addressed with own SLA+W : data has been received : ACK has been returned
		{
			
		}
		else
		{
			
		}
		return TWDR ;
	}
	
void I2C_write_address_and_read(unsigned char address)
{
	TWDR=address;
	TWCR=(1<<TWINT)|(1<<TWEN);
	while(READ_BIT(TWCR,TWINT)==0); //wait to finish
	if ((TWSR & 0XF8) == 0x40) // SLA+read has been transmitted and ACK has been received
	{
		
	}
	else
	{
		
	}
}	

unsigned char I2C_read_with_NACK(void)
{
	TWCR=(1<<TWINT)|(1<<TWEN);
	while(READ_BIT(TWCR,TWINT)==0);
	if ((TWSR & 0XF8) == 0x58) // data byte has been received and Not ACK has been returned
	{
		
	}
	else
	{
		
	}
	return TWDR ;
}

unsigned char I2C_read_with_ACK(void)
{
	TWCR=(1<<TWINT)|(1<<TWEN);
	while(READ_BIT(TWCR,TWINT)==0);
	if ((TWSR & 0XF8) == 0x58) // data byte has been received and Not ACK has been returned// need to edit hex number
	{
		
	}
	else
	{
		
	}
	return TWDR ;
}