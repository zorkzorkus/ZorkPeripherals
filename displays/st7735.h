// License: MIT
// Adapted from Adafruit Arduino Library
// https://github.com/adafruit/Adafruit-ST7735-Library
// Copyright 2022 (c) Zork 'zorkzorkus' Zorkus
//                (c) Adafruit Industries

#ifndef ST7735_H
#define ST7735_H

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "spi.h"

// --- Configure the library for the MCU 
#define ST7735_INCLUDE_FRAMEBUFFER 0
// ---

#define ST7735_BASEWIDTH 128
#define ST7735_BASEHEIGHT 160

typedef struct {
	Spi_Write_f f_write;
	Spi_ChipSelect_f f_cs;
	Spi_CommandSelect_f f_cmd;
	Spi_DelayMs_f f_delay;
	uint8_t m_Width;
	uint8_t m_Height;
	uint8_t m_Rotation;
	#if ST7735_INCLUDE_FRAMEBUFFER == 1
	static uint16_t m_Framebuffer[ST7735_BASEHEIGHT * ST7735_BASEWIDTH];
	#endif // ST7735_INCLUDE_FRAMEBUFFER == 1
} ST7735;

void ST7735_Init(ST7735* st7735);
void ST7735_DrawPixel(ST7735* st7735, uint8_t x, uint8_t y, uint16_t color);
void ST7735_DrawArea(ST7735* st7735, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t* data);
void ST7735_FillArea(ST7735* st7735, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t data);
void ST7735_SetRotation(ST7735* st7735, uint8_t rotation);
void ST7735_SetArea(ST7735* st7735, uint8_t x, uint8_t y, uint8_t width, uint8_t height);
void ST7735_StreamData(ST7735* st7735, uint16_t* data, uint32_t length);

#if ST7735_INCLUDE_FRAMEBUFFER == 1
void ST7735_Present(ST7735* st7735);
void ST7735_PresentArea(ST7735* st7735, uint8_t x, uint8_t y, uint8_t width, uint8_t height);
void ST7735_SetPixel(ST7735* st7735, uint8_t x, uint8_t y, uint16_t color);
#endif // ST7735_INCLUDE_FRAMEBUFFER == 1

#endif // DISPLAY_ST7735_H
