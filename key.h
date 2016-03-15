
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
#define KEY1	50		
#define KEY2	333
#define KEY3	644
#define KEY4	991

#define KEY_NONE	0
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