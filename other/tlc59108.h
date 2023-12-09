// License: MIT
// Copyright 2023 (c) Zork 'zorkzorkus' Zorkus

#pragma once

#include "i2c.h"

typedef struct {
	uint8_t addr;
	I2C_Write_f write;
	I2C_Read_f read;
} TLC59108;

#define TLC59108_MODE_BLINKING 1
#define TLC59108_MODE_DIMMING 0
#define TLC59108_LED_OFF 0
#define TLC59108_LED_FULLON 1
#define TLC59108_LED_PWM 2
#define TLC59108_LED_GRPPWM 3


void TLC59108_Init(TLC59108* tlc, uint8_t addr, I2C_Write_f f_write, I2C_Read_f f_read);
void TLC59108_SetLed(TLC59108* tlc, uint8_t channel, uint8_t state);
void TLC59108_SetPwm(TLC59108* tlc, uint8_t channel, uint8_t pwm);
void TLC59108_SetMode(TLC59108* tlc, uint8_t mode);
void TLC59108_SetGroupPwm(TLC59108* tlc, uint8_t pwm);
void TLC59108_SetGroupFreq(TLC59108* tlc, uint8_t freq);
void TLC59108_SetRef(TLC59108* tlc, uint8_t iref);
uint8_t TLC59108_SetCurrent(TLC59108* tlc, uint8_t current, uint16_t resistor);
