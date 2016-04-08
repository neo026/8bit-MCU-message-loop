
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
	uint8 keyRepeatTimeCnt;
}key_type;

// need add 50 adc value
#define KEY1	0
#define KEY2	283
#define KEY3	594
#define KEY4	941

#define KEY0	1023
#define KEYT	50

#define KEY_ERR		0xff		// detect error or sample error, something multi keys press will cause it
#define KEY_NONE	0x0			// key release
#define KEY_POWER	(1 << 0)
#define KEY_VOLSUB	(1 << 1)
#define KEY_VOLADD	(1 << 2)
#define KEY_BT		(1 << 3)
#define KEY_PHONE	(1 << 4)

#define KEY_LONG_TIME	(10)	//uint: 100ms
#define KEY_VLONG_TIME	(20)
#define KEY_VVLONG_TIME	(50)
#define	KEY_REPEAT_TIME	(5)

void keyInit(void);
void keyLoop(void);
#endif