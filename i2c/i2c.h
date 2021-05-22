#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stdbool.h>

// Use your preferred type:
typedef uint8_t length_t;

bool I2CInit();
bool I2CWrite(uint8_t addr, uint8_t* data, length_t length);
bool I2CRead(uint8_t addr, uint8_t* data, length_t length);

#endif // I2C_H
