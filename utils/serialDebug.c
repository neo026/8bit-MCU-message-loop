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

/*
*******************************************************************************
**  Include files
*******************************************************************************
*/

#if ENABLE_SERIAL
#pragma	interrupt	INTST6	MD_INTST6

#include "../macrodriver.h"
#include "serial.h"

serial_type theSerial;
/* -------------------------------------------------------------------*/
/*convert decimal number to hexadecimal*/
uint8 dec2Hex(uint8 input)
{
	if((input >= 0x0a) && (input <= 0x0f))
		return input - 0x0a + 'a';
	else if((input >= 0x00) && (input <= 0x09))
		return input + '0';
	else
		return 'z';		// input wrong number		
}

/* put the data into the Tx Buffer and update the write index */
void addTxBuffer(uint8 *tx_addr, uint8 tx_len)
{
	uint8 i;

	for(i = 0; i < tx_len; i ++)
		theSerial.txBuffer[theSerial.txPutIndex ++] = tx_addr[i];
}

/* for printing the debug information */
void serialSendStringWithData(const char *str, uint8 argv)
{
	char *p = str;
	uint8 str_size = 0;
	uint8 tmp;
	
	// send string
	while(*p++ != '\0') str_size ++;
	addTxBuffer((uint8 *)str, str_size);

	// send argv
	tmp = ' ';
	addTxBuffer(&tmp, 1);
	tmp = dec2Hex(argv/16);
	addTxBuffer(&tmp, 1);
	tmp = dec2Hex(argv%16);
	addTxBuffer(&tmp, 1);
	
	// send tail
	tmp = '\n';
	addTxBuffer(&tmp, 1);
}


// transmit one byte each cycle.
void txLoop(void)
{
	if((theSerial.txPutIndex != theSerial.txGetIndex) && ((ASIF6 & 0x02) == 0))
		TXB6 = theSerial.txBuffer[theSerial.txGetIndex ++];
}

void serialLoop(void)
{
	txLoop();
}

void serialEnable(uint8 enable)
{
	if(enable)
	{
		// firstly we need to setup the TXD and RXD gpio
		P1 |= 0x08;
		PM1 &= ~0x08;
		
		STMK6 = 0;	/* INTST6 enable */
		POWER6 = 1; /* uart6 enable */
		TXE6 = 1;	/* uart6 transmit enable */

		// TX
		theSerial.txPutIndex = 0;
		theSerial.txGetIndex = 0;
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

void serialInit(void)
{
	// hardware initialize
	STMK6 = 1;	/* INTST6 disable */
	STIF6 = 0;	/* INTST6 IF clear */
	TXE6 = 0;	/* uart6 transmit disable */
	
	POWER6 = 0; /* uart6 disable */
	ASIM6 = UART6_ERR_INTSR6|UART6_DATA_LENGTH_8|UART6_STOP_BIT_1|UART6_PARITY_NONE;
	ASICL6 |= UART6_FIRST_BIT_LSB | UART6_TXD6_OUTPUT_NORMAL;
	CKSR6 = UART6_BASECLK_4;	//38400
	BRGC6 = UART6_BASECLK_DIVISION;

	/* INTST6 priority low */
	STPR6 = 1;
}

__interrupt void MD_INTST6( void )
{

}
#else
	static char dumy = 0;
#endif

