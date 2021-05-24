#include "pca9685.h"

enum PCA9685 {
    PCA9685_REG_MODE1 = 0x00,
    PCA9685_REG_MODE2 = 0x01,
    PCA9685_REG_CH0 = 0x06,
    PCA9685_REG_PRESCALE = 0xFE,
    PCA9685_MODE1_SLEEP = 0x10
};

void PCAInitialize(uint8_t addr) {
    I2CWriteReg8(addr, PCA9685_REG_MODE1, 0);
}

void PCASetFrequency(uint8_t addr, uint32_t frequency) {
    uint32_t preScalerVal = (25000000UL / (4096 * frequency)) - 1;
    if (preScalerVal > 255) preScalerVal = 255;
    if (preScalerVal < 3) preScalerVal = 3;
    uint8_t mode;
    I2CReadReg8(addr, PCA9685_REG_MODE1, &mode);
    mode |= PCA9685_MODE1_SLEEP;
    I2CWriteReg8(addr, PCA9685_REG_MODE1, mode);
    I2CWriteReg8(addr, PCA9685_REG_PRESCALE, (uint8_t) preScalerVal);
    mode &= ~(PCA9685_MODE1_SLEEP);
    I2CWriteReg8(addr, PCA9685_REG_MODE1, mode);
}

void PCASetChannel(uint8_t addr, uint8_t channel, uint16_t up, uint16_t down) {
    I2CWriteReg8(addr, (uint8_t)(PCA9685_REG_CH0 + 4 * channel), (uint8_t)up);
    I2CWriteReg8(addr, (uint8_t)(PCA9685_REG_CH0 + 4 * channel + 1), (uint8_t)(up >> 8));
    I2CWriteReg8(addr, (uint8_t)(PCA9685_REG_CH0 + 4 * channel + 2), (uint8_t)down);
    I2CWriteReg8(addr, (uint8_t)(PCA9685_REG_CH0 + 4 * channel + 3), (uint8_t)(down >> 8));
}

void PCASetDuty100(uint8_t addr, uint8_t channel, uint8_t duty, uint16_t phaseDegree) {
    uint32_t down = ((uint32_t)duty) * 4096 / 100;
    uint32_t up = 4096 * ((uint32_t)phaseDegree) / 360 ;
    down = (up + down) % 4096;
    PCASetChannel(addr, channel, (uint16_t) up, (uint16_t) down);
}
