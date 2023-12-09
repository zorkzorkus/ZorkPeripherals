// License: MIT
// Copyright 2023 (c) Zork 'zorkzorkus' Zorkus

#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t(*I2C_Write_f)(uint8_t addr, uint8_t* data, uint16_t len);
typedef uint8_t(*I2C_Read_f)(uint8_t addr, uint8_t* data, uint16_t len);
typedef uint8_t(*I2C_CmdWrite_f)(uint8_t addr, uint8_t cmd, uint8_t* data, uint16_t len);

typedef void(*I2C_Start_f)();
typedef bool(*I2C_ByteWrite_f)(uint8_t byte);
typedef uint8_t(*I2C_ByteRead_f)(bool ack);
typedef void(*I2C_Stop_f)();

void I2C_Init();

void I2C_Start();
bool I2C_ByteWrite(uint8_t data);
uint8_t I2C_ByteRead(bool ack);
void I2C_Stop();
uint8_t I2C_CmdWrite(uint8_t addr, uint8_t cmd, uint8_t* data, uint16_t len);
uint8_t I2C_Write(uint8_t addr, uint8_t* data, uint16_t len);
uint8_t I2C_Read(uint8_t addr, uint8_t* data, uint16_t len);

#endif
