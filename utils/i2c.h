
#ifndef _I2C_H
#define _I2C_H

void i2cInit(void);
void i2cStart(void);
void i2cStop(void);
uint8 i2cWriteByte(uint8 value);
uint8 i2cReadByte(uint8 once);

#endif


