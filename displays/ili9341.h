// License: MIT
// Adapted from Adafruit Arduino Library
// https://github.com/adafruit/Adafruit_ILI9341
// Copyright 2023 (c) Zork 'zorkzorkus' Zorkus
//                (c) Adafruit Industries

#ifndef ILI9341_H
#define ILI9341_H

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "spi.h"

// --- Configure the library for the MCU
#define ILI9341_INCLUDE_FRAMEBUFFER 0
// ---

#define ILI9341_BASEWIDTH 240
#define ILI9341_BASEHEIGHT 320

typedef struct {
	Spi_Write_f f_write;
	Spi_ChipSelect_f f_cs;
	Spi_CommandSelect_f f_cmd;
	Spi_DelayMs_f f_delay;
	Spi_Reset_f f_reset;
	uint16_t m_Width;
	uint16_t m_Height;
	uint8_t m_Rotation;
	#if ILI9341_INCLUDE_FRAMEBUFFER == 1
	static uint16_t m_Framebuffer[ILI9341_BASEHEIGHT * ILI9341_BASEWIDTH];
	#endif // ILI9341_INCLUDE_FRAMEBUFFER == 1
} ILI9341;

void ILI9341_Init(ILI9341* ILI9341);
void ILI9341_DrawPixel(ILI9341* ILI9341, uint16_t x, uint16_t y, uint16_t color);
void ILI9341_DrawArea(ILI9341* ILI9341, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t* data);
void ILI9341_FillArea(ILI9341* ILI9341, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t data);
void ILI9341_SetRotation(ILI9341* ILI9341, uint8_t rotation);
void ILI9341_SetArea(ILI9341* ILI9341, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void ILI9341_StreamData(ILI9341* ILI9341, uint16_t* data, uint32_t length);

#if ILI9341_INCLUDE_FRAMEBUFFER == 1
void ILI9341_Present(ILI9341* ILI9341);
void ILI9341_PresentArea(ILI9341* ILI9341, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void ILI9341_SetPixel(ILI9341* ILI9341, uint16_t x, uint16_t y, uint16_t color);
#endif // ILI9341_INCLUDE_FRAMEBUFFER == 1

#endif // DISPLAY_ILI9341_H
