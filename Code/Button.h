/*
 * Button.h
 *
 * Created: 14/05/2024 04:22:01 م
 *  Author: FREINDS
 */ 

#ifndef BUTTON_H_
#define BUTTON_H_

void button_init(unsigned char portname, unsigned char pinnumber);
unsigned char button_read(unsigned char portname, unsigned char pinnumber);

#endif /* BUTTON_H_ */