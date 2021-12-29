# ZorkPeripherals

A library for all kind of different microcontroller peripheral devices (such as SPI-Flash, I2C-Displays) using common interfaces.

## Supported Devices

* SPI-Display 
	* ST7735 160 x 128
	* ILI9341 320 x 240

## How to Use

* Include the peripheral files you want to use. (ex. ST7735 Display)
* Implement all functions of the interface (ex. SPI, I2C) that the peripherals require
* Create a struct of the peripheral and supply the functions to it
* Call the initialize function, then you can use the library functions

---

## Example: ST7735 SPI Display

The display with the ST7735 uses 4 signals:
 * SPI Clock
 * SPI Data (MOSI)
 * SPI Chip Select
 * Register Select / Data Command

As such the following functions have to be implemented:

| Function | Description |
| --- | --- |
| ```void Spi_Write(uint8_t* data, uint32_t length)``` | Writes ```length``` bytes pointed to by ```data``` on the SPI bus (MSB first), does not control the *Chip Select* Signal |
| ```void Spi_ChipSelect(bool assert)``` | (De-)Asserts the *Chip Select* Signal. ```true``` usually means setting the signal to low and vice versa |
| ```void Spi_CommandSelect(bool assert)``` | (De-)Asserts the *Command Select* Signal. ```true``` means the SPI transfer is a command, which usually means the signal is set to low and vice versa |
| ```void Spi_DelayMs(uint32_t ms)``` | Delays the execution of code for approximately **ms** milliseconds |

Then create the device struct, supply the functions and call the initialize function.

```
ST7735 st;

st.f_write = Spi_Write;
st.f_cs = Spi_ChipSelect;
st.f_cmd = Spi_CommandSelect;
st.f_delay = Spi_DelayMs;

ST7735_Init(&st);

// Fill the entire screen in color black
ST7735_FillArea(&st, 0, 0, st->m_Width, st->m_Height, 0);
```
