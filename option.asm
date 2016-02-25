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
;*  Filename :	option.asm
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
;*  Include files
;******************************************************************************
;*/
$ INCLUDE (option.inc)
;/*
;******************************************************************************
;*  Macro define
;******************************************************************************
;*/

;/*
;******************************************************************************
;*  Global define
;******************************************************************************
;*/
	OPT_SET CSEG AT 80H
OPTION:	DB	OPTION_BYTE
	DB	POC81
	DB	POC82
	DB	POC83

	ONC_SET CSEG AT 84H
ONCHIP:	DB	00H

; Start adding user code. Do not edit comment generated here
; End user code adding. Do not edit comment generated here
	END
