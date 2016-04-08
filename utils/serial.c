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

#ifdef ENABLE_SERIAL
#pragma	interrupt	INTST6	MD_INTST6
#pragma interrupt	INTSR6	MD_INTSR6

#include "../macrodriver.h"
#include "serial.h"

/* For finding packet byte*/
#define	FLAG_HEAD0		0
#define	FLAG_HEAD1		1
#define	FLAG_LENGTH		2
#define FLAG_PDU		3

#define MIN_PKT_LENGTH	3
static uint8 packet_header[] = {'*','@'};

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

#if 0
void printIndex(void)
{
	uint8 tmp;
	
	while((ASIF6 & 0x02) != 0);
	TXB6 = dec2Hex(theSerial.txPutIndex/16);
	while((ASIF6 & 0x02) != 0);
	TXB6 = dec2Hex(theSerial.txPutIndex%16);
	while((ASIF6 & 0x02) != 0);
	TXB6 = dec2Hex(theSerial.txGetIndex/16);
	while((ASIF6 & 0x02) != 0);
	TXB6 = dec2Hex(theSerial.txGetIndex%16);
	while((ASIF6 & 0x02) != 0);
	TXB6 = ':';
	for(tmp = theSerial.txGetIndex; tmp != theSerial.txPutIndex; tmp ++)
	{
		if(tmp > SERIAL_TX_BUFFER_LENGTH)tmp = 0;
		while((ASIF6 & 0x02) != 0);
		TXB6 = theSerial.txBuffer[tmp];
	}
}

void printRaw(uint8 byte)
{
	while((ASIF6 & 0x02) != 0);
	TXB6 = byte;
}

void printChar(uint8 byte)
{
	while((ASIF6 & 0x02) != 0);
	TXB6 = dec2Hex(byte/16);
	while((ASIF6 & 0x02) != 0);
	TXB6 = dec2Hex(byte%16);
}

void printString(const char *str)
{
	uint8 i;
	uint8 size = 0;
	char *p = str;
	
	while(*p++ != '\0')
		size ++;

	for(i = 0; i < size; i++)
	{
		while((ASIF6 & 0x02) != 0);
		TXB6 = str[i];
	}	
}
void gernateFrame(void)
{
#define G_SIZE 10

	uint8 frame[G_SIZE] = {'*','@','>',G_SIZE-7};
	uint8 i;
	static uint8 a = 12;
	uint8 checksum;
	
	frame[0] = packet_header[0];
	frame[1] = packet_header[1];
	frame[2] = G_SIZE - 2;
	frame[3] = '>';
	
	checksum = frame[2]+frame[3];
	//format: HEADER + LENGTH + CMD + DATA+ CHECKSUM
	for(i = 4; i < G_SIZE-3; i++)
	{
		frame[i] = a++;
		checksum += frame[i];
	}
	
	frame[i++] = ~checksum + 1;
	addTxBuffer(frame,G_SIZE);
}
#endif

void serialSendNum(uint8 num)
{
	uint8 tmp;
	
	tmp = dec2Hex(num/16);
	addTxBuffer(&tmp, 1);
	tmp = dec2Hex(num%16);
	addTxBuffer(&tmp, 1);
	tmp = ' ';
	addTxBuffer(&tmp, 1);
}

