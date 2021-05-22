#include "pca9685.h"

enum PCA9685 {
	 PCA9685_MODE1 = 0x00,
	 PCA9685_MODE1_RESTART = 0x80,
	 PCA9685_MODE_TOTEM = 0x04, // TODO...
	 PCA9685_CH0 = 0x06
};

void PCAInitialize(uint8_t addr) {
	uint8_t cmds[] = {PCA9685_MODE1, PCA9685_MODE1_RESTART};
	I2CWrite(addr, cmds, sizeof(cmds));
}

void PCASetFrequency(uint8_t addr, uint32_t frequency) {
	
}

void PCASetMode(uint8_t mode) {
	I2CRead(addr, );
}

void PCASetChannel(uint8_t addr, uint8_t channel, uint16_t up, uint16_t down) {
	uint8_t cmds[] = {PCA9685_CH0 + 4 * channel, up, up >> 8, down, down >> 8};
	I2CWrite(addr, cmds, sizeof(cmds));
}

void PCASetDuty100(uint8_t addr, uint8_t channel, uint8_t duty, uint8_t phaseDegree) {
	
}
