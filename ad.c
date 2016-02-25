/*
*******************************************************************************
**
**  This device driver was created by Applilet for the 78K0S/KB1+, 78K0S/KA1+,
**  78K0S/KY1+,78K0S/KU1+ 8-Bit Single-Chip Microcontrollers.
**
**  Copyright(C) NEC Electronics Corporation 2002-2005
**  All rights reserved by NEC Electronics Corporation.
**
**  This program should be used on your own responsibility.
**  NEC Electronics Corporation assumes no responsibility for any losses
**  incurred by customers or third parties arising from the use of this file.
**
**  Filename :	ad.c
**  Abstract :	This file implements device driver for AD module.
**  APIlib :	NEC78K0SKX1+.lib V1.70 [7 Dec. 2006]
**
**  Device :	uPD78F9221
**
**  Compiler :	NEC/CC78K0
**
*******************************************************************************
*/
/*
*******************************************************************************
**  Include files
*******************************************************************************
*/
#include "macrodriver.h"
#include "ad.h"
/*
*******************************************************************************
**  MacroDefine
*******************************************************************************
*/

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function initializes the A/D converter.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void AD_Init( void )
{
	ADM = 0x00;	/* AD conversion disable */
	ADMK = 1;		/* INTAD disabled */
	ADIF = 0;		/* INTAD interrupt flag clear */
	ADPC = 0x04;
	/* AD pin setting */
	/* set 4 analog input */
	PM2 |= 0xF0;
	ADM |= 0x00 |0x00;
	ADS = 0x05;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function starts the A/D converter.
**
**  Parameters:
**	channel :	enum ADChannel
**
**  Returns:
**	MD_OK
**
**-----------------------------------------------------------------------------
*/
void AD_Start(void)
{
	ADCS = 1;		/* AD conversion enable */
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function returns conversion result in the buffer.
**
**  Parameters:
**	buffer :	the address to store conversion result
**
**  Returns:
**	MD_OK
**
**-----------------------------------------------------------------------------
*/
void AD_Read( uint8* buffer )
{
	*buffer = (uint8)( ADCR );
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function stops the A/D converter.
**
**  Parameters:
**	None
**
**  Returns:
**	MD_OK
**
**-----------------------------------------------------------------------------
*/
void AD_Stop( void )
{
	ADCS = 0;		/* AD conversion disable */
}

uint16 getAdcValue(AdcChannel channel)
{
	uint8 i = 8;
	uint16 result;

	ADS = channel;
	AD_Start();
	while(i--);
	result = (uint16)(ADCR >> 6);
	AD_Stop();
	
	return result;
}

