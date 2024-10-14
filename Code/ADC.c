/*
 * ADC.c
  *
  * Created: 24/03/2024
  *  Author: Hazem Ragab Elsayed
  */
#include"DIO.h"
#include"std_macros.h"
#include<avr/io.h>

void ADC_init(void)
{
	DIO_vsetPINDir('A',0,0);
	SET_BIT(ADCSRA,ADEN);
	
	SET_BIT(ADCSRA,ADPS1);
	SET_BIT(ADCSRA,ADPS2);
	
	SET_BIT(ADMUX,REFS0);
	SET_BIT(ADMUX,REFS1);
	
}
unsigned short ADC_read(void)
{
	SET_BIT(ADCSRA,ADSC);
	while(1 == READ_BIT(ADCSRA,ADIF));
	unsigned short return_value = ADCL;
	return_value |= ADCH<<8;
	SET_BIT(ADCSRA,ADIF);
	
	return return_value;
}