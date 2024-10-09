/*
 * pushbutton.c
 *
 * Created: 14/05/2024 04:21:46 م
 *  Author: FREINDS
 */ 

#include"DIO.h"
#include"Button.h"

void button_init(unsigned char portname, unsigned char pinnumber)
{
	DIO_vsetPINDir(portname,pinnumber,0);
}
unsigned char button_read(unsigned char portname, unsigned char pinnumber)
{
	DIO_u8read(portname,pinnumber);
}
