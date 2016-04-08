
#include "private.h"
#include "message.h"
#include "batteryManager.h"
#include "key.h"
#include "utils/pwm.h"
#include "utils/led.h"

#define MAX_VOLUME 16


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
		runningStop(ERR_MEMSET);  
	
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
	
	ledFlashInit();

	//i2cInit();

	messageInit();
	
	messageSend(EventEnterLimbo, 0, 2000);
	messageSend(EventBatteryMonitor, 0, 0);

	gProject.state = StateStandby;
	/*Start detect interrupts here, so we must remove DI() in the hdinit() function*/
	//EI();
}

void eventEnterLimbo(void)
{
	gProject.state = StateLimbo;
	
	/*Stop system, all the data will save at this time*/
	//enterSleep();
	
	/*system wakeup, delay 10ms*/
	delay1ms(1);
	
	/*Clear original data, Reset system data*/
	//initData();	
}

void eventPowerKey(void)
{
	messageCancel(EventEnterLimbo);
	messageCancel(EventPowerOn);
	messageCancel(EventPowerOff);

	if(StateLimbo == gProject.state)
	{
		gProject.state = StateReadyOn;
		// turn on led 
		ledOnOff(LED_POWER, ON);
		//debugStr("ON\n");
	}
	else
	{
		gProject.state = StateLimbo;
		// turn off led and amp
		ledFadeStop();
		ledOnOff(LED_POWER, OFF);
		//debugStr("OFF\n");
	}
	
	messageSend(EventReady, 0, T_SEC(1));
}

void eventReady(void)
{
	if(StateReadyOn == gProject.state)
		messageSend(EventPowerOn, 0, 0);
	else
		messageSend(EventPowerOff, 0, 0);
}

void eventPowerOn(void)
{
	if(StatePowerOn != gProject.state)
	{
		// open some switch
		gProject.state = StatePowerOn;
		
		// start monitor the battery voltage and temperature
	}
}

void eventPowerOff(void)
{
	if(StatePowerOn == gProject.state)
	{
		gProject.state = StateLimbo;
	}
}

void eventBatteryLow(void)
{
	
}

void eventChargerOnOff(uint8 on)
{
	if(gProject.chargerConnnected)
	{
		if(on)
		{
			if((gProject.battLevel < BATT_FULL) && batteryIsNormal())
			{
				// open charger
				GPIO_CHARGER_ON(ON);
				// turn on indicator
				ledOnOff(LED_CHARGER, ON);
				//
				messageSend(EventChargerDetectStart, 0, T_SEC(1));
			}
		}
		else
		{
			// close the charger 
			GPIO_CHARGER_ON(OFF);

			// turn off indicator
			ledOnOff(LED_CHARGER, OFF);

			//
			messageCancel(EventChargerDetectStart);
			gProject.chargerStatus = chargerNotCharging;
			gProject.chargerStartCnt = FALSE;
		}
		
		gProject.chargerOnOff = on;
	}
	else
	{
		// close the charger
		GPIO_CHARGER_ON(OFF);
		// turn off indicator
		ledOnOff(LED_CHARGER, OFF);
		gProject.chargerOnOff = OFF;

		//
		messageCancel(EventChargerDetectStart);
		gProject.chargerStatus = chargerNotCharging;
		gProject.chargerStartCnt = FALSE;
	}

}

void eventChargerConnected(void)
{
	gProject.chargerConnnected = TRUE;
	messageSend(EventChargerOnOff, ON, 0);
}

void eventChargerDisconnected(void)
{
	gProject.chargerConnnected = FALSE;
	messageSend(EventChargerOnOff, OFF, 0);
}

void eventChargerStatus(uint8 status)
{	
	uint8 lastStatus = gProject.chargerStatus;
	debugMsg("EventChargerLastStatus", lastStatus);
	if(lastStatus != status)
	{
		if(chargerChargingComplete == status)
		{
			gProject.battLevel = BATT_FULL;
			//turn off the charger
			messageSend(EventChargerOnOff, OFF, 0);
		}
		else if(chargerError == status)
		{
			//stop the charger and then start to blink error indicator
			messageSend(EventChargerOnOff, OFF, 0);
		}

		gProject.chargerStatus = status;
	}
}

