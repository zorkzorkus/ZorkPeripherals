// License: MIT
// Adapted from Adafruit Arduino Library
// https://github.com/adafruit/Adafruit_ILI9341
// Copyright 2022 (c) Zork 'zorkzorkus' Zorkus
//                (c) Adafruit Industries

#include "ili9341.h"

#include <stdio.h>

#define DISPLAY_DELAY 0x80

#define ILI9341_NOP 0x00
#define ILI9341_SWRESET 0x01
#define ILI9341_RDDID 0x04
#define ILI9341_RDDST 0x09
#define ILI9341_SLPIN 0x10
#define ILI9341_SLPOUT 0x11
#define ILI9341_PTLON 0x12
#define ILI9341_NORON 0x13
#define ILI9341_RDMODE 0x0A
#define ILI9341_RDMADCTL 0x0B
#define ILI9341_RDPIXFMT 0x0C
#define ILI9341_RDIMGFMT 0x0D
#define ILI9341_RDSELFDIAG 0x0F
#define ILI9341_INVOFF 0x20
#define ILI9341_INVON 0x21
#define ILI9341_GAMMASET 0x26
#define ILI9341_DISPOFF 0x28
#define ILI9341_DISPON 0x29
#define ILI9341_CASET 0x2A
#define ILI9341_RASET 0x2B
#define ILI9341_RAMWR 0x2C
#define ILI9341_RAMRD 0x2E
#define ILI9341_PTLAR 0x30
#define ILI9341_VSCRDEF 0x33
#define ILI9341_MADCTL 0x36
#define ILI9341_VSCRSADD 0x37
#define ILI9341_PIXFMT 0x3A
#define ILI9341_FRMCTR1 0xB1
#define ILI9341_FRMCTR2 0xB2
#define ILI9341_FRMCTR3 0xB3
#define ILI9341_INVCTR 0xB4
#define ILI9341_DFUNCTR 0xB6
#define ILI9341_PWCTR1 0xC0
#define ILI9341_PWCTR2 0xC1
#define ILI9341_PWCTR3 0xC2
#define ILI9341_PWCTR4 0xC3
#define ILI9341_PWCTR5 0xC4
#define ILI9341_VMCTR1 0xC5
#define ILI9341_VMCTR2 0xC7
#define ILI9341_PWCTRA 0xCB
#define ILI9341_PWCTRB 0xCF
#define ILI9341_RDID1 0xDA
#define ILI9341_RDID2 0xDB
#define ILI9341_RDID3 0xDC
#define ILI9341_RDID4 0xDD
#define ILI9341_GMCTRP1 0xE0
#define ILI9341_GMCTRN1 0xE1
#define ILI9341_DTCTRA 0xE8
#define ILI9341_DTCTRB 0xEA	
#define ILI9341_POSCTR 0xED
#define ILI9341_ENA3G 0xF2
#define ILI9341_PUMPRCTR 0xF7

#define MADCTL_MY 0x80
#define MADCTL_MX 0x40
#define MADCTL_MV 0x20
#define MADCTL_ML 0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH 0x04


static void _Spi_WriteByte(ILI9341* ili9341, uint8_t byte) {
	ili9341->f_write(&byte, 1);
}

