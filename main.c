
#include "private.h"
#include "message.h"
#include "key.h"
//#include "utils/led.h"

application_type gProject;

void delay1ms(uint8 count)
{
	gProject.delayTimer =  count;
	while(gProject.delayTimer);
}

void main(void)
{
	/*hdwinit() function already finish hardware initial*/
	/*system hardware initialization*/

	uint8 timer1sec = 0;
	
	/*Delay 10ms, let system stable*/
	delay1ms(5);
	
	/*Start initial the system data*/
	initData();
	
	while(1)
	{	
		/*message Loop*/
		messageLoop();
		
		if(gProject.timer100ms > 100)
		{
			gProject.timer100ms = 0;
			timer1sec ++;
			keyLoop();
			//ledFlashLoop();
		}
		
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
