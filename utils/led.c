
#include "../private.h"
#include "led.h"

static led_type theLed;

void ledOnOff(uint8 led, uint8 on)
{
	if(led & LED_POWER)			GPIO_LED_POWER(on);
	if(led & LED_CHARGER)		GPIO_LED_CHARGER(on);
	if(led & LED_VOLSUB)		GPIO_LED_VOLSUB(on);
	if(led & LED_VOLADD)		GPIO_LED_VOLADD(on);
	if(led & LED_BT)			GPIO_LED_BT(on);
}

void ledFlashInit(void)
{
	// shut down all the Flash LED
	ledOnOff(LED_CHARGER | LED_POWER, OFF);
		
	theLed.led = LED_NONE;
}

// led -> which led, unit 100ms
void ledFlashStart(uint8 led, uint8 onTime, uint8 offTime, uint8 count)
{
	if((LED_NONE == led) || (0 == onTime) ||(0 == offTime))
		return ;

	theLed.led = led;
	theLed.onTime = onTime;
	theLed.offTime = offTime;
	theLed.count = count;
	theLed.on = ON;
	theLed.time = onTime;
	ledOnOff(theLed.led, ON);
}

void ledFlashStop(void)
{
	if(LED_NONE == theLed.led)
		return;
	
	ledOnOff(theLed.led, OFF);
	theLed.led = LED_NONE;
}

void ledFlashLoop(void)
{
	if(LED_NONE == theLed.led)
		return ;

	if(theLed.time)
	{
		theLed.time --;
	}
	else
	{
		theLed.on ^= 1;
		theLed.time = theLed.on ? theLed.onTime : theLed.offTime;		
		ledOnOff(theLed.led, theLed.on);
		
		// calc the count of this led flash
		if(LIMITLESS != theLed.count)
		{
			theLed.count --;
			if(0 == theLed.count)
				ledFlashStop();
		}
	}
}

