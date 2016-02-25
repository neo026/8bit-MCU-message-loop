
#ifndef _KEY_H
#define _KEY_H

enum
{
	noPressFlag,
	shortPressFlag,
	longPressFlag,
	vLongPressFlag,
	vvLongPressFlag
};

typedef struct
{
	uint8 keyValue;		/*Save Current Press Key value*/
	uint8 keyValueLast;	/*Save Last Press Key value*/
	uint8 keyStatus;
	uint8 keyHoldTimeCnt;	
}key_type;

#define KEYT	50
#define KEY0	1023

#define KEY1	0
#define KEY2	283
#define KEY3	594
#define KEY4	941

#define KEYERR	0xFF
#define KEYNO0	0

#define KEY_POWER	(1 << 0)
#define KEY_VOLSUB	(1 << 1)
#define KEY_VOLADD	(1 << 2)
#define KEY_BT		(1 << 3)
#define KEY_PHONE	(1 << 4)

#define KEYHOLD3S	(20)	
#define KEYHOLD5S	(30)
#define KEYHOLD10S	(80)

void keyInit(void);
void keyLoop(void);
#endif