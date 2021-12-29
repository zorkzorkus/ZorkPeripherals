// License: MIT
// Adapted from Adafruit Arduino Library
// https://github.com/adafruit/Adafruit-ST7735-Library
// Copyright 2022 (c) Zork 'zorkzorkus' Zorkus
//                (c) Adafruit Industries

#include "st7735.h"

#include <stdio.h>

#define DISPLAY_DELAY 0x80
#define ST77XX_NOP 0x00
#define ST77XX_SWRESET 0x01
#define ST77XX_RDDID 0x04
#define ST77XX_RDDST 0x09
#define ST77XX_SLPIN 0x10
#define ST77XX_SLPOUT 0x11
#define ST77XX_PTLON 0x12
#define ST77XX_NORON 0x13
#define ST77XX_INVOFF 0x20
#define ST77XX_INVON 0x21
#define ST77XX_DISPOFF 0x28
#define ST77XX_DISPON 0x29
#define ST77XX_CASET 0x2A
#define ST77XX_RASET 0x2B
#define ST77XX_RAMWR 0x2C
#define ST77XX_RAMRD 0x2E
#define ST77XX_PTLAR 0x30
#define ST77XX_TEOFF 0x34
#define ST77XX_TEON 0x35
#define ST77XX_MADCTL 0x36
#define ST77XX_COLMOD 0x3A
#define ST77XX_MADCTL_MY 0x80
#define ST77XX_MADCTL_MX 0x40
#define ST77XX_MADCTL_MV 0x20
#define ST77XX_MADCTL_ML 0x10
#define ST77XX_MADCTL_RGB 0x00
#define ST77XX_RDID1 0xDA
#define ST77XX_RDID2 0xDB
#define ST77XX_RDID3 0xDC
#define ST77XX_RDID4 0xDD
#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH 0x04
#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR 0xB4
#define ST7735_DISSET5 0xB6
#define ST7735_PWCTR1 0xC0
#define ST7735_PWCTR2 0xC1
#define ST7735_PWCTR3 0xC2
#define ST7735_PWCTR4 0xC3
#define ST7735_PWCTR5 0xC4
#define ST7735_VMCTR1 0xC5
#define ST7735_PWCTR6 0xFC
#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

static void _Spi_WriteByte(ST7735* st7735, uint8_t byte) {
	st7735->f_write(&byte, 1);
}

static void _Spi_Command(ST7735* st, uint8_t cmd, uint8_t argc, uint8_t* args, uint8_t delay) {
	st->f_cs(true);
	st->f_cmd(true);
	_Spi_WriteByte(st, cmd);
	st->f_cmd(false);
	if (argc) {
		st->f_write(args, argc);
	}
	st->f_cs(false);
	if (delay) {
		st->f_delay(delay);
	}
}

void ST7735_Init(ST7735* st7735) {

	static const uint8_t initcmds[] = {
		ST77XX_SWRESET, DISPLAY_DELAY, 150,
		ST77XX_SLPOUT, DISPLAY_DELAY, 255,
		ST7735_FRMCTR1, 3, 0x01, 0x2C, 0x2D,
		ST7735_FRMCTR2, 3, 0x01, 0x2C, 0x2D,
		ST7735_FRMCTR3, 6, 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D,
		ST7735_INVCTR, 1, 0x07,
		ST7735_PWCTR1, 3, 0xA2, 0x02, 0x84,
		ST7735_PWCTR2, 1, 0xC5,
		ST7735_PWCTR3, 2, 0x0A, 0x00,
		ST7735_PWCTR4, 2, 0x8A, 0x2A,
		ST7735_PWCTR5, 2, 0x8A, 0xEE,
		ST7735_VMCTR1, 1, 0x0E,
		ST77XX_INVOFF, 0,
		ST77XX_COLMOD, 1, 0x05,
		ST7735_GMCTRP1, 16,
		0x02, 0x1c, 0x07, 0x12,
		0x37, 0x32, 0x29, 0x2d,
		0x29, 0x25, 0x2B, 0x39,
		0x00, 0x01, 0x03, 0x10,
		ST7735_GMCTRN1, 16,
		0x03, 0x1d, 0x07, 0x06,
		0x2E, 0x2C, 0x29, 0x2D,
		0x2E, 0x2E, 0x37, 0x3F,
		0x00, 0x00, 0x02, 0x10,
		ST77XX_NORON, DISPLAY_DELAY, 10,
		ST77XX_DISPON, DISPLAY_DELAY, 100,
	};

	uint32_t index = 0;
	while (index < sizeof (initcmds)) {
		uint8_t cmd = initcmds[index];
		uint8_t argc = initcmds[index + 1] & ~DISPLAY_DELAY;
		uint8_t delay = (initcmds[index + 1] & DISPLAY_DELAY) ? initcmds[index + 2 + argc] : 0;
		_Spi_Command(st7735, cmd, argc, (uint8_t*) initcmds + index + 2, delay);
		index += argc + 2 + (delay > 0);
	}

	ST7735_SetRotation(st7735, 1);
	ST7735_SetArea(st7735, 0, 0, st7735->m_Width - 1, st7735->m_Height - 1);

}

