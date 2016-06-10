
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
			messageSend(EventPowerKey, 0, 0);
			break;

		case KEY_VOLSUB:
			messageSend(EventVolSub, 0, 0);
			break;

		case KEY_VOLADD:
			messageSend(EventVolAdd, 0, 0);
			break;

		case KEY_BT:
			messageSend(EventBtKey, 0, 0);
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

static void shortPressRelease(uint8 keyno)
{

}

static void longPressRelease(uint8 keyno)
{

}

static void vLongPressRelease(uint8 keyno)
{

}

static void keyRepeat(uint8 keyno)
{

}

static uint8 getPressKey(void)
{
	uint8 key = KEY_NONE;
	uint16 value;

	/*Power key press or not*/
	if(GPIO_BN_POWER == 0)
	{
		key = KEY_POWER;
		//return key;
	}
	
	/*Get ADC*/
	value = getAdcValue(ADC_KEY);

#if 0	/*simple handle method, it will error if there is multi key press at the same time*/
	if(value < KEY1)
		key |= KEY_VOLSUB;
	else if(value < KEY2)
		key |= KEY_VOLADD;
	else if(value < KEY3)
		key |= KEY_BT;
	else if(value < KEY4)
		key |= KEY_PHONE;
	else
		key |= KEY_NONE;
#else	/**/
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
	else if(value > (KEY0 - KEYT - KEYT))
	{
		key |= KEY_NONE;	// key release
	}
	else
	{
		key = KEY_ERR;		// key sample value is not defined
	}

#endif

	return key;
}

void keyInit(void)
{
	theKey.keyStatus = noPressFlag;
	theKey.keyValueLast = KEY_NONE;
    theKey.keyRepeatTimeCnt = 0;
	theKey.keyHoldTimeCnt = 0;
}

void pressKey(void)
{
	/*Key hold handle*/

	//theKey.keyHoldTimeCnt ++;   --> Note don't write it here, it will overflow when someone is always pressing the key
    if(noPressFlag == theKey.keyStatus)
	{
		keyPress(theKey.keyValueLast);
		theKey.keyStatus = shortPressFlag;
	}
	else if(shortPressFlag == theKey.keyStatus)
	{
		theKey.keyHoldTimeCnt ++;
		if(theKey.keyHoldTimeCnt >= KEY_LONG_TIME)
		{
			/*long press*/
			longPress(theKey.keyValueLast);
			theKey.keyStatus = longPressFlag;
		}
	}
	else if(longPressFlag == theKey.keyStatus)
	{
		theKey.keyHoldTimeCnt ++;
		if(theKey.keyHoldTimeCnt > KEY_VLONG_TIME)
		{
			vLongPress(theKey.keyValueLast);
			theKey.keyStatus = vLongPressFlag;
		}
	}

	/*Here add Repeat and VVLong Press handle*/
	theKey.keyRepeatTimeCnt ++;
	if(theKey.keyRepeatTimeCnt > KEY_REPEAT_TIME)
	{
		theKey.keyRepeatTimeCnt = 0;
		keyRepeat(theKey.keyValueLast);
	}
}

void releaseKey(void)
{
	/*Key typematic handle*/
	if(shortPressFlag == theKey.keyStatus)
	{
		/*Short press release*/
		shortPressRelease(theKey.keyValueLast);
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

	theKey.keyStatus = noPressFlag;
    theKey.keyRepeatTimeCnt = 0;
	theKey.keyHoldTimeCnt = 0;
}

void keyLoop(void)
{
	uint8 temp = getPressKey();

	if(KEY_ERR == temp)
		return ;

	// invalid key value
	if((KEY_NONE == temp) && (KEY_NONE == theKey.keyValueLast))
		return;

    if(KEY_NONE != theKey.keyValueLast)
	{
		if(KEY_NONE == temp)		// key was released
		{
			// there is a release key
			releaseKey();
		}
		else
		{
			// there is keep status, or typematic key
			if(temp == theKey.keyValueLast)	// key keep pressing
				pressKey();
			else
				releaseKey();
		}
	}

	theKey.keyValueLast = temp;
#if 0
	if(KEY_NONE == theKey.keyValue)
	{
		/*Key release  handle*/
		if(shortPressFlag == theKey.keyStatus)
		{
			/*Short press release*/
			shortPressRelease(theKey.keyValueLast);
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
		theKey.keyRepeatTimeCnt = 0;
	}
	else if(theKey.keyValueLast == theKey.keyValue)
	{
		/*Key hold handle*/

		//theKey.keyHoldTimeCnt ++;   --> Note don't write it here, it will overflow when someone is always pressing the key
		if(keyDebounceFlag == theKey.keyStatus)
		{
			keyPress(theKey.keyValue);
			theKey.keyStatus = shortPressFlag;
		}
		else if(shortPressFlag == theKey.keyStatus)
		{
			theKey.keyHoldTimeCnt ++;
			if(theKey.keyHoldTimeCnt >= KEY_LONG_TIME)
			{
				/*long press*/
				longPress(theKey.keyValueLast);
				theKey.keyStatus = longPressFlag;
			}
		}
		else if(longPressFlag == theKey.keyStatus)
		{
			theKey.keyHoldTimeCnt ++;
			if(theKey.keyHoldTimeCnt > KEY_VLONG_TIME)
			{
				vLongPress(theKey.keyValueLast);
				theKey.keyStatus = vLongPressFlag;
			}
		}

		/*Here add Repeat and VVLong Press handle*/
		theKey.keyRepeatTimeCnt ++;
		if(theKey.keyRepeatTimeCnt > KEY_REPEAT_TIME)
		{
			theKey.keyRepeatTimeCnt = 0;
			keyRepeat(theKey.keyValueLast);
		}
			
	}
	else
	{
		if(KEY_NONE != theKey.keyValueLast)
		{
			/*Key typematic handle*/
			if(shortPressFlag == theKey.keyStatus)
			{
				/*Short press release*/
				shortPressRelease(theKey.keyValueLast);
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

			theKey.keyValue = KEY_NONE;
			theKey.keyStatus = noPressFlag;
		}
		else
		{
			theKey.keyStatus = keyDebounceFlag;		// this a new press, we need debounce this key
			
			#if 0
			/*Key press handle*/
			/*debounce*/
			delay1ms(20);
			/*Get the key value again*/
			temp = getPressKey();

			theKey.keyStatus = keyDebounceFlag;

			if(temp == theKey.keyValue)
			{
				keyPress(theKey.keyValue);
				theKey.keyStatus = shortPressFlag;
			}
			else
			{
				theKey.keyValue = KEY_NONE;
				theKey.keyStatus = noPressFlag;
			}
			#endif
		}

		theKey.keyRepeatTimeCnt = 0;
		theKey.keyHoldTimeCnt = 0;
	}
#endif
}

