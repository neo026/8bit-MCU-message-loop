
#include "private.h"
#include "message.h"
#include "key.h"
#include "utils/pwm.h"

application_type gProject;

// timer tick is 500us, so this input "count" range is 0 ~ 127ms
void delay1ms(uint8 count)
{
	gProject.timerDelay =  count * 2;
	while(gProject.timerDelay);
}

void main(void)
{
	/*hdwinit() function already finish hardware initial*/
	/*system hardware initialization*/

	uint8 timer1sec = 0;
	uint8 timer100ms = 0;
	
	/*Delay 10ms, let system stable*/
	delay1ms(5);
	
	/*Start initial the system data*/
	initData();
	
	while(1)
	{	
		/*message Loop*/
		messageLoop();

		// 500 us handle, because the timer is 500us tick timer
		if(gProject.timerTick)
		{
			gProject.timerTick = 0;
			timer100ms ++;
			//ledPwmLoop();
			/* Fade on or fade off*/
			ledFadeLoop();
		}

		// 100 ms handle
		if(timer100ms > 200)
		{
			timer100ms = 0;
			timer1sec ++;
			keyLoop();
			//ledFlashLoop();
		}

		// 1 second handle
		if(timer1sec > 10)
		{
			timer1sec = 0;

			if(chargerCharging == gProject.chargerStatus)
			{
				gProject.chargingCount ++;
			}
			else
			{
				if(gProject.chargingCount > 0)
					gProject.chargingCount --;
			}
		}
	}
}
