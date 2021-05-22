#include "i2c.h"

void PCAInitialize(uint8_t addr);
void PCASetFrequency(uint8_t addr, uint32_t frequency);
void PCASetChannel(uint8_t addr, uint8_t channel, uint16_t up, uint16_t down);
void PCASetDuty100(uint8_t addr, uint8_t channel, uint8_t duty, uint8_t phaseDegree);
