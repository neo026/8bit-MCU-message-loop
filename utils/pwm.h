
#ifndef _PWM_H
#define _PWM_H

#include "private.h"

typedef struct
{
	uint8 led;
	uint8 period;
	uint8 ratio;
	uint8 time;
	uint8 on;
}pwm_type;

void ledPwmLoop(void);


#endif // _PWM_H

