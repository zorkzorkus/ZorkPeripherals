// License: MIT
// Copyright 2022 (c) Zork 'zorkzorkus' Zorkus

#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include <stdbool.h>

typedef void (*Spi_Write_f)(uint8_t*, uint32_t);
typedef void (*Spi_Read_f)(uint8_t*, uint32_t);
typedef void (*Spi_Exchange_f)(uint8_t*, uint8_t*, uint32_t);
typedef void (*Spi_ChipSelect_f)(bool);
typedef void (*Spi_CommandSelect_f)(bool);
typedef void (*Spi_Reset_f)(bool);
typedef void (*Spi_DelayMs_f)(uint32_t);

// Example declarations:

void Spi_Init(void); // Not strictly needed, initializes the SPI hardware

void Spi_Write(uint8_t* wdata, uint32_t length); // Writes bytes to device, ignores receive
void Spi_Read(uint8_t* rdata, uint32_t length); // Writes dummy bytes, receives data
void Spi_Exchange(uint8_t* wdata, uint8_t* rdata, uint32_t length); // Writes and receives useful data
void Spi_ChipSelect(bool active); // Asserts chip select signal (usually active low), 'active = true' means the spi bus is active
void Spi_CommandSelect(bool active); // Asserts command (or register) select signal (usually active low), 'active = true' means the following spi transfer is a command
void Spi_Reset(bool active); // Asserts reset signal (does not reset SPI hardware)
void Spi_DelayMs(uint32_t ms); // Delay for given milliseconds

#endif
