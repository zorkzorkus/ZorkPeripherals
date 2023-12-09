// License: MIT
// Adapted from Adafruit Arduino Library
// https://github.com/adafruit/Adafruit_SSD1306
// Copyright 2023 (c) Zork 'zorkzorkus' Zorkus
//                (c) Adafruit Industries

#include "ssd1306.h"

#define SSD1306_COMMAND    0x00
#define SSD1306_DATA       0x40

#define SSD1306_MEMORYMODE          0x20
#define SSD1306_COLUMNADDR          0x21
#define SSD1306_PAGEADDR            0x22
#define SSD1306_SETSTARTLINE        0x40
#define SSD1306_SETCONTRAST         0x81
#define SSD1306_CHARGEPUMP          0x8D
#define SSD1306_MIRROR_X_OFF        0xA0
#define SSD1306_MIRROR_X_ON         0xA1
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON        0xA5
#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_INVERTDISPLAY       0xA7
#define SSD1306_SETMULTIPLEX        0xA8
#define SSD1306_DISPLAYOFF          0xAE
#define SSD1306_DISPLAYON           0xAF
#define SSD1306_MIRROR_Y_OFF        0xC0
#define SSD1306_MIRROR_Y_ON         0xC8
#define SSD1306_SETDISPLAYOFFSET    0xD3
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5
#define SSD1306_SETPRECHARGE        0xD9
#define SSD1306_SETCOMPINS          0xDA
#define SSD1306_SETVCOMDETECT       0xDB

void SSD1306_Init(SSD1306* ssd, uint8_t addr) {

	ssd->addr = addr;
	ssd->width = 128;
	ssd->height = 64;

	const uint8_t initArray[] = {
		0, SSD1306_DISPLAYOFF,
		1, SSD1306_SETMULTIPLEX, 0x3f,
		1, SSD1306_MEMORYMODE, 0x00,
		2, SSD1306_COLUMNADDR, 0, 0xff,
		2, SSD1306_PAGEADDR, 0, 0x07,
		0, SSD1306_SETSTARTLINE,
		1, SSD1306_SETDISPLAYOFFSET, 0x00,
		0, SSD1306_MIRROR_X_ON,
		0, SSD1306_MIRROR_Y_ON,
		1, SSD1306_SETCOMPINS, 0x12,
		1, SSD1306_SETCONTRAST, 0x7f,
		0, SSD1306_DISPLAYALLON_RESUME,
		0, SSD1306_NORMALDISPLAY,
		1, SSD1306_SETDISPLAYCLOCKDIV, 0x80,
		1, SSD1306_SETPRECHARGE, 0xc2,
		1, SSD1306_SETVCOMDETECT, 0x20,
		1, SSD1306_CHARGEPUMP, 0x14,
		0, SSD1306_DISPLAYON
	};

	for (int i = 0; i < sizeof (initArray);) {
		ssd->f_cmdWrite(addr, SSD1306_COMMAND, (uint8_t*) initArray + i + 1, initArray[i] + 1);
		i += initArray[i] + 2;
	}

}

void SSD1306_SetArea(SSD1306* ssd, uint8_t xStart, uint8_t xEnd, uint8_t yStart, uint8_t yEnd) {
	uint8_t cmd1[] = {SSD1306_COLUMNADDR, xStart, xEnd};
	uint8_t cmd2[] = {SSD1306_PAGEADDR, yStart, yEnd};
	ssd->f_cmdWrite(ssd->addr, SSD1306_COMMAND, cmd1, sizeof (cmd1));
	ssd->f_cmdWrite(ssd->addr, SSD1306_COMMAND, cmd2, sizeof (cmd2));
}

void SSD1306_StreamData(SSD1306* ssd, uint8_t* data, uint16_t len) {
	ssd->f_cmdWrite(ssd->addr, SSD1306_DATA, data, len);
}

void SSD1306_Fill(SSD1306* ssd, uint8_t data) {
	ssd->f_start();
	if (!ssd->f_byteWrite(ssd->addr << 1)) {
		ssd->f_stop();
		return;
	}
	if (!ssd->f_byteWrite(SSD1306_DATA)) {
		ssd->f_stop();
		return;
	}
	for (int x = 0; x < ssd->width; ++x) {
		for (int y = 0; y < ssd->height / 8; ++y) {
			if (!ssd->f_byteWrite(data)) {
				ssd->f_stop();
				return;
			}
		}
	}
	ssd->f_stop();
}
