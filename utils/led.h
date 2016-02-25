
#ifndef _LED_H
#define _LED_H

#define LED_NONE		0x00
#define LED_POWER		(1 << 0)
#define LED_CHARGER		(1 << 1)
#define LIMITLESS		0x00

typedef struct
{
	uint8 led;
	uint8 onTime;
	uint8 offTime;
	uint8 time;
	uint8 on;
	uint8 count;
}led_type;

void ledFlashInit(void);
void ledFlashLoop(void);
#endif	//_LED_H