void eventVolSub(void)
{
	if(gProject.volume > 0)
	{
		gProject.volume --;
	}
	else
	{
		// min volume, close amp
		if(ON == gProject.ampOnOff)
			messageSend(EventAmp, OFF, 0);
	}

	//sendPacket(U_KEY_VOLUME_UP, &gProject.volume, 1);
}

void eventVolAdd(void)
{
	if(0 == gProject.volume)
	{
		// open amp
		messageSend(EventAmp, ON, 0);
	}

	if(gProject.volume < MAX_VOLUME)
	{
		gProject.volume ++;
	}
	else
	{
		// max volume
	}

	//sendPacket(KEY_PLAY, &gProject.volume, 1);
}

void eventBtKey(void)
{
	uint8 sendData = 0;
	ledFlashStart(LED_BT, 1, 1, 1);
	//sendPacket(U_KEY_VOLUME_DOWN, &sendData, 1);
}

void eventPhoneKey(void)
{

}

void eventAmp(uint8 OnOff)
{
	if((ON == OnOff) || (OFF == OnOff))
	{
		if(OnOff != gProject.ampOnOff)
		{
			gProject.ampOnOff = OnOff;
			
			messageCancel(EventVolFadeIn);
			
			if(OnOff)
			{
				// open channel

				//start volume fade in function
				messageSend(EventVolFadeIn, 0, 0);
			}
			else
			{
				// close channel
			}
		}
	}
}

void eventVolFadeIn(uint8 vol)
{
	if(ON == gProject.ampOnOff)
	{
		if(vol < gProject.volume)
		{
			// set volume value

			// fade in next volume step
			messageSend(EventVolFadeIn, vol++, T_MS(100));
		}
		else
		{
			// reach the volume

			// set volume value
		}

		// set volume
	}
}

void messageHandler(const message_type *msg)
{
	if(NULL == msg)
		return;
	
	//debugMsg("Handle message:", msg->event);
	
	switch(msg->event)
	{
		case EventLoopBack:
			//P3.3 ^= 1;
			//debugMsg("EventLoopBack", 0);
			messageSend(EventLoopBack,0, T_SEC(2));
			break;
			
		case EventEnterLimbo:
			debugStr("eventEnterLimbo\n");
			messageSend(EventLoopBack,0, 0);
			eventEnterLimbo();
			break;
			
		case EventPowerKey:
			eventPowerKey();
			break;

		case EventReady:
			eventReady();
			break;
			
		case EventPowerOn:
			debugStr("EventPowerOn\n");
			eventPowerOn();
			break;	
			
		case EventPowerOff:
			debugStr("EventPowerOff\n");
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
			debugStr("EventChargerConnected\n");
			eventChargerConnected();
			break;

		case EventChargerDisconnected:
			debugStr("EventChargerDisconnected\n");
			eventChargerDisconnected();
			break;

		case EventChargerOnOff:
			eventChargerOnOff(msg->msg);
			break;

		case EventChargerStatus:
			debugMsg("EventChargerStatus", msg->msg);
			eventChargerStatus(msg->msg);
			break;

		case EventChargerDetectStart:
			debugStr("EventChargerDetectStart\n");
			gProject.chargerStartCnt = TRUE;
			break;

		case EventVolSub:
			debugStr("eventVolSub\n");
			eventVolSub();
			//ledFadeStart(LED_POWER, FADE_TIME(3000));
			break;

		case EventVolAdd:
			debugStr("eventVolAdd\n");
			eventVolAdd();
			//ledFadeStop();
			break;

		case EventBtKey:
			debugStr("EventBtKey\n");
			eventBtKey();
			break;

		case EventPhoneKey:
			debugStr("eventPhoneKey\n");
			eventPhoneKey();
			break;

		case EventAmp:
			debugMsg("eventAmp:", msg->msg);
			eventAmp(msg->msg);
			break;

		case EventVolFadeIn:
			//debugMsg("EventVolFadeIn:", msg->msg);
			//eventVolFadeIn(msg->msg);
			break;

		default:
			debugMsg("Unknown event:", msg->event);
			break;
	}
}
