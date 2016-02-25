/*
*******************************************************************************
**
**  This device driver was created by Applilet for the 78K0S/KB1+, 78K0S/KA1+,
**  78K0S/KY1+,78K0S/KU1+ 8-Bit Single-Chip Microcontrollers.
**
**  Copyright(C) NEC Electronics Corporation 2002-2006
**  All rights reserved by NEC Electronics Corporation.
**
**  This program should be used on your own responsibility.
**  NEC Electronics Corporation assumes no responsibility for any losses
**  incurred by customers or third parties arising from the use of this file.
**
**  Filename :	ad.h
**  Abstract :	This file implements device driver for AD module.
**  APIlib :	NEC78K0SKX1+.lib V1.70 [7 Dec. 2006]
**
**  Device :	uPD78F9221
**
**  Compiler :	NEC/CC78K0
**
*******************************************************************************
*/
#ifndef	_MDAD_
#define	_MDAD_
/*
*******************************************************************************
**  MacroDefine
*******************************************************************************
*/
typedef enum
{
	ADChanne5 = 0x05,
	ADChanne6 = 0x06, 
	ADChanne7 = 0x07,
	
	batteryVoltage = ADChanne5,
	batteryTemperature = ADChanne6,
	ADC_KEY = ADChanne7
}AdcChannel;

void AD_Init( void );
uint16 getAdcValue(AdcChannel channel);

#endif
