/*
*******************************************************************************
**
**  This device driver was created by Applilet for the 78K0S/KB1+, 78K0S/KA1+,
**  78K0S/KY1+,78K0S/KU1+ 8-Bit Single-Chip Microcontrollers
**
**  Copyright(C) NEC Electronics Corporation 2002-2006
**  All rights reserved by NEC Electronics Corporation.
**
**  This program should be used on your own responsibility.
**  NEC Electronics Corporation assumes no responsibility for any losses
**  incurred by customers or third parties arising from the use of this file.
**
**  Filename :	timer.c
**  Abstract :	This file implements device driver for the timer module.
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
#include "timer.h"

void TM50_OnOff(uint8 on)
{
	if(on)
	{
		TMMK50	= 0;		/* INTTM50 interrupt enabled */
		TCE50 = 1;			/* count operation start */
	}
	else
	{
		TCE50 = 0;			/* TM50 counter disable */
		TMMK50	= 1;		/* INTTM50 interrupt disabled */
		TMIF50	= 0;		/* clear interrupt flag */
	}
}


/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function initiate TM50.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void TM50_Init()
{
	TCE50 = 0;			/* TM50 counter disable */
	TMMK50	= 1;		/* INTTM50 interrupt disabled */
	TMIF50	= 0;		/* clear interrupt flag */
	TMPR50	= 1;		/* set low priority level */

	TCL50 = 0x05;

	 /* TM50 interval mode, 500us interval timeout */
	CR50 = 62;  
	TMC50 = 0x02;	
	
	TM50_OnOff(ON);
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function change TM50 compare value.
**
**  Parameters:
**	value:	 value for compare register
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void TM50_ChangeTimerCondition(uint8 value)
{
	CR50 = value;
}

