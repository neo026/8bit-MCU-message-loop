/*
*******************************************************************************
**
**  This device driver was created by Applilet for the 78K0S/KB1+, 78K0S/KA1+
**  8-Bit Single-Chip Microcontrollers.
**
**  Copyright(C) NEC Electronics Corporation 2002-2006
**  All rights reserved by NEC Electronics Corporation.
**
**  This program should be used on your own responsibility.
**  NEC Electronics Corporation assumes no responsibility for any losses
**  incurred by customers or third parties arising from the use of this file.
**
**  Filename :	serial.c
**  Abstract :	This file implements device driver for serial module.
**  APIlib :	NEC78K0SKX1+.lib V1.70 [7 Dec. 2006]
**
**  Device :	uPD78F9221
**
**  Compiler :	NEC/CC78K0
**
*******************************************************************************
*/
#pragma	interrupt	INTST6	MD_INTST6
/*
*******************************************************************************
**  Include files
*******************************************************************************
*/
#include "../macrodriver.h"
#include "serial.h"


/* UART6 Transmission */
volatile char *UART6_TX_ADDRESS;	/* UART6 transfer buffer address */
volatile uint8  UART6_TX_CNT;		/* UART6 transfer number */

/* UART6 Reception, Neo, Only the TX availble, reduce this memory */
//uint8* UART6_RX_ADDRESS;	/* UART6 receive buffer address */
//uint8  UART6_RX_CNT;		/* UART6 recieve data number */
//uint8  UART6_RX_LEN;		/* UART6 recieve data count number */

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function initializes UART6 module.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/


//38400bps, 8, none parity, 1 stop bit
void UART6_Init( void )
{
	STMK6 = 1;	/* INTST6 disable */
	STIF6 = 0;	/* INTST6 IF clear */
	TXE6 = 0;	/* uart6 transmit disable */
	RXE6 = 0;
	POWER6 = 0; /* uart6 disable */
	ASIM6 = UART6_ERR_INTSR6|UART6_DATA_LENGTH_8|UART6_STOP_BIT_1|UART6_PARITY_NONE;
	ASICL6 |= UART6_FIRST_BIT_LSB | UART6_TXD6_OUTPUT_NORMAL;
	CKSR6 = UART6_BASECLK_4;
	BRGC6 = UART6_BASECLK_DIVISION;

	/* INTST6 priority low */
	STPR6 = 1;
	UART6_TX_ADDRESS = NULL;
	UART6_TX_CNT = 0;

	UART6_OnOff(ON);
}

void UART6_OnOff(uint8 on)
{
	if(on)
	{
		// firstly we need to setup the TXD and RXD gpio
		P1 |= 0x08;
		PM1 &= ~0x08;
		
		STMK6 = 0;	/* INTST6 enable */
		POWER6 = 1; /* uart6 enable */
		TXE6 = 1;	/* uart6 transmit enable */
	}
	else
	{
		TXE6 = 0;	/* uart6 transmit disable */
		POWER6 = 0; /* uart6 disable */
		STMK6 = 1;	/* INTST6 disable */
		STIF6 = 0;	/* INTST6 IF clear */

		// we need to set TXD as a input gpio, for saving consumption.
		P1 &= ~0x08;
		PM1 |= 0x08;
	}
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function is responsible for UART6 data transfering.
**
**  Parameters:
**	txbuf :	Header pointer of transfer buffer.
**	txnum :	The number of data to transmit(frame number).
**
**  Returns:
**	MD_ERROR :	When an error argument input.
**	MD_OK :		When transfer success.
**
**-----------------------------------------------------------------------------
*/
MD_STATUS UART6_SendData(const char *txbuf, uint8 txnum )
{
	if( txnum < 1 )
	{
		return MD_ERROR;		/* 1 frame data should be transfered at least */
	}

	while(0 != UART6_TX_CNT);
	
	UART6_TX_ADDRESS = txbuf;
	UART6_TX_CNT = txnum;
	
	// send the data into TXB
	if((ASIF6 & 0x02) == 0)
	{
		TXB6 = *UART6_TX_ADDRESS;
		return MD_OK;
	}
	else
	{
		return MD_DATAEXISTS;
	}
}

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function is the UART6 transmit end interrupt handler.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
__interrupt void MD_INTST6( void )
{
	UART6_TX_CNT --;
	if(UART6_TX_CNT > 0)
	{
		if(((ASIF6 & 0x02) == 0) && ((ASIM6 & 0x80) == 0x80))
		{
			UART6_TX_ADDRESS ++;
			TXB6 = *UART6_TX_ADDRESS;
		}
	}
}

void dec2Hex(uint8 input, char *output)
{
	if((input >= 0x0a) && (input <= 0x0f))
		*output = input - 0x0a + 'a';
	else if((input >= 0x00) && (input <= 0x09))
		*output = input + '0';
}

void sendHex(const char *str, void *argv)
{
	uint8 length = 0;
	char *p = str;
	char tmp;
	uint16 value = (uint16)argv;
	
	while(*p++ != '\0') length ++;
	
	
	UART6_SendData(str, length);
	
	UART6_SendData("  0x", 4);
	dec2Hex((uint8)(value/0x1000), &tmp);
	UART6_SendData(&tmp, 1);
	value = value%0x1000;
	
	dec2Hex((uint8)(value/0x100), &tmp);
	UART6_SendData(&tmp, 1);
	value = value%0x100;
	
	dec2Hex((uint8)(value/0x10), &tmp);
	UART6_SendData(&tmp, 1);
	value = value%0x10;
	
	dec2Hex((uint8)value, &tmp);
	UART6_SendData(&tmp, 1);

	UART6_SendData("\r\n", 2);	
}
//
void sendStringWithData(const char *str, uint8 argv)
{
	
#if 1
	uint8 length = 0;
	char *p = str;
	char tmp;
	
	while(*p++ != '\0') length ++;
	
	
	UART6_SendData(str, length);
	if(0 != argv)
	{
		UART6_SendData("  0x", 4);
		
		dec2Hex(argv/16, &tmp);
		UART6_SendData(&tmp, 1);
		dec2Hex(argv%16, &tmp);
		UART6_SendData(&tmp, 1);
	}
	
	UART6_SendData("\r\n", 2);
#else
	UART6_SendData("abc test\r\n",10);
#endif
}

