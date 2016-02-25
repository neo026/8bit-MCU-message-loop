
#include "pwm.h"

#define PWM_STEP		15

static pwm_type thePWM;

void ledPwmStart(uint8 led, uint8 period, uint8 ratio)
{
	if(LED_NONE == led)
		return;

	thePWM.led = led;
	thePWM.period = period;
	thePWM.ratio = ratio;
	thePWM.on = ON;
	thePWM.time = ratio;
	ledOnOff(thePWM.led, ON);
}

void ledPwmStop(void)
{
	if(LED_NONE == thePWM.led)
		return ;

	thePWM.period = 0;
	thePWM.ratio = 0;
	thePWM.on = OFF;
	thePWM.time = 0;
	ledOnOff(thePWM.led, OFF);	
	thePWM.led = LED_NONE;
}

void ledPwmLoop(void)
{
	if(LED_NONE != thePWM.led)
	{
		if(thePWM.time)
		{
			thePWM.time --;
		}
		else
		{
			thePWM.on ^= 1;
			thePWM.time = (thePWM.on) ? thePWM.ratio : thePWM.period - thePWM.ratio;
			ledOnOff(thePWM.led, thePWM.on);
		}
	}
}



