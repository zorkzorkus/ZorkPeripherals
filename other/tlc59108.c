// License: MIT
// Copyright 2023 (c) Zork 'zorkzorkus' Zorkus

#include "tlc59108.h"

void TLC59108_Init(TLC59108* tlc, uint8_t addr, I2C_Write_f f_write, I2C_Read_f f_read) {
	uint8_t data[] = { 0x00, 0x01 };
	tlc->addr = addr;
	I2C_Write(tlc->addr, data, 2);
}

void TLC59108_AllInit(TLC59108* tlc) {
	uint8_t data[] = { 0x00, 0x01 };
	I2C_Write(0x90, data, 2);
}

void TLC59108_SetLed(TLC59108* tlc, uint8_t channel, uint8_t state) {
	uint8_t buffer[2];
	if (channel < 4) {
		buffer[0] = 0x0c;
	} else {
		buffer[0] = 0x0d;
		channel -= 4;
	}
	I2C_Write(tlc->addr, buffer, 1);
	I2C_Read(tlc->addr, buffer + 1, 1);
	buffer[1] &= ~(0x03 << (channel * 2));
	buffer[1] |= state << (channel * 2);
	I2C_Write(tlc->addr, buffer, 2);
}

void TLC59108_SetPwm(TLC59108* tlc, uint8_t channel, uint8_t pwm) {
	uint8_t buffer[2] = { 0x02 + channel, pwm };
	I2C_Write(tlc->addr, buffer, 2);
}

void TLC59108_SetMode(TLC59108* tlc, uint8_t mode) {
	uint8_t buffer[2];
	buffer[0] = 0x01;
	I2C_Write(tlc->addr, buffer, 1);
	I2C_Read(tlc->addr, buffer + 1, 1);
	buffer[1] = (mode) ? buffer[1] | 0x20 : buffer[1] & 0xdf;
	I2C_Write(tlc->addr, buffer, 2);
}

void TLC59108_SetGroupPwm(TLC59108* tlc, uint8_t pwm) {
	uint8_t buffer[2] = { 0x0a, pwm };
	I2C_Write(tlc->addr, buffer, 2);
}

void TLC59108_SetGroupFreq(TLC59108* tlc, uint8_t freq) {
	uint8_t buffer[2] = { 0x0b, freq };
	I2C_Write(tlc->addr, buffer, 2);
}

void TLC59108_SetRef(TLC59108* tlc, uint8_t iref) {
	uint8_t buffer[2] = { 0x12, iref };
	I2C_Write(tlc->addr, buffer, 2);
}

uint8_t TLC59108_SetCurrent(TLC59108* tlc, uint8_t current, uint16_t resistor) {
	// WIP
	/*uint8_t maxCurrent = 18750 / resistor;
	uint8_t buffer[2] = { 0x12, 0x00 };
	if (current > maxCurrent) {
		buffer[1] = 0xff;
		I2C_Write(tlc->addr, buffer, 2);
		return maxCurrent;
	} else {
		uint8_t lowCurrent = maxCurrent / 3;
		if (current <= lowCurrent) {
			// CM = 0
			uint16_t vg = current * resistor * 10 / 63;
			if (vg >= 500) {
			}
			uint8_t iref = vg >> 1;
		} else {
			// CM = 1
			uint16_t vg = current * resistor * 10 / 189;
			uint8_t iref = 0x80 | vg >> 1;
		}
		// vg of 250 -> HC_CG = 0x00 minimum
		// vg 0f 500 -> HC_CG = 0x40, HC = 1, CG = 0
		// vg of 992 -> HC_CG = 0x7f maximum
	}
	return 0;*/
}
