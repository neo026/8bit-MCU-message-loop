
#include "private.h"
#include "i2c.h"

/*
Note:
	Set SDA and SCK as input mode at idle mode, because SDA and SCK have pull-up risistors, it can reduce the consumption
*/

#define drive_input		1
#define drive_output	0

void i2cInit(void)
{
	//set SDA and SCK as input mode.
	GPIO_SDA_MODE(drive_input);
	GPIO_SCK_MODE(drive_input);
}

void i2cStart(void)
{
	//change SDA and SCK as output mode first.
	GPIO_SDA_MODE(drive_output);
	GPIO_SCK_MODE(drive_output);
	NOP();NOP();NOP();NOP();
	GPIO_SDA = 1;
	GPIO_SCK = 1;
	NOP();NOP();NOP();NOP();
	GPIO_SDA = 0;
	NOP();NOP();NOP();NOP();
	GPIO_SCK = 0;
}

void i2cStop(void)
{
	GPIO_SCK = 1;
	NOP();NOP();NOP();NOP();
	GPIO_SDA = 0;
	NOP();NOP();NOP();NOP();
	//change SDA and SCK as input mode for reducing consumption.
	GPIO_SDA_MODE(drive_input);
	GPIO_SCK_MODE(drive_input);
}
//
uint8 i2cWriteByte(uint8 value)
{
	uint8 i = 8, retVal;

	while(i --)
	{
		GPIO_SDA = (value & 0x80) ? 1:0;		
		value <<= 1;
		GPIO_SCK = 1;
		NOP();NOP();NOP();NOP();
		GPIO_SCK = 0;
	}

	// read acknowledge signal
	GPIO_SDA_MODE(drive_input);
	NOP();NOP();NOP();
	GPIO_SCK = 1;
	NOP();NOP();NOP();
	retVal = GPIO_SDA;
	//change SDA as output mode
	GPIO_SDA_MODE(drive_output);
	
	return retVal;
}

//once ->0 continue read, once ->1 one time read
uint8 i2cReadByte(uint8 once)
{
	uint8 i = 8, retVal = 0;

	// change SDA as input mode firstly;
	GPIO_SDA_MODE(drive_input);
	NOP();NOP();NOP();
	
	while(i --)
	{
		retVal <<= 1;
		GPIO_SCK = 1;
		NOP();NOP();NOP();
		retVal |= (GPIO_SDA) ? 1:0;
		GPIO_SCK = 0;
	}

	//send acknowledge signal
	GPIO_SDA_MODE(drive_output);
	NOP();NOP();NOP();
	GPIO_SDA = once ? 1:0;
	NOP();NOP();NOP();
	GPIO_SCK = 1;
	NOP();NOP();NOP();
	GPIO_SCK = 0;

	//

	return retVal;
}
