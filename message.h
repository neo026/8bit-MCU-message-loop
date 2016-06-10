
#ifndef _MESSAGE_H
#define _MESSAGE_H

enum
{
//index 0x00 ~ 0x05, for sepecial events, don't modify
	EventEmpty,
	EventLoopBack,
	// general events
	EventEnterLimbo = 0x06,
	EventPowerKey,
	EventReady,
	EventPowerOn,
	EventPowerOff,
	
	EventBatteryMonitor,
	EventBatteryLow,
	
	EventChargerConnected,
	EventChargerDisconnected,
	EventChargerOnOff,
	EventChargerStatus,
	EventChargerDetectStart,
	
	EventVolSub,
	EventVolAdd,
	EventBtKey,
	EventPhoneKey,
	EventAmp,
	EventVolFadeIn

};

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

