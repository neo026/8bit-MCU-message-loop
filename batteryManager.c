/*
*   Battery manager code
*   author: Neo Gan
*/
#include "private.h"
#include "batteryManager.h"
#include "ad.h"
#include "message.h"

#define CHARGING_NORMAL_INTERVAL            (3600)           
#define CHARGING_HIGH_INTERVAL              (2400)

static battery_type theBattery;

/*
*   update the voltage level.
*/
static uint8 getAdcLevel(uint16 adcValue)
{
	uint8 level;
#if 0
    if(adcValue < )             // battery is pluged out by someone
        level = BATT_L00;
    else if(adcValue < )        // battery is too low, must be shut down machine
        level = BATT_L01;
    else if(adcValue < )        
        level = BATT_L02;
    else if(adcValue < )        
        level = BATT_L03; 
    else if(adcValue < )        
        level = BATT_L04; 
    else if(adcValue < )        
        level = BATT_L05;
    else if(adcValue < )        
        level = BATT_L06; 
    else if(adcValue < )       
        level = BATT_L07;
    else if(adcValue < )       
        level = BATT_L08;
    else if(adcValue < )       
        level = BATT_L09;
    else if(adcValue < )       
        level = BATT_L10;
    else if(adcValue < )       
        level = BATT_L11;
    else if(adcValue < )       
        level = BATT_L12;
    else if(adcValue < )       
        level = BATT_L13;
    else if(adcValue < )       
        level = BATT_L14;
    else if(adcValue < )       
        level = BATT_L15;
    else if(adcValue < )       
        level = BATT_L16;
    else if(adcValue < )       
        level = BATT_L17;
    else if(adcValue < )       
        level = BATT_L18;
    else if(adcValue < )       
        level = BATT_L19;
    else     
        level = BATT_L20;
#endif
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

	theBattery.initialising = TRUE;
}

/*
* get and handle battery temperature status. 
* NOTE, battery is not pluged in, but MCU can runing normal
*/
void checkBatteryTemperature(void)
{
    uint16 tmpValue;
    
    tmpValue = getAdcValue(batteryTemperature);

   if(chargerCharging ==  gProject.chargerStatus)
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
    if(chargerCharging ==  gProject.chargerStatus)
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
    for(level = 0, i = 1; i < MAX_AV_COUNT; i ++)
        level += theBattery.currentVoltArray[i];

    level /= MAX_AV_COUNT;

    // 3, update the minimal battery voltage in memory
    if(level < theBattery.minVolt)
        theBattery.minVolt = level;

    // 4, calulate the average voltage of battery, for Battery voltage display quickly
    theBattery.currentAverVolt = (theBattery.minVolt + level) / 2;

    // 5, update average voltage arrary
    for(i = 1; i < MAX_AAV_COUNT; i ++)
        theBattery.averageVoltArrary[i - 1] = theBattery.averageVoltArrary[i];

    theBattery.averageVoltArrary[i - 1] = theBattery.currentAverVolt;

    // 6, update the Average Average voltage value, for Battery low display
    for(level = 0, i = 0; i < MAX_AAV_COUNT; i++)
        level += theBattery.averageVoltArrary[i];
        
    theBattery.averageAverVolt = level / MAX_AAV_COUNT;

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
		if(StatePowerOn == gProject.state)
			messageSend(EventPowerOff, 0, 0);
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
    if(chargerCharging ==  gProject.chargerStatus)
    {
    	// some machines have two ways to  charge battery.
       if(normal == gProject.chargingSpeed)
        {
            if(gProject.chargingCount > CHARGING_NORMAL_INTERVAL)
                setFlag = 1;
        }
        else
        {
            if(gProject.chargingCount > CHARGING_HIGH_INTERVAL)
                setFlag = 1;
        }

		// charging interval timeout, change the battery step for indication
        if(setFlag)
        {
            gProject.chargingCount = 0;
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
			gProject.chargingCount = 0;
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

	
	// handling the battery indication		
	batteryProcess();
	
}

void batteryMonitor(void)
{
	if(theBattery.initialising)
		batteryLoop();				// as a Loop function, you can put this function into main() while(1), for saving a message space
	else
		batteryInit();

	messageSend(EventBatteryMonitor, 0, 100);
}