void ST7735_DrawPixel(ST7735* st7735, uint8_t x, uint8_t y, uint16_t color) {
	st7735->f_cs(true);
	st7735->f_cmd(true);
	_Spi_WriteByte(st7735, ST77XX_CASET);
	st7735->f_cmd(false);
	_Spi_WriteByte(st7735, 0);
	_Spi_WriteByte(st7735, x);
	st7735->f_cmd(true);
	_Spi_WriteByte(st7735, ST77XX_RASET);
	st7735->f_cmd(false);
	_Spi_WriteByte(st7735, 0);
	_Spi_WriteByte(st7735, y);
	st7735->f_cmd(true);
	_Spi_WriteByte(st7735, ST77XX_RAMWR);
	st7735->f_cmd(false);
	st7735->f_write((uint8_t*) & color, sizeof (color));
	st7735->f_cs(false);

}

void ST7735_DrawArea(ST7735* st7735, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t* data) {
	ST7735_SetArea(st7735, x,y, width, height);
	ST7735_StreamData(st7735, data, width*height);
}

void ST7735_FillArea(ST7735* st7735, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color) {
	ST7735_SetArea(st7735, x,y, width, height);
	st7735->f_cs(true);
	st7735->f_cmd(true);
	_Spi_WriteByte(st7735, ST77XX_RAMWR);
	st7735->f_cmd(false);
	for(uint16_t i = 0; i < width*height; ++i) {
		_Spi_WriteByte(st7735, (uint8_t) (color >> 8));
		_Spi_WriteByte(st7735, (uint8_t) color);
	}
	st7735->f_cs(false);
}

void ST7735_SetRotation(ST7735* st7735, uint8_t rotation) {
	uint8_t madctl = ST77XX_MADCTL_RGB;
	rotation %= 4;
	if (rotation == 0) {
		st7735->m_Height = ST7735_BASEHEIGHT;
		st7735->m_Width = ST7735_BASEWIDTH;
		madctl |= ST77XX_MADCTL_MX | ST77XX_MADCTL_MY;
	} else if (rotation == 1) {
		st7735->m_Width = ST7735_BASEHEIGHT;
		st7735->m_Height = ST7735_BASEWIDTH;
		madctl |= ST77XX_MADCTL_MY | ST77XX_MADCTL_MV;
	} else if (rotation == 2) {
		st7735->m_Height = ST7735_BASEHEIGHT;
		st7735->m_Width = ST7735_BASEWIDTH;
	} else if (rotation == 3) {
		st7735->m_Width = ST7735_BASEHEIGHT;
		st7735->m_Height = ST7735_BASEWIDTH;
		madctl |= ST77XX_MADCTL_MX | ST77XX_MADCTL_MV;
	}
	st7735->m_Rotation = rotation;
	_Spi_Command(st7735, ST77XX_MADCTL, 1, &madctl, 0);
}

void ST7735_SetArea(ST7735* st7735, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
	uint8_t data1[4] = {0, 0, 0, 0};
	uint8_t data2[4] = {0, 0, 0, 0};
	data1[1] = x;
	data1[3] = width+x-1;
	data2[1] = y;
	data2[3] = height+y-1;
	_Spi_Command(st7735, ST77XX_CASET, 4, data1, 0);
	_Spi_Command(st7735, ST77XX_RASET, 4, data2, 0);
}

void ST7735_StreamData(ST7735* st7735, uint16_t* data, uint32_t length) {
	st7735->f_cs(true);
	st7735->f_cmd(true);
	_Spi_WriteByte(st7735, ST77XX_RAMWR);
	st7735->f_cmd(false);
	while (length) {
		_Spi_WriteByte(st7735, (*data) >> 8);
		_Spi_WriteByte(st7735, (*data));
		length--;
	}
	st7735->f_cs(false);
}

#if ST7735_INCLUDE_FRAMEBUFFER == 1

void ST7735_Present(ST7735* st7735) {
	ST7735_SetArea(st7735, 0, 0, st7735->width, st7735->height);
	ST7735_StreamData(st7735, st7735->m_Framebuffer, sizeof(st7735->m_Framebuffer) / 2);
}

void ST7735_PresentArea(ST7735* st7735, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
	for (int i = y; y < height; ++y) {
		ST7735_SetArea(st7735, x, i, width, 1);
		ST7735_StreamData(st7735, &m_Framebuffer[i*st7735->m_Width+x], width);
	}
}

void ST7735_SetPixel(ST7735* st7735, uint8_t x, uint8_t y, uint16_t color) {
	st7735->m_Framebuffer[y * m_Width + x] = color;
}

#endif // ST7735_INCLUDE_FRAMEBUFFER == 1
