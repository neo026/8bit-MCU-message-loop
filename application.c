
#include "private.h"
#include "message.h"
#include "batteryManager.h"
#include "key.h"

void runningStop(uint8 errno)
{
	debugMsg("runningStop Error=", errno);
	//here you do some indications

	while(1);
}

void myMemset(const void *src, uint8 value, uint8 len)
{
	uint8 *p = (uint8*)src;
	
	if((NULL == src) || (0 == len))	
		runningStop(EER_MEMSET);  
	
	while(len)
		p[len --] = value;
}

void initData(void)
{
	/*Stop the interrupts first*/
	//DI();
	
	/*Initial global variable*/
	myMemset(&gProject, 0, sizeof(application_type));

	keyInit();
	
	//ledFlashInit();

	//i2cInit();

	messageInit();
	
	messageSend(EventEnterLimbo, 0, 1000);

	/*Start detect interrupts here, so we must remove DI() in the hdinit() function*/
	//EI();
}

void eventEnterLimbo(void)
{
	/*Stop system, all the data will save at this time*/
	enterSleep();
	
	/*system wakeup, delay 10ms*/
	delay1ms(1);
	
	/*Clear original data, Reset system data*/
	initData();	
}

void eventReady(void)
{

}

void eventPowerOn(void)
{
	if(StateLimbo == gProject.state)
	{
		// open some switch

		// start monitor the battery voltage and temperature
		//messageSend(EventBatteryMonitor, 0, 30);

	}
}

void eventPowerOff(void)
{
	if(StatePowerOn == gProject.state)
	{

	}
}

void eventBatteryLow(void)
{
	
}

void eventChargerConnected(void)
{

}

void eventChargerDisconnected(void)
{

}

void eventChargerOnOff(uint8 on)
{

}

void eventChargerError(void)
{

}

void eventChargerCompleted(void)
{
	gProject.battLevel = BATT_FULL;
}

void eventVolSub(void)
{

}

void eventVolAdd(void)
{

}

void eventMode(void)
{

}

void eventPhoneKey(void)
{

}

void messageHandler(const message_type *msg)
{
	if(NULL == msg)
		return;
	
	//debugMsg("Handle message:", msg->event);
	
	switch(msg->event)
	{
		case EventEnterLimbo:
			debugMsg("eventEnterLimbo", 0);
			//eventEnterLimbo();
			P0.0 ^= 1;
			messageSend(EventEnterLimbo, 0, 3000);
			break;
			
		case EventReady:
			eventReady();
			break;
			
		case EventPowerOn:
			eventPowerOn();
			break;	
			
		case EventPowerOff:
			eventPowerOff();
			break;

		case EventBatteryMonitor:
			batteryMonitor();
			break;

		case EventBatteryLow:
			messageSend(EventBatteryLow, 0, 1000);
			eventBatteryLow();
			break;

		case EventChargerConnected:
			eventChargerConnected();
			break;

		case EventChargerDisconnected:
			eventChargerDisconnected();
			break;

		case EventChargerOnOff:
			eventChargerOnOff(msg->msg);
			break;

		case EventChargerError:
			eventChargerError();
			break;
			
		case EventChargerCompleted:
			eventChargerCompleted();
			break;

		case EventVolSub:
			debugMsg("eventVolSub",0);
			eventVolSub();
			break;

		case EventVolAdd:
			debugMsg("eventVolAdd",0);
			eventVolAdd();
			break;

		case EventMode:
			debugMsg("eventMode",0);
			eventMode();
			break;

		case EventPhoneKey:
			debugMsg("eventPhoneKey",0);
			eventPhoneKey();
			break;

		default:
			debugMsg("Unknown message:", msg->event);
			break;
	}
}
