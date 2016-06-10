
#include "../private.h"
#include "led.h"
#include "pwm.h"

static pwm_type thePWM;
static fade_type theFade;

void ledPwmStart(uint8 led, uint8 period, uint8 ratio)
{
	if((LED_NONE == led) || (period == 0) || (ratio > period))
		return;

	thePWM.led = led;
	thePWM.period = period;
	thePWM.ratio = ratio;

	// we need to count the time of full fade off state or full fade on state
	if(ratio == 0)
	{
		// full fade off state
		thePWM.onOff = OFF;
		thePWM.time = period;
	}
	else
	{
		thePWM.onOff = ON;
		thePWM.time = ratio;
	}

	ledOnOff(thePWM.led, thePWM.onOff);
}

void ledPwmStop(void)
{
	if(LED_NONE == thePWM.led)
		return ;

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
			if((thePWM.period == thePWM.ratio) || (0 == thePWM.ratio))
			{
				// solid on or solid off state, we don't change led output level. but we the duty still need.
				thePWM.time = thePWM.period;
			}
			else
			{
				thePWM.onOff ^= 1;
				thePWM.time = (thePWM.onOff) ? thePWM.ratio : thePWM.period - thePWM.ratio;
				ledOnOff(thePWM.led, thePWM.onOff);
			}

		}		
	}
}

/* 1, fade on and fade in have the same time.
     2, keeptime, we will current ratio of PWM for a while
     fade on time = fade time /2.
     keep time = fade on time / PWM_STEP.

     example, product required a 3 second fade time.
     fade on time = 3/2 = 1.5sec
     keep time = 1.5/10 = 0.15sec = 150ms.

     keep time is what we need at this function.

*/



// input "repeat" range is
void ledFadeStart(uint8 led, uint16 repeat)
{
	if((LED_NONE != led) && (repeat > 0))
	{
		// check the fade led is runnig, if yes, stop the last fading led firstly.
		if(LED_NONE != theFade.led)
			ledFadeStop();

		// set parameters for fade  led.
		theFade.led = led;
		theFade.repeat = repeat;	// the keep time of every step.
		theFade.direction = ON;		// fade on first
		theFade.step = 0;
		// pwm
		theFade.pwmRatio = theFade.step;	// for pwm ratio
		theFade.pwmRepeat = theFade.repeat;	// for pwm repeat counter
		
		// we need to count the time of full fade off state or full fade on state
		if(0 == theFade.pwmRatio)
		{
			//full fade off state
			theFade.onOff = OFF;
			theFade.pwmTime = PWM_PERIOD;
		}
		else
		{
			//
			theFade.onOff = ON;
			theFade.pwmTime = theFade.pwmRatio;
		}
		
		ledOnOff(theFade.led, theFade.onOff);
	}
}

void ledFadeStop(void)
{
	if(LED_NONE == theFade.led)
		return ;
	
	ledOnOff(theFade.led, OFF);
	theFade.led = LED_NONE;
}

void ledFadeLoop(void)
{
	if(LED_NONE == theFade.led)
		return;

	if(theFade.pwmRepeat == 0)
	{		
		if(ON == theFade.direction)
		{
			if(theFade.step < PWM_STEP)
			{
				theFade.step ++;
			}
			else
			{
				//change to fade off direction
				theFade.direction = OFF;
			}
		}
		else
		{
			if(theFade.step > 0)
			{
				theFade.step --;
			}
			else
			{
				// change to fade on direction
				theFade.direction = ON;
			}
		}


		// here change the fade parameters
		theFade.pwmRatio = theFade.step;	// for pwm ratio
		theFade.pwmRepeat = theFade.repeat;	// for pwm repeat counter
		//debugMsg("r:", theFade.pwmRatio);
		// we need to count the time of full fade off state or full fade on state
		if(0 == theFade.pwmRatio)
		{
			//full fade off state
			theFade.onOff = OFF;
			theFade.pwmTime = PWM_PERIOD;
		}
		else
		{
			//
			theFade.onOff = ON;
			theFade.pwmTime = theFade.pwmRatio;
		}
		
		ledOnOff(theFade.led, theFade.onOff);
	}
	else
	{
		// this is a pwm loop detection
		if(theFade.pwmTime)
		{
			theFade.pwmTime --;
		}
		else
		{
			if((0 == theFade.pwmRatio) || (PWM_PERIOD == theFade.pwmRatio))
			{
				theFade.pwmTime = PWM_PERIOD;
			}
			else
			{
				// change direction
				theFade.onOff ^= 1;
				theFade.pwmTime = (theFade.onOff) ? theFade.pwmRatio : PWM_PERIOD - theFade.pwmRatio;
				ledOnOff(theFade.led, theFade.onOff);
			}
		}

		if(theFade.pwmRepeat)
			theFade.pwmRepeat --;
	}
}

