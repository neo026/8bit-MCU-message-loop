
#ifndef _LED_H
#define _LED_H

typedef struct
{
	uint8 led;
	uint8 onTime;
	uint8 offTime;
	uint8 time;
	uint8 on;
	uint8 count;
}led_type;

void ledOnOff(uint8 led, uint8 on);
void ledFlashStart(uint8 led, uint8 onTime, uint8 offTime, uint8 count);
void ledFlashStop(void);
void ledFlashInit(void);
void ledFlashLoop(void);
#endif	//_LED_H

