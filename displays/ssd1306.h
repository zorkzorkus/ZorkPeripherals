// License: MIT
// Adapted from Adafruit Arduino Library
// https://github.com/adafruit/Adafruit_SSD1306
// Copyright 2023 (c) Zork 'zorkzorkus' Zorkus
//                (c) Adafruit Industries

#ifndef SSD1306_H
#define SSD1306_H

#include "i2c.h"

typedef struct {
	uint8_t addr;
	uint8_t width;
	uint8_t height;
	I2C_CmdWrite_f f_cmdWrite;
	I2C_Start_f f_start;
	I2C_ByteWrite_f f_byteWrite;
	I2C_Stop_f f_stop;
} SSD1306;

void SSD1306_Init(SSD1306* ssd, uint8_t addr);
void SSD1306_SetArea(SSD1306* ssd, uint8_t xStart, uint8_t xEnd, uint8_t yStart, uint8_t yEnd);
void SSD1306_StreamData(SSD1306* ssd, uint8_t* data, uint16_t len);
void SSD1306_Fill(SSD1306* ssd, uint8_t data);

#endif
