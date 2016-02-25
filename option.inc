;/*
;******************************************************************************
;*
;*  This device driver was created by Applilet2 for the Applilet2 for 78K0/Kx2
;*  8-Bit Single-Chip Microcontrollers
;*
;*  Copyright(C) NEC Electronics Corporation 2002 - 2014
;*  All rights reserved by NEC Electronics Corporation.
;*
;*  This program should be used on your own responsibility.
;*  NEC Electronics Corporation assumes no responsibility for any losses
;*  incurred by customers or third parties arising from the use of this file.
;*
;*  Filename :	option.inc
;*  Abstract :	This file implements OPTION-BYTES/SECURITY-ID setting.
;*  APIlib :	Applilet2 for 78K0/Kx2 V2.10 [6 Mar. 2007]
;*
;*  Device :	uPD78F0513_48
;*
;*  Compiler :	CC78K0
;*
;*  Creation date:	31/7/2014
;*  
;******************************************************************************
;*/

;/*
;******************************************************************************
;*  Macro define
;******************************************************************************
;*/
;/* 
;******************************************************************************
;*  Register bit define
;******************************************************************************
;*/ 
;*** OPTION-BYTES bit define ***
; Option at 80H
; Watchdog timer window open period (WINDOW1-0)
WDT_WINDOW_OPEN	EQU	60H
WDT_WINDOW_OPEN_PERIOD0	EQU	00H	;25%
WDT_WINDOW_OPEN_PERIOD1	EQU	20H	;50%
WDT_WINDOW_OPEN_PERIOD2	EQU	40H	;75%
WDT_WINDOW_OPEN_PERIOD3	EQU	60H	;100%
; Watchdog timer counter control (WDTON)
WDT_OPERATION	EQU	10H
WDT_OPERATION_ENABLE	EQU	10H	;Counter operation enabled
WDT_OPERATION_DISABLE	EQU	00H	;Counter operation disabled
; Watchdog timer overflow time (WDCS2-0)
WDT_OVERFLOW	EQU	0EH
WDT_OVERFLOW_TIME0	EQU	00H	;2^10/fRL (3.88 ms)
WDT_OVERFLOW_TIME1	EQU	02H	;2^11/fRL (7.76 ms)
WDT_OVERFLOW_TIME2	EQU	04H	;2^12/fRL (15.52 ms)
WDT_OVERFLOW_TIME3	EQU	06H	;2^13/fRL (31.03 ms)
WDT_OVERFLOW_TIME4	EQU	08H	;2^14/fRL (62.06 ms)
WDT_OVERFLOW_TIME5	EQU	0AH	;2^15/fRL (124.12 ms)
WDT_OVERFLOW_TIME6	EQU	0CH	;2^16/fRL (248.24 ms)
WDT_OVERFLOW_TIME7	EQU	0EH	;2^17/fRL (496.48 ms)
; Internal low-speed oscillator operation (RINGOSC)
CG_LOWINTERNAL	EQU	01H
CG_LOWINTERNAL_CANSTOP	EQU	00H	;can be stopped by software
CG_LOWINTERNAL_CANNOT	EQU	01H	;cannot be stopped

; Option at 81H
; POC mode selection (POCMODE)
CG_POCMODE		EQU	01H
CG_POCMODE_1DOT59V	EQU	00H	;1.59 V POC mode (default)
CG_POCMODE_2DOT7V	EQU	01H	;2.7 V/1.59 V POC mode

; Option at 84H
; On-chip debug operation control (OCDEN1-0)
CG_FLASH		EQU	03H	;when security ID code authenticaiton fails
CG_FLASH_NOTERASE	EQU	02H	;data is not deleted
CG_FLASH_ERASE	EQU	01H	;data is deleted

;*** OPTION-BYTES/SECURITY-ID define ***
OPTION_BYTE EQU	CG_LOWINTERNAL_CANSTOP
POC81	EQU	00H
POC82	EQU	00H
POC83	EQU	00H

