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
**  Filename :	int_user.c
**  Abstract :	This file implements device driver for int module.
**  APIlib :	NEC78K0SKX1+.lib V1.70 [7 Dec. 2006]
**
**  Device :	uPD78F9221
**
**  Compiler :	NEC/CC78K0
**
*******************************************************************************
*/
#pragma	interrupt	INTP0	MD_INTP0
#pragma	interrupt	INTTM50	MD_INTTM50

/*
*******************************************************************************
**  Include files
*******************************************************************************
*/
#include "macrodriver.h"
#include "private.h"
/*
*******************************************************************************
**  MacroDefine
*******************************************************************************
*/


/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	INTP0 Interrupt service routine.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
__interrupt void MD_INTP0( void )
{
	/* TODO. Add user defined interrupt service routine */
}

__interrupt void MD_INTTM50(void)
{
	/* TODO. Add user defined interrupt service routine */
	gProject.timer1ms ++;
	gProject.timer100ms ++;

	if(gProject.delayTimer > 0)
		gProject.delayTimer --;
}



