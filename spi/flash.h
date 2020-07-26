#ifndef FLASH_H
#define FLASH_H

#include "spi.h"

// Library valid for:
// Winbond W25Q64FV
// Adesto AT25QF128A (not tested)

#define FLASH_CMD_WREN 0x06
#define FLASH_CMD_READ 0x03
#define FLASH_CMD_ERASE 0x20
#define FLASH_CMD_WRITE 0x02
#define FLASH_CMD_RSTATUS 0x05
#define FLASH_CMD_ID 0x9F

#define FLASH_FLAG_WREN 0x02
#define FLASH_FLAG_BUSY 0x01

#define FLASH_PAGE_SIZE 256
#define FLASH_PAGE_MASK 0xfffff000
#define FLASH_SECTOR_SIZE 4096

void FlashQueryId(uint8_t* rbuffer);
void FlashFillSectorBuffer(uint32_t offset, uint8_t* data, uint32_t len);
void FlashCommitPageBuffer(uint32_t address, uint32_t wlen);
void FlashWritePage(uint32_t address, uint8_t* wdata, uint32_t wlen);
void FlashOverwrite(uint32_t address, uint8_t* wdata, uint32_t wlen);
void FlashWrite(uint32_t address, uint8_t wdata, length_t wlen);
void FlashRead(uint32_t address, uint8_t rdata, length_t rlen);
void FlashErasePage(uint32_t address);
void FlashWaitForWriteEnable();
void FlashWaitForReady();

#endif // FLASH_H