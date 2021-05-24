#ifndef I2C_H
#define	I2C_H

#include <stdint.h>
#include <stdbool.h>

// Use your preferred type:
typedef uint8_t i2c_length;

void I2CInit();
bool I2CWriteReg8(uint8_t addr, uint8_t reg, uint8_t value);
bool I2CReadReg8(uint8_t addr, uint8_t reg, uint8_t* pValue);
i2c_length I2CWrite(uint8_t addr, uint8_t* buffer, i2c_length len);
i2c_length I2CRead(uint8_t addr, uint8_t* buffer, i2c_length len);

#endif	/* I2C_H */
