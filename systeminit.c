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
**  Filename :	systeminit.c
**  Abstract :	This file implements macro initial function.
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
#include "timer.h"

#define ENABLE_SERIAL	1

#if ENABLE_SERIAL 
#include "utils/serial.h"
#endif
/*
*******************************************************************************
**  MacroDefine
*******************************************************************************
*/
//following definitions should be set at the GPIO initialize. for NEC 0513-48
#define PM0_DEFAULT		0xFC
#define PM3_DEFAULT		0xF0
#define PM4_DEFAULT		0xFC
#define PM6_DEFAULT		0xF0
#define PM7_DEFAULT		0xC0
#define PM12_DEFAULT	0xE0
#define PM14_DEFAULT	0xFE

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function initializes the I/O port module.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
	input -> 1,  output ->0, up -> 1
GPIO Assign:
	P12.0 ->	 Power Key, input,  up
	P00 -> output


**-----------------------------------------------------------------------------
*/
void PORT_Init( void )
{
	/* Initialize the mode registers */
	PM0	= PM0_DEFAULT | ~(1 << 0);
	PM3 = PM3_DEFAULT;
	PM4 = PM4_DEFAULT;
	PM6 = PM6_DEFAULT;
	PM7 = PM7_DEFAULT;
	PM12 = PM12_DEFAULT | (1 << 0);
	PM14 = PM14_DEFAULT;

	/* Initialize the pull-up resistor option registers */
	PU12 = (1 << 0);

	/* Initialize the port registers */
	P0 = (1 << 0);
	P7 = 0x00;
	P12 = (1 << 0);

}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function initializes the external INT, including enable or disable,
**	priority setting
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void INT_Init( void )
{
	EGP = 0x00;
	EGN = 0x00;
	
	PMK0 = 1;		/* INTP0 disable */
	PIF0 = 0;		/* INTP0 IF clear */

	/* INTP0 pin setting */
	PM12 |= (1 << 0);

	EGP = 0x00;
	EGN = (1 << 0);

	PPR0 = 1;		/* INTP0 priority low */
	PMK0 = 0;		/* INTP0 enable */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function initializes watchdogtimer module.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void WDT_Init( void )
{
#if 0
	uint8 TempWDTM;
	TempWDTM = 0x60;
	/* Clock: low speed ring-OSC clock */
	/* Overflow time setting: frl/2^18 */
	SetIORBit(TempWDTM, 0x07);
	WDTM = TempWDTM;

#endif
}


void Clock_Init( void )
{
	/* Set fxh */
	OSCCTL = 0x00;
	MSTOP = 1;
	/* Set frh */
	RSTOP = 0;
	/* Set fprs */
	MCM = 0x00;

	/* Set fsub */
	XTSTART = 0;
	/* Set fcpu */
	PCC &= (uint8)~(0x07 | 0x10);
	PCC |= 0x01;
	/* Set frl */
	LSRSTOP = 1;
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function initializes every Macro.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void SystemInit( void )
{
	uint8 resetflag = RESF;
	Clock_Init();
	/* Port initiate */
	PORT_Init();
	/* INT initiate */
	INT_Init();
	/* WDT initiate */
	WDT_Init();
	
	/* UART6 initiate */
	#if ENABLE_SERIAL
	 UART6_Init();
	#endif

	/* AD initiate */
	AD_Init();
	/* TM00 initiate */
	TM50_Init();
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function initializes hardware settings.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void hdwinit( void )
{
	DI( );
	IMS = MEMORY_IMS_SET;
	IXS = MEMORY_IXS_SET;
	SystemInit( );
	EI( );
}
