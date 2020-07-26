#ifndef SPI_H
#define SPI_H

#include <stdint.h>

// #typedef <your_type> length_t

void SPIInit();
void SPIWrite(uint8_t* wdata, length_t wlen);
void SPIWriteCmd(uint8_t* cdata, length_t clen, uint8_t* wdata, length_t wlen);
void SPIWriteRead(uint8_t* wdata, length_t wlen, uint8_t* rdata, length_t rlen);
void SPIWriteReadCmd(uint8_t* cdata, length_t clen, uint8_t* wdata, length_t wlen, uint8_t* rdata, length_t rlen);
void SPIRead(uint8_t* rdata, length_t rlen);

#endif // SPI_H