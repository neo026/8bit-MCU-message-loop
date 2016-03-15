
#ifndef _MESSAGE_H
#define _MESSAGE_H

//index 0x00 ~ 0x05, for sepecial events, don't modify
#define EventEmpty					0x00		// special event

// general events
#define	EventEnterLimbo				0x06
#define	EventReady					0x07
#define	EventPowerOn				0x08
#define	EventPowerOff				0x09
#define	EventBatteryMonitor			0x0A
#define	EventBatteryLow				0x0B
#define	EventChargerConnected		0x0C
#define	EventChargerDisconnected	0x0D
#define	EventChargerOnOff			0x0E
#define	EventChargerError			0x0F
#define	EventChargerCompleted		0x10
#define EventVolSub					0x11
#define EventVolAdd					0x12
#define EventMode					0x13
#define EventPhoneKey				0x14

//message struct, it is very simple.
typedef struct _message_type
{
	uint8	event;					// message id or event
	uint8	msg;					// the message data
	uint16	interval;				// 1ms unit, 500us ~ 32.767s, unit 500us
}message_type;

//this defines is for the interval time of messageSend().
#define T_MS(x)		(uint16)((x)*2)
#define T_SEC(x)	(uint16)((x)*2*1000)	// this input time range is 0~32768, so 'x' value is 0~32

void messageInit(void);
void messageAdd(uint8 event, uint8 mdata, uint16 interval);
void messageCancel(uint8 event);
void messageLoop(void);
// Note that the content of the 'msg' do not to modify
void messageHandler(const message_type *msg);

#define messageSend(event, mdata, interval)					messageAdd(event, mdata, interval)
#define messageSendConditionally(event, mdata, condition)

#endif	// _MESSAGE_H