static void _Spi_Command(ILI9341* st, uint8_t cmd, uint8_t argc, uint8_t* args, uint8_t delay) {
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

void ILI9341_Init(ILI9341* ili9341) {

	static const uint8_t initCmds[] = {
		//0xEF, 3, 0x03, 0x80, 0x02, // couldn't find what this command is in the datasheet
		ILI9341_PWCTRB  , 3, 0x00, 0xC1, 0x30,
		ILI9341_POSCTR  , 4, 0x64, 0x03, 0x12, 0x81,
		ILI9341_DTCTRA  , 3, 0x85, 0x00, 0x78,
		ILI9341_PWCTRA  , 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
		ILI9341_PUMPRCTR, 1, 0x20,
		ILI9341_DTCTRB  , 2, 0x00, 0x00,
		ILI9341_PWCTR1  , 1, 0x23,
		ILI9341_PWCTR2  , 1, 0x10,
		ILI9341_VMCTR1  , 2, 0x3e, 0x28,
		ILI9341_VMCTR2  , 1, 0x86,
		ILI9341_MADCTL  , 1, 0x48,
		ILI9341_VSCRSADD, 1, 0x00,
		ILI9341_PIXFMT  , 1, 0x55,
		ILI9341_FRMCTR1 , 2, 0x00, 0x18,
		ILI9341_DFUNCTR , 3, 0x08, 0x82, 0x27,
		ILI9341_ENA3G   , 1, 0x00,
		ILI9341_GAMMASET, 1, 0x01,
		ILI9341_GMCTRP1 , 15,
			0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1,
			0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
		ILI9341_GMCTRN1 , 15,
			0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1,
			0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
		ILI9341_SLPOUT  , 0x80,                // Exit Sleep
		ILI9341_DISPON  , 0x80,                // Display on
	};

	uint32_t index = 0;
	while (index < sizeof (initCmds)) {
		uint8_t cmd = initCmds[index];
		uint8_t argc = initCmds[index + 1] & ~DISPLAY_DELAY;
		uint8_t delay = (initCmds[index + 1] & DISPLAY_DELAY) ? 100 : 0;
		_Spi_Command(ili9341, cmd, argc, (uint8_t*) initCmds + index + 2, delay);
		index += argc + 2;
	}

	ILI9341_SetRotation(ili9341, 0);
	ILI9341_SetArea(ili9341, 0, 0, ili9341->m_Width - 1, ili9341->m_Height - 1);

}

void ILI9341_DrawPixel(ILI9341* ili9341, uint16_t x, uint16_t y, uint16_t color) {
	ili9341->f_cs(true);
	ili9341->f_cmd(true);
	_Spi_WriteByte(ili9341, ILI9341_CASET);
	ili9341->f_cmd(false);
	_Spi_WriteByte(ili9341, x >> 8);
	_Spi_WriteByte(ili9341, x);
	ili9341->f_cmd(true);
	_Spi_WriteByte(ili9341, ILI9341_RASET);
	ili9341->f_cmd(false);
	_Spi_WriteByte(ili9341, y >> 8);
	_Spi_WriteByte(ili9341, y);
	ili9341->f_cmd(true);
	_Spi_WriteByte(ili9341, ILI9341_RAMWR);
	ili9341->f_cmd(false);
	ili9341->f_write((uint8_t*) & color, sizeof (color));
	ili9341->f_cs(false);

}

void ILI9341_DrawArea(ILI9341* ili9341, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t* data) {
	ILI9341_SetArea(ili9341, x, y, width, height);
	ILI9341_StreamData(ili9341, data, (uint32_t)width*height);
}

void ILI9341_FillArea(ILI9341* ili9341, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
	ILI9341_SetArea(ili9341, x, y, width, height);
	ili9341->f_cs(true);
	ili9341->f_cmd(true);
	_Spi_WriteByte(ili9341, ILI9341_RAMWR);
	ili9341->f_cmd(false);
	for (uint32_t i = 0; i < (uint32_t)width * height; ++i) {
		_Spi_WriteByte(ili9341, (uint8_t) (color >> 8));
		_Spi_WriteByte(ili9341, (uint8_t) color);
	}
	ili9341->f_cs(false);
}

void ILI9341_SetRotation(ILI9341* ili9341, uint8_t rotation) {
	uint8_t madctl = MADCTL_BGR;
	rotation %= 4;
	if (rotation == 0) {
		ili9341->m_Height = ILI9341_BASEHEIGHT;
		ili9341->m_Width = ILI9341_BASEWIDTH;
		madctl |= MADCTL_MX;
	} else if (rotation == 1) {
		ili9341->m_Width = ILI9341_BASEHEIGHT;
		ili9341->m_Height = ILI9341_BASEWIDTH;
		madctl |= MADCTL_MV;
	} else if (rotation == 2) {
		ili9341->m_Height = ILI9341_BASEHEIGHT;
		ili9341->m_Width = ILI9341_BASEWIDTH;
		madctl |= MADCTL_MY;
	} else if (rotation == 3) {
		ili9341->m_Width = ILI9341_BASEHEIGHT;
		ILI9341->m_Height = ILI9341_BASEWIDTH;
		madctl |= MADCTL_MX | MADCTL_MY | MADCTL_MV;
	}
	ili9341->m_Rotation = rotation;
	_Spi_Command(ili9341, ILI9341_MADCTL, 1, &madctl, 0);
}

void ILI9341_SetArea(ILI9341* ili9341, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	uint8_t data1[4];
	uint8_t data2[4];
	data1[0] = x >> 8;
	data1[1] = x;
	data1[2] = (width+x-1) >> 8;
	data1[3] = width+x-1;
	data2[0] = y >> 8;
	data2[1] = y;
	data2[2] = (height+y-1) >> 8;
	data2[3] = height+y-1;
	_Spi_Command(ili9341, ILI9341_CASET, 4, data1, 0);
	_Spi_Command(ili9341, ILI9341_RASET, 4, data2, 0);
}

void ILI9341_StreamData(ILI9341* ili9341, uint16_t* data, uint32_t length) {
	ili9341->f_cs(true);
	ili9341->f_cmd(true);
	_Spi_WriteByte(ili9341, ILI9341_RAMWR);
	ili9341->f_cmd(false);
	while (length) {
		_Spi_WriteByte(ili9341, (*data) >> 8);
		_Spi_WriteByte(ili9341, (*data));
		length--;
	}
	ili9341->f_cs(false);
}

#if ILI9341_INCLUDE_FRAMEBUFFER == 1

void ILI9341_Present(ILI9341* ili9341) {
	ILI9341_SetArea(ili9341, 0, 0, ILI9341->width, ILI9341->height);
	ILI9341_StreamData(ili9341, ILI9341->m_Framebuffer, sizeof(ili9341->m_Framebuffer) / 2);
}

void ILI9341_PresentArea(ILI9341* ili9341, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	for (int i = y; y < height; ++y) {
		ILI9341_SetArea(ili9341, x, i, width, 1);
		ILI9341_StreamData(ili9341, &m_Framebuffer[i*ili9341->m_Width+x], width);
	}
}

void ILI9341_SetPixel(ILI9341* ili9341, uint16_t x, uint16_t y, uint16_t color) {
	ili9341->m_Framebuffer[(uint32_t)y * m_Width + x] = color;
}

#endif // ILI9341_INCLUDE_FRAMEBUFFER == 1
