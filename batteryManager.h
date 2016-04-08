/*
* Battery manager header code
*/

#ifndef _BATTERY_MANAGER_H
#define _BATTERY_MANAGER_H

#define MAX_AV_COUNT           3
#define MAX_AAV_COUNT          9

enum
{
    temperature_unnormal,
    temperature_normal
};

enum
{
	BATT_L00 = 0x0l,
    BATT_L01,	// do not to modify
    BATT_L02,
    BATT_L03,
    BATT_L04,
    BATT_L05,
    BATT_L06,
    BATT_L07,
    BATT_L08,
    BATT_L09,
    BATT_L10,
    BATT_L11,
    BATT_L12,
    BATT_L13,
    BATT_L14,
    BATT_L15,
    BATT_L16,
    BATT_L17,
    BATT_L18,
    BATT_L19,
    BATT_L20,

    BATT_EMPTY      = BATT_L00,
    BATT_LOW2       = BATT_L05,
    BATT_LOW1       = BATT_L07,
    BATT_NORMAL1    = BATT_L10,
    BATT_NORMAL2    = BATT_L15,
    BATT_FULL       = BATT_L20,

	BATT_TOP
};

/*
*
*/
typedef struct
{
	uint8 initialising;							// check if this is the firt time to handle battery monitor

    uint8 currentVoltArray[MAX_AV_COUNT];		/* CV,  the current level of Battery voltage*/
    uint8 currentAverVolt;						/* AV,  the current average voltage value of battery, this is for LED PWM */
    uint8 averageVoltArrary[MAX_AAV_COUNT];     /* AV arrary, the average voltage arrary of battery,*/
    uint8 averageAverVolt;						/* AAV, this is for low battery display */
    
    uint8 minVolt;								/*MV, the min adc value of battery voltage*/
	uint16 chargingCount;						// record the charging time
	uint8 battChargingStep;						// record the charging step
    uint8 temperatureStatus;					// record the temperature status;
}battery_type;

void checkChargingTime(void);
void batteryMonitor(void);
uint8 batteryIsNormal(void);
void batteryChargerStatus(void);

#endif  // _BATTERY_MANAGR_H

