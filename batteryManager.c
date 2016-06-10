/*
*   Battery manager code
*   author: Neo Gan
*/
#include "private.h"
#include "batteryManager.h"
#include "ad.h"
#include "message.h"

#define CHARGING_NORMAL_INTERVAL            (1200) // unit: 1sec         
#define CHARGING_HIGH_INTERVAL              (600)

static battery_type theBattery;

#define DBG_BAT_ENABLE 1


uint16 adcValueTbl[] =
{
	702,
	713,
	724,
	735,
	746,
	757,
	769,
	780,
	791,
	802,
	813,
	824,
	835,
	847,
	858,
	869,
	880,
	891,
	902,
	913,
	925
	
};

/*
*   update the voltage level.
*/
static uint8 getAdcLevel(uint16 adcValue)
{
	uint8 level;

	for(level = BATT_L00; level < BATT_FULL; level ++)
	{
		if(adcValue < adcValueTbl[level])
			break;
	}

	return level;
}

/*
*   Battery Initial
*/
void batteryInit(void)
{
    uint16 tmpValue;
    uint8 i;
	uint8 level;

	#if DBG_BAT_ENABLE
		debugStr("battery Initial\n");
	#endif
	
    // get battery voltage first
    for(tmpValue = 0, i = 0 ; i < MAX_AV_COUNT; i ++)
        tmpValue += getAdcValue(batteryVoltage);

	// average
	tmpValue /= MAX_AV_COUNT;

	// get the level of voltage according to the table
	level = getAdcLevel(tmpValue);
	
	// save into the currrent voltage array
	for(i = 0; i < MAX_AV_COUNT; i ++)
		theBattery.currentVoltArray[i] = level;
	
    // set average arrary
    for(i = 0; i < MAX_AAV_COUNT; i ++)
        theBattery.averageVoltArrary[i] = level;
	
    // save into current voltage
    theBattery.currentAverVolt = level;

	//save into average average voltage
	theBattery.averageAverVolt = level;

	// keep the save
	theBattery.battChargingStep = level;

	// set the battery level
	gProject.battLevel = level;

    // set the minimal value, set a middle value
    theBattery.minVolt = level;

    // set the power supply type, this is determined by hardware limit
   // gProject.powerSupplyType = fromCharging;

	// do some indication when battery low

	theBattery.temperatureStatus = temperature_normal;
	gProject.chargingSpeed = normal;
	
	theBattery.initialising = TRUE;
}

void checkChargingTime(void)
{
	// when charging, we need to increase the charging time(uint:1sec)
	if(chargerCharging == gProject.chargerStatus)
	{
		theBattery.chargingCount ++;
	}
	else
	{
		// when charging stop, we need to decrease the charging time, do not clear immediately, for such a situation:
		// if charger plug out when do not reach the setting charging time,
		// and then charger plug in again, battery level will not  increase, but actually the battery voltage was up slowly
		if(theBattery.chargingCount > 0)
			theBattery.chargingCount --;
	}

}

/*
* get and handle battery temperature status. 
* NOTE, battery is not pluged in, but MCU can runing normal
*/
void checkBatteryTemperature(void)
{
    uint16 tmpValue;
    
    tmpValue = getAdcValue(batteryTemperature);

	theBattery.temperatureStatus = temperature_normal;
	return;

   if(ON ==  gProject.chargerOnOff)
   {
        if(tmpValue < 11)
            theBattery.temperatureStatus = temperature_unnormal;
        else if(tmpValue < 11)
            theBattery.temperatureStatus = temperature_normal;
        else
            theBattery.temperatureStatus = temperature_unnormal;
    }
    else
    {
        if(tmpValue < 11)
            theBattery.temperatureStatus = temperature_unnormal;
        else if(tmpValue < 11)
            theBattery.temperatureStatus = temperature_normal;
        else
            theBattery.temperatureStatus = temperature_unnormal;
    }

    // handle the battery temperature status below, such as indications
    if(ON ==  gProject.chargerOnOff)
    {
        // stop charging
        if(temperature_unnormal == theBattery.temperatureStatus)
			messageSend(EventChargerOnOff, OFF, 0);
    }
    else
    {
        // power off immediatly
        if((temperature_unnormal == theBattery.temperatureStatus) && (StatePowerOn == gProject.state))
			messageSend(EventPowerOff, 0, 0);
    }
}

