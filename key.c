
#include "private.h"
#include "key.h"
#include "ad.h"
#include "message.h"

static key_type theKey;

static void keyPress(uint8 keyno)
{
	switch(keyno)
	{
		case KEY_POWER:
			messageSend(EventPowerOn, 0, 0);
			break;

		case KEY_VOLSUB:
			messageSend(EventVolSub, 0, 0);
			break;

		case KEY_VOLADD:
			messageSend(EventVolAdd, 0, 0);
			break;

		case KEY_BT:
			messageSend(EventMode, 0, 0);
			break;

		case KEY_PHONE:
			messageSend(EventPhoneKey, 0, 0);
			break;

		default:
			break;
	}
}

static void longPress(uint8 keyno)
{

}

static void vLongPress(uint8 keyno)
{

}

static void shortPress(uint8 keyno)
{

}

static void longPressRelease(uint8 keyno)
{

}

static void vLongPressRelease(uint8 keyno)
{

}

static uint8 getPressKey(void)
{
	uint8 key = KEYNO0;
	uint16 value;

	/*Power key press or not*/
	if(GPIO_BN_POWER == 0)
	{
		key = KEY_POWER;
		return key;
	}
	
	/*Get ADC*/
	value = getAdcValue(ADC_KEY);
	
	if(value < (KEY1 + KEYT))
	{
		key |= KEY_VOLSUB;
	}
	else if(value > (KEY2 - KEYT) && value < (KEY2 + KEYT))	
	{
		key |= KEY_VOLADD;
	}
	else if(value > (KEY3 - KEYT) && value < (KEY3 + KEYT))
	{
		key |= KEY_BT;
	}
	else if(value > (KEY4 - KEYT) && value < (KEY4 + KEYT))
	{
		key |= KEY_PHONE;
	}
	else if(value > (KEY0 - KEYT -KEYT))
	{
		key |= KEYNO0;
	}
	else
	{
		key = KEYERR;
	}
	
	return key;
}

void keyInit(void)
{
	theKey.keyHoldTimeCnt = 0;
	theKey.keyStatus = noPressFlag;
	theKey.keyValueLast= KEYNO0;
	theKey.keyValue = KEYNO0;
}

void keyLoop(void)
{
	uint8 temp = getPressKey();

	if(KEYERR == temp)
		return;

	theKey.keyValueLast = theKey.keyValue;
	theKey.keyValue	 = temp;

	if((KEYNO0 == theKey.keyValue) && (KEYNO0 == theKey.keyValueLast))
		return;

	if(KEYNO0 == theKey.keyValue)
	{
		/*Key release  handle*/
		if(shortPressFlag == theKey.keyStatus)
		{
			/*Short press release*/
			shortPress(theKey.keyValueLast);
		}
		else if(longPressFlag == theKey.keyStatus)
		{
			/*Long press release*/
			longPressRelease(theKey.keyValueLast);
		}
		else if(vLongPressFlag == theKey.keyStatus)
		{
			/*Very long press release*/
			vLongPressRelease(theKey.keyValueLast);
		}

		/*Remember Clear this status*/
		theKey.keyStatus = noPressFlag;
	}
	else if(theKey.keyValueLast == theKey.keyValue)
	{
		/*Key hold handle*/
		theKey.keyHoldTimeCnt ++;
		if(shortPressFlag == theKey.keyStatus)
		{
			if(theKey.keyHoldTimeCnt >= KEYHOLD3S)
			{
				/*long press*/
				longPress(theKey.keyValueLast);
				theKey.keyStatus = longPressFlag;
			}
		}
		else if(longPressFlag == theKey.keyStatus)
		{
			if(theKey.keyHoldTimeCnt > KEYHOLD5S)
			{
				vLongPress(theKey.keyValueLast);
				theKey.keyStatus = vLongPressFlag;
			}
		}

		/*Here add Repeat and VVLong Press handle*/
			
	}
	else
	{
		if(KEYNO0 != theKey.keyValueLast)
		{
			/*Key typematic handle*/
			if(shortPressFlag == theKey.keyStatus)
			{
				/*Short press release*/
				shortPress(theKey.keyValueLast);
			}
			else if(longPressFlag == theKey.keyStatus)
			{
				/*Long press release*/
				longPressRelease(theKey.keyValueLast);
			}
			else if(vLongPressFlag == theKey.keyStatus)
			{
				/*Very long press release*/
				vLongPressRelease(theKey.keyValueLast);
			}

			theKey.keyValue = KEYNO0;
			theKey.keyStatus = noPressFlag;
		}
		else
		{
			/*Key press handle*/
			/*debounce*/
			for(temp = 80; temp > 0; temp --);
			/*Get the key value again*/
			temp = getPressKey();

			if(temp == theKey.keyValue)
			{
				keyPress(theKey.keyValue);
				theKey.keyStatus = shortPressFlag;
			}
			else
			{
				theKey.keyValue = KEYNO0;
				theKey.keyStatus = noPressFlag;
			}

		}

		theKey.keyHoldTimeCnt = 0;
	}
}

