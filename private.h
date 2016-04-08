
#ifndef _PRIVATE_H
#define _PRIVATE_H

#include "macrodriver.h"

#ifdef ENABLE_SERIAL
#include "utils/serial.h"
#define debugMsg	serialSendStringWithData
#define debugStr	serialSendString
#define debugNum	serialSendNum
#else
#define debugMsg
#define debugStr
#define debugNum
#endif

enum
{
	StateStandby,	// do not modify, power up state
	StateLimbo,
	StateReadyOn,		// power button pressed by someone, but not power on, the unit need to display led first.
	StatePowerOn	// really power on state
};

enum
{
	chargerNotCharging,
	chargerError,
	chargerCharging,
	chargerChargingComplete,

	normal,	// this is for chargerSpeed
};

typedef struct
{
	// timers
	uint8 timerMsgTick;		// timer for message loop
	uint8 timerTick;		// timer tick, uint 500us
	uint8 timerDelay;		// timer for delay function

	// machine status
	uint8 state;

	// charger
	uint8 chargerConnnected;
	uint8 chargerOnOff;
	uint8 chargerStartCnt;
	uint8 chargerStatus;
	uint8 chargingSpeed;
	
	// battery
	uint8 battLevel;
	uint8 battLowFlag;

	//amplifier
	uint8 ampOnOff;

	uint8 volume;
}application_type;

extern application_type gProject;

// Error Number
#define ERR_NONE		0
#define ERR_MEMSET		1
#define ERR_MSG_FULL	2
#define ERR_TX_FULL		3


// LED 
#define LED_NONE		0x00
#define LED_POWER		(1 << 0)
#define LED_CHARGER		(1 << 1)
#define LED_VOLSUB		(1 << 2)
#define LED_VOLADD		(1 << 3)
#define LED_BT			(1 << 4)
#define LIMITLESS		0x00


/*----------------GPIO Configuration------------------*/
//i2c interface
#define GPIO_SCK				(P2.1)
#define GPIO_SDA				(P2.1)
#define GPIO_SCK_MODE(x)		(P2.1)
#define GPIO_SDA_MODE(x)		(P2.1)

#define GPIO_LED_POWER(x)		(P0.0 = x)
#define GPIO_LED_CHARGER(x)		(P3.3 = x)
#define GPIO_LED_VOLSUB(x)		(P7.2 = x)
#define GPIO_LED_VOLADD(x)		(P7.1 = x)
#define GPIO_LED_BT(x)			(P7.0 = x)
//Keys
#define GPIO_BN_POWER			(P12.0)
//charger
#define GPIO_CHARGER			(P14.0)
#define GPIO_CHARGER_ON(x)		(P2.3 = x)
#define GPIO_CHARGER_LED		(P2.2)
// aux audio in

/*----------------Functions-------------------------*/
void delay1ms(uint8 count);
void initData(void);
void myMemset(const void *src, uint8 value, uint8 len);
void runningStop(uint8 errno);

#define interruptDisable()		do{DI();}while(0)
#define interruptEnable()		do{EI();}while(0)
#define enterSleep()			do{STOP();}while(0)

#endif // _PRIVATE_H