/*
*
*/
void detectAlgorithm(void)
{
    uint8 level;
    uint8 i;

    // 0, get the voltage level
    level = getAdcLevel(getAdcValue(batteryVoltage));

	// 1, remove the head(or first) value, add a new value at tail.
    for(i = 1; i < MAX_AV_COUNT; i ++)
        theBattery.currentVoltArray[i - 1] = theBattery.currentVoltArray[i];

    theBattery.currentVoltArray[i - 1] = level;

    // 2, calculate the current average of battery voltage
    for(level = 0, i = 0; i < MAX_AV_COUNT; i ++)
        level += theBattery.currentVoltArray[i];

    level /= MAX_AV_COUNT;

    // 3, update the minimal battery voltage in memory
    if(level < theBattery.minVolt)
        theBattery.minVolt = level;

	/*debug*/
	#if DBG_BAT_ENABLEx
		debugStr("BAT:\n");
		for(i = 0; i < MAX_AV_COUNT; i++)
			debugNum(theBattery.currentVoltArray[i]);
		
		debugMsg("MV:", theBattery.minVolt);
	#endif

    // 4, calulate the average voltage of battery, for Battery voltage display quickly
    theBattery.currentAverVolt = (theBattery.minVolt + level) / 2;

	#if DBG_BAT_ENABLEx
		debugMsg("CAV:", theBattery.currentAverVolt);
	#endif

    // 5, update average voltage arrary
    for(i = 1; i < MAX_AAV_COUNT; i ++)
        theBattery.averageVoltArrary[i - 1] = theBattery.averageVoltArrary[i];

    theBattery.averageVoltArrary[i - 1] = theBattery.currentAverVolt;

    // 6, update the Average Average voltage value, for Battery low display
    for(level = 0, i = 0; i < MAX_AAV_COUNT; i++)
        level += theBattery.averageVoltArrary[i];
        
    theBattery.averageAverVolt = level / MAX_AAV_COUNT;

	#if DBG_BAT_ENABLEx
		for(i = 0; i < MAX_AAV_COUNT; i++)
			debugNum(theBattery.averageVoltArrary[i]);
		
		debugMsg("AAV:", theBattery.averageAverVolt);
	#endif
}

uint8 batteryIsNormal(void)
{
	return (theBattery.temperatureStatus == temperature_normal) ? TRUE : FALSE;
}

void batteryChargerStatus(void)
{
	static uint8 chargerLedCnt = 0;
	static uint8 chargerLedHighCnt = 0;
	uint8 status;
	
	// charge status detection
	if(gProject.chargerStartCnt && gProject.chargerOnOff == ON)
	{
		chargerLedCnt ++;
		
		if(GPIO_CHARGER_LED)
			chargerLedHighCnt ++;
		
		if(chargerLedCnt ++ > 10)
		{
			if(chargerLedHighCnt > 5)
				status = chargerCharging;
			else if(chargerLedHighCnt < 2)
				status = chargerChargingComplete;
			else
				status = chargerError;

			if(status != gProject.chargerStatus)
				messageSend(EventChargerStatus, status, 0);
				
			chargerLedCnt = 0;
			chargerLedHighCnt = 0;
		}
		
	}
	else
	{
		chargerLedCnt = 0;
		chargerLedHighCnt = 0;
	}
}
void batteryProcess(void)
{
	if(gProject.battLevel < BATT_LOW1)
	{
		if(FALSE == gProject.battLowFlag)
		{
			gProject.battLowFlag = TRUE;
			messageSend(EventBatteryLow, 0, 0);
		}
	}
	else
	{
		//if(StatePowerOn == gProject.state)
		//	messageSend(EventPowerOff, 0, 0);
	}
}

/*
* Battery process
*/
void batteryLoop(void)
{
    uint8 setFlag = 0;
	
	// detect the temperature of battery
    checkBatteryTemperature();

    // always detect the battery voltage ADC value.
    detectAlgorithm();

    // calculate the step by the time, and update the battery step
    if(ON ==  gProject.chargerOnOff)
    {
    	// some machines have two ways to  charge battery.
       if(normal == gProject.chargingSpeed)
        {
            if(theBattery.chargingCount > CHARGING_NORMAL_INTERVAL)
                setFlag = 1;
        }
        else
        {
            if(theBattery.chargingCount > CHARGING_HIGH_INTERVAL)
                setFlag = 1;
        }

		// charging interval timeout, change the battery step for indication
        if(setFlag)
        {
            theBattery.chargingCount = 0;
            if(theBattery.battChargingStep > BATT_FULL)
                theBattery.battChargingStep = BATT_FULL;
			else
				theBattery.battChargingStep ++;
        }

		// change the battery level if the averageAverVolt is less, it would happen when playing music while charging.
		if(theBattery.averageAverVolt < theBattery.battChargingStep)
			gProject.battLevel = theBattery.averageAverVolt;
		else
			gProject.battLevel = theBattery.battChargingStep;
    }
    else
    {		
		// for battery low
		if(theBattery.averageAverVolt < gProject.battLevel)
		{
			gProject.battLevel = theBattery.averageAverVolt;			
			theBattery.battChargingStep = theBattery.averageAverVolt;
			theBattery.chargingCount = 0;
		}
		else
		{
			//battery level have no change
			/* 	1, gProject.battLevel is the exactly voltage of the battery, or called battery level.

				2, when charging, battery level will be set , it choice the lower voltage between 
			   	theBattery.battChargingStep and theBattery.averageAverVolt.
			   	
				3, when charging stop, it will come here, you have to change to exactly battery level.
					that is averageAverVolt poperbly greater than battChargingStep, you need to keep the battChargingStep
			*/
		}
    }

	#if DBG_BAT_ENABLEx
		debugMsg("battLevel:", gProject.battLevel);
		debugStr("chargingCount: ");
		debugNum((uint8)(theBattery.chargingCount >> 8));
		debugNum((uint8)theBattery.chargingCount);
	#endif
	
	// handling the battery indication		
	batteryProcess();
	
}

void batteryMonitor(void)
{
	if(theBattery.initialising)
		batteryLoop();				// as a Loop function, you can put this function into main() while(1), for saving a message space
	else
		batteryInit();

	messageSend(EventBatteryMonitor, 0, T_MS(1000));
}

