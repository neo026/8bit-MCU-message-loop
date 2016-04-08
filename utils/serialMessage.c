/*


*/
#if ENABLE_SERIAL
#include "../macrodriver.h"
#include "../message.h"
#include "serial.h"

/* */
void cmdPhoneNumber(uint8 startIndex, uint8 data_size)
{
	uint8 i;
	uint8 buffer[11];
	if(data_size > 11)

	for(i = 0; i < data_size; i++)
	{
		buffer[i] = theSerial.rxBuffer[startIndex ++];
	}
}

void cmdWriteEeprom(uint8 data_size)
{
	uint8 ackBuff[] = "OK";
	//sendPacket(CMD_SETLED, ackBuff, sizeof(ackBuff));
}

/* command handle*/
void cmdHandler(uint8 cmd, uint8 startIndex, uint8 data_size)
{
	switch(cmd)
	{
		case CMD_POWERON:
			messageSend(EventPowerOn, 1, 0);
			break;

		case CMD_PHONE_NUMBER:
			cmdPhoneNumber(startIndex, data_size);
			break;

		case CMD_SET_LED:
			cmdWriteEeprom(data_size);
			break;
			
		default:
			break;
	}
}

#else
	static char dumy = 0;
#endif