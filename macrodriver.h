/*
*******************************************************************************
**
**  This device driver was created by Applilet2 for 78K0/Kx2
**  8-Bit Single-Chip Microcontrollers
**
**  Copyright(C) NEC Electronics Corporation 2002 - 2014
**  All rights reserved by NEC Electronics Corporation.
**
**  This program should be used on your own responsibility.
**  NEC Electronics Corporation assumes no responsibility for any losses
**  incurred by customers or third parties arising from the use of this file.
**
**  Filename :	macrodriver.h
**  Abstract :	This file implements general head file.
**  APIlib :	Applilet2 for 78K0/Kx2 V2.10 [6 Mar. 2007]
**
**  Device :	uPD78F0513_48
**
**  Compiler :	CC78K0
**
**  Creation date:	31/7/2014
**  
*******************************************************************************
*/
#ifndef _MDSTATUS_
#define _MDSTATUS_
/*
*******************************************************************************
**	Register bit define
*******************************************************************************
*/

/*
*******************************************************************************
**  Macro define
*******************************************************************************
*/
#pragma sfr
#pragma DI
#pragma EI
#pragma NOP
#pragma HALT
#pragma STOP

/* data type defintion */
typedef	unsigned int	uint16;
typedef	unsigned char	uint8;
#define NULL	((void *)0)


#define	ON	1
#define	OFF	0

#define	TRUE	1
#define	FALSE	0

#define IDLE	0			/* idle status */
#define READ	1			/* read mode */
#define WRITE	2			/* write mode */

#define SET	1
#define CLEAR	0

#define MD_STATUS		unsigned short
#define MD_STATUSBASE		0x0

/* status list definition */
#define MD_OK				MD_STATUSBASE+0x0	/* register setting OK */
#define MD_RESET			MD_STATUSBASE+0x1	/* reset input */
#define MD_SENDCOMPLETE			MD_STATUSBASE+0x2	/* send data complete */
#define MD_OVF				MD_STATUSBASE+0x3	/* timer count overflow */

/* error list definition */
#define MD_ERRORBASE		0x80
#define MD_ERROR		MD_ERRORBASE+0x0	/* error */
#define MD_RESOURCEERROR	MD_ERRORBASE+0x1	/* no resource available */
#define MD_PARITYERROR		MD_ERRORBASE+0x2	/* UARTn parity error */
#define MD_OVERRUNERROR		MD_ERRORBASE+0x3	/* UARTn overrun error */
#define MD_FRAMEERROR		MD_ERRORBASE+0x4	/* UARTn frame error */
#define MD_ARGERROR		MD_ERRORBASE+0x5	/* Error agrument input error */
#define MD_TIMINGERROR		MD_ERRORBASE+0x6	/* Error timing operation error */
#define MD_SETPROHIBITED	MD_ERRORBASE+0x7	/* setting prohibited */
#define MD_DATAEXISTS		MD_ERRORBASE+0x8	/* Data to be transferred next exists in TXBn register */
#define MD_SPT			MD_STATUSBASE+0x8	/*IIC stop*/
#define MD_NACK			MD_STATUSBASE+0x9 	/*IIC no ACK*/
#define MD_SLAVE_SEND_END	MD_STATUSBASE+0x10 	/*IIC slave send end*/
#define MD_SLAVE_RCV_END	MD_STATUSBASE+0x11	 /*IIC slave receive end*/
#define MD_MASTER_SEND_END	MD_STATUSBASE+0x12	 /*IIC master send end*/
#define MD_MASTER_RCV_END	MD_STATUSBASE+0x13 	/*IIC master receive end*/
#define MD_BUSY			MD_STATUSBASE+0x14 	/*IIC is busy*/

/* the value for IMS and IXS */
#define  MEMORY_IMS_SET		0xC8
#define  MEMORY_IXS_SET		0x0C

/*
*******************************************************************************
**  Function define
*******************************************************************************
*/
#endif
