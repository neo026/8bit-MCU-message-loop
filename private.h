
#ifndef _PRIVATE_H
#define _PRIVATE_H

#include "macrodriver.h"

// open or close the debug
#define ENABLE_SERIAL	1

#if ENABLE_SERIAL
#include "utils/serial.h"
#define debugMsg	sendStringWithData
#else
#define debugMsg	
#endif

enum
{
	StateStandby,	// do not modify, power up state
	StateLimbo,
	//StateReady,		// power button pressed by someone, but not power on, the unit need to display led first.
	StatePowerOn	// really power on state
};

enum
{
	chargerDisconnected,
	chargerCharging,
	normal,
};

typedef struct
{
	// timers
	uint8 timer1ms;
	uint8 timer100ms;
	
	// delay timer for delay function, exactly
	uint8 delayTimer;

	// machine status
	uint8 state;

	// charger
	uint8 chargerStatus;
	uint8 chargingSpeed;
	uint8 chargingCount;

	// battery
	uint8 battLevel;
	uint8 battLowFlag;
}application_type;

extern application_type gProject;

// Eeror Number
#define EER_NONE		0
#define EER_MEMSET		1
#define EER_MSG_FULL	2


/*----------------GPIO Configuration------------------*/
//i2c interface
#define GPIO_SCK			(P2.1)
#define GPIO_SDA			(P2.1)
#define GPIO_SCK_MODE(x)	(P2.1)
#define GPIO_SDA_MODE(x)	(P2.1)
#define GPIO_LED_POWER(x)	(P2.1)
#define GPIO_LED_CHARGER(x)	(P2.1)

//
#define GPIO_BN_POWER		(P12.0)

/*----------------Functions-------------------------*/
void delay1ms(uint8 count);
void initData(void);
void myMemset(const void *src, uint8 value, uint8 len);
void runningStop(uint8 errno);

#define interruptDisable()		do{}while(0)
#define interruptEnable()		do{}while(0)
#define enterSleep()			do{}while(0)

#endif // _PRIVATE_H