void serialSendString(const char *str)
{
	char *p = str;
	uint8 str_size = 0;
	// send string
	while(*p++ != '\0') str_size ++;
	addTxBuffer((uint8 *)str, str_size);
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

/* format: HEADER + LENGTH + CMD + DATA+ CHECKSUM
    data_size, is the size of pdata.
*/
void sendPacket(uint8 cmd, uint8 *pdata, uint8 data_size)
{
	uint8 i;
	uint8 checksum;
	uint8 length;
	
	addTxBuffer(packet_header, sizeof(packet_header));
	length = data_size + MIN_PKT_LENGTH;
	addTxBuffer(&length, 1);
	addTxBuffer(&cmd, 1);	
	addTxBuffer(pdata,data_size);

	checksum = cmd + length;
	for(i = 0; i < data_size; i++)
		checksum += pdata[i];

	checksum = ~checksum + 1;
	addTxBuffer(&checksum, 1);
}

/* this is a valid packet, it means that this packet have the header and the tail,
	we need to process it again here, such as judge packet length, data_size and checksum
*/
static uint8 checkSum(uint8 startIndex, uint8 endIndex)
{
	uint8 index;
	uint8 checksum;
	uint8 *buff = theSerial.rxBuffer;

	// HEADER + LENGTH + CMD + DATA+ CHECKSUM
	#if 0	//debug
	printString("\npkt:");
	for(index = startIndex; index != endIndex; index ++)
		printChar(buff[index]);
	
	printChar(buff[index]);	// checksum
	#endif

	for(checksum = 0, index = startIndex; index != endIndex; index ++)
		checksum += buff[index];
	
	if(checksum & buff[endIndex] == 0)  //
		return FALSE;

	#if 0	//debug
	printString(" cmd:");
	printChar(buff[startIndex + 1]);
	printString("\n");
	#endif

	return TRUE;
}
// handle the data that received by RX interrupt service routine.
void rxLoop(void)
{
	uint8 byte;								// get data from the Rx Buffer 
	uint8 *buff = theSerial.rxBuffer;
	uint8 getIndex = theSerial.rxGetIndex;	// current index of the Rx buffer
	static uint8 rxFindFlag = FLAG_HEAD0;	// this is a flag indicate that which part of packet we need to find
	static uint8 startIndex;
	static uint8 packet_length;
	
	// There is nothing need to handle for this loop
	if(getIndex == theSerial.rxPutIndex)
		return;

	// get the specific byte
	byte = buff[getIndex];
	
	// HEADER + LENGTH + CMD + DATA+ CHECKSUM
	if(rxFindFlag == FLAG_HEAD0)
	{
		if(byte == packet_header[0])
			rxFindFlag = FLAG_HEAD1;

	}
	else if(rxFindFlag == FLAG_HEAD1)
	{
		if(byte == packet_header[1])
		{
			rxFindFlag = FLAG_LENGTH;
		}
		else
		{
			rxFindFlag = FLAG_HEAD0;
			// do not update the read index, because this situation will be ignore: **@xxx
			return;
		}
	}
	else if(rxFindFlag == FLAG_LENGTH)
	{
		packet_length = byte - 1;	// because packet_length contains itself.
		if(byte >= MIN_PKT_LENGTH)
		{
			rxFindFlag = FLAG_PDU;
			startIndex = getIndex;
		}
		else
			rxFindFlag = FLAG_HEAD0;
	}
	else if(rxFindFlag == FLAG_PDU)
	{
		packet_length --;
		if(packet_length == 0)
		{
			if(checkSum(startIndex, getIndex))
				cmdHandler(buff[startIndex + 1], startIndex, buff[startIndex] - MIN_PKT_LENGTH);
			
			rxFindFlag = FLAG_HEAD0;
		}
	}

	theSerial.rxGetIndex ++;
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
	rxLoop();
}

void serialEnable(uint8 enable)
{
	if(enable)
	{
		// firstly we need to setup the TXD and RXD gpio
		P1 |= 0x08;
		PM1 &= ~0x08;
		
		STMK6 = 0;	/* INTST6 enable */
		SRMK6 = 0;
		POWER6 = 1; /* uart6 enable */
		TXE6 = 1;	/* uart6 transmit enable */
		RXE6 = 1;
		// RX
		theSerial.rxPutIndex = 0;
		theSerial.rxGetIndex = 0;
		// TX
		theSerial.txPutIndex = 0;
		theSerial.txGetIndex = 0;
	}
	else
	{
		RXE6 = 0;
		TXE6 = 0;	/* uart6 transmit disable */
		POWER6 = 0; /* uart6 disable */
		STMK6 = 1;	/* INTST6 disable */
		SRMK6 = 0;
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
	SRMK6 = 1;
	STIF6 = 0;	/* INTST6 IF clear */
	SRIF6 = 0;
	TXE6 = 0;	/* uart6 transmit disable */
	RXE6 = 0;
	
	POWER6 = 0; /* uart6 disable */
	ASIM6 = UART6_ERR_INTSR6|UART6_DATA_LENGTH_8|UART6_STOP_BIT_1|UART6_PARITY_NONE;
	ASICL6 |= UART6_FIRST_BIT_LSB | UART6_TXD6_OUTPUT_NORMAL;
	CKSR6 = UART6_BASECLK_4;	//38400
	BRGC6 = UART6_BASECLK_DIVISION;

	/* INTST6 priority low */
	STPR6 = 1;
	SRPR6 = 1;
}


__interrupt void MD_INTST6( void )
{

}

__interrupt void MD_INTSR6(void)
{
	if(ASIS6 & 0x07)
		return;
	
	theSerial.rxBuffer[theSerial.rxPutIndex ++] = (uint8) RXB6;
}
#else
	static char dumy = 0;
#endif

