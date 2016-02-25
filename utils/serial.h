
#ifndef _SERIAL_H
#define _SERIAL_H



/*
*******************************************************************************
**   UART6n
*******************************************************************************
*/

/* Asynchronous serial interface operation mode register (ASIM6n) */

/*uart6 transfer parity(PS01,PS00)*/
#define UART6_PARITY_NONE	0x00	/* transfer without parity */
#define UART6_PARITY_ZERO	0x08	/* judges as 0 parity */
#define UART6_PARITY_ODD	0x10	/* judges as odd parity */
#define UART6_PARITY_EVEN	0x18	/* judges as even parity */

/* uart6 character length of transfer data(CL0) */
#define UART6_DATA_LENGTH_8	0x04	/* character length of data = 8 bits */
#define UART6_DATA_LENGTH_7	0x00	/* character length of data = 7 bits */

/* uart6 stop bits of transmit data(SL0) */
#define UART6_STOP_BIT_2	0x02	/* stop bits = 2 */
#define UART6_STOP_BIT_1	0x00	/* stop bits = 1 */

/* uart6 occurrence of reception completion interrupt in case of error(ISRM6) */
#define UART6_ERR_INTSRE6	0x00	/* INTSRE6 occurs in case of error */
#define UART6_ERR_INTSR6	0x01	/* INTSR6 occurs in case of error */

/* Asynchronous serial interface reception error status register (ASIS6) */
/* Status flag indicating parity error(PE6) */
#define UART6_PARITY_ERROR	0x04
/* Status flag indicating framing error(FE6) */
#define UART6_FRAMING_ERROR	0x02
/* Status flag indicating overrun error(OVE6) */
#define UART6_OVERRUN_ERROR	0x01 

/* Format of baud rate generator control register 0 (CKSR6) */
#define UART6_BASECLK_1		0x00	/* fxclk0 = fprs */
#define UART6_BASECLK_2		0x01	/* fxclk0 = fprs/2  */
#define UART6_BASECLK_3		0x02	/* fxclk0 = fprs/4  */
#define UART6_BASECLK_4		0x03	/* fxclk0 = fprs/8  */
#define UART6_BASECLK_5		0x04	/* fxclk0 = fprs/16 */
#define UART6_BASECLK_6		0x05	/* fxclk0 = fprs/32 */
#define UART6_BASECLK_7		0x06	/* fxclk0 = fprs/64 */
#define UART6_BASECLK_8		0x07	/* fxclk0 = fprs/128 */
#define UART6_BASECLK_9		0x08	/* fxclk0 = fprs/256 */
#define UART6_BASECLK_10	0x09	/* fxclk0 = fprs/512 */
#define UART6_BASECLK_11	0x0a	/* fxclk0 = fprs/1024*/
#define UART6_BASECLK_12	0x0b	/* fxclk0 = TM50 output */

#define UART6_BASECLK_DIVISION		0xd

/* Format of asynchronous serial interface control register 6 (ASICL6) */
/* First-bit direction(DIR6) */
#define UART6_FIRST_BIT_LSB	0x02	/* LSB */
#define UART6_FIRST_BIT_MSB	0x00	/* MSB */

/* Enable/disable inverting TxD6 output(TXDLV6) */
#define UART6_TXD6_OUTPUT_INVERTED	0x01	/* TxD6 output inverted */
#define UART6_TXD6_OUTPUT_NORMAL	0x00	/* TxD6 output normal */

/* Format of asynchronous serial interface transmission status register 6 (ASIF6) */
#define UART6_TRANSMIT_BUFFER_FLAG	0x02	/* if data exists in TXB6,the data is written to transmit buffer register 6 (TXB6) */
#define UART6_TRANSMIT_SHIFT_FLAG	0x01	/* if data transmission is in progress,the data is transferred from transmit buffer register 6 (TXB6) */
/*
	Serial operation mode register (CSIM1n)
*/

/* Receive/transfer mode control (TRMD1n) */  
#define	CSI1_MODE_RECEIVE	0x00	/* receive mode */
#define	CSI1_MODE_BOTH		0x40	/* transmit/receive mode */

/* First bit specification (DIR1n) */
#define	CSI1_FIRST_BIT_MSB	0x00	/* MSB */ 
#define	CSI1_FIRST_BIT_LSB	0x10	/* LSB */

/* SSI11 pin use selection (SSE11) */    
#define	CSI1_SSI11_UNUSED	0x00	/* SSI11 pin is not used */
#define	CSI1_SSI11_USED		0x20	/* SSI11 pin is used */

/*
	Serial clock selection register (CSIC1n)
*/

/* Receive/transmit timing (CKP1n,DAP1n) */	
#define	CSI1_DATA_TIMING_1	0x00	/* type 1 */
#define	CSI1_DATA_TIMING_2	0x08	/* type 2 */
#define	CSI1_DATA_TIMING_3	0x10	/* type 3 */
#define	CSI1_DATA_TIMING_4	0x18	/* type 4 */

/* Clock selection (CKS1n2,CKS1n1,CKS1n0) */
#define	CSI1_CLOCK_1		0x00	/* fprs/2 */
#define	CSI1_CLOCK_2		0x01	/* fprs/4 */
#define	CSI1_CLOCK_3		0x02	/* fprs/8 */	
#define	CSI1_CLOCK_4		0x03	/* fprs/16 */
#define	CSI1_CLOCK_5		0x04	/* fprs/32 */
#define	CSI1_CLOCK_6		0x05	/* fprs/64 */
#define	CSI1_CLOCK_7		0x06	/* fprs/128 */
#define	CSI1_CLOCK_EXT		0x07	/* external clock */

#define UART_ERR_PARITY		0x04		/* UART parity error */
#define UART_ERR_FRAME		0x02		/* UART frame error */
#define UART_ERR_OVERRUN	0x01		/* UART overrun error */
/*
*******************************************************************************
**  Global variables
*******************************************************************************
*/
#define	UART_BAUDRATE_M6	0x0
#define	UART_BAUDRATE_K6	0x68

void UART6_Init( void );
void UART6_OnOff(uint8 on);
void sendStringWithData(const char *str, uint8 argv);
void sendHex(const char *str, void *argv);

#define sendString(x) sendStringWithData(x, 0)
#endif // _SERIAL_H
