#include "flash.h"

uint8_t m_FlashSectorBuffer[FLASH_SECTOR_SIZE];

void FlashQueryId(uint8_t* rbuffer) {
	int8_t cmd[] = {FLASH_CMD_ID};
	SPIWriteRead(cmd, sizeof (cmd), rbuffer, 2);
}

void FlashFillSectorBuffer(uint32_t offset, uint8_t* data, uint32_t len) {
	uint32_t indexBuffer = offset;
	uint32_t bufferEnd = min(offset + len, FLASH_PAGE_SIZE);
	uint32_t indexData = 0;
	while (indexBuffer < bufferEnd) {
		m_FlashSectorBuffer[indexBuffer] = data[indexData];
		++indexBuffer;
		++indexData;
	}
}

void FlashCommitPageBuffer(uint32_t address, uint32_t wlen) {
	FlashErasePage(address);
	FlashWritePage(address, m_FlashSectorBuffer, wlen);
}

void FlashWritePage(uint32_t address, uint8_t* data, uint32_t wlen) {
	address &= FLASH_PAGE_MASK;
	while (wlen) {
		uint32_t toWrite = min(FLASH_PAGE_SIZE, wlen);
		uint8_t cmd[] = {FLASH_CMD_WRITE, address >> 16, address >> 8, address};
		FlashWaitForWriteEnable();
		SPIWriteCmd(cmd, sizeof (cmd), data, toWrite);
		address += 256;
		data += 256;
		wlen -= toWrite;
	}
}

void FlashOverwrite(uint32_t address, uint8_t* data, uint32_t length) {
	// TODO: erase affected sectors
	// write data to sectors
}

void FlashWrite(uint32_t address, uint8_t* wdata, length_t wlen) {

	uint32_t addrBegin = address & FLASH_PAGE_MASK;
	uint32_t addrEnd = (address + wlen + (FLASH_PAGE_SIZE - 1)) & FLASH_PAGE_MASK;
	uint32_t pages = (addrEnd - addrBegin) / FLASH_PAGE_SIZE;

	uint32_t currentAddr = addrBegin;
	uint32_t remainingBytes = wlen;
	uint32_t writtenBytes = 0;

	for (uint32_t p = 0; p < pages; ++p) {
		FlashRead(currentAddr, m_FlashSectorBuffer, FLASH_PAGE_SIZE);
		FlashWaitForWriteEnable();
		FlashErasePage(currentAddr);

		//modify page

		// first (or only) page (can be partial)
		if (p == 0) {
			uint32_t offset = address - addrBegin;
			uint32_t firstPageLength = min(wlen, FLASH_PAGE_SIZE - offset);
			for (uint32_t i = 0; i < firstPageLength; ++i) {
			m_FlashSectorBuffer[i + offset] = wdata[i];
		}
		remainingBytes -= firstPageLength;
		writtenBytes += firstPageLength;
		} else if (p + 1 == pages) { // last page (can be partial))
			for (uint32_t i = 0; i < remainingBytes; ++i) {
				m_FlashSectorBuffer[i] = wdata[writtenBytes + i];
			}
		} else { // full page in between
			for (uint32_t i = 0; i < FLASH_PAGE_SIZE; ++i) {
				m_FlashSectorBuffer[i] = wdata[writtenBytes + i];
			}
			remainingBytes -= FLASH_PAGE_SIZE;
			writtenBytes += FLASH_PAGE_SIZE;
		}

		FlashWaitForWriteEnable();
		FlashWritePage(currentAddr, m_FlashSectorBuffer, FLASH_PAGE_SIZE);
		currentAddr += FLASH_PAGE_SIZE;
	}
}

void FlashRead(uint32_t addr, uint8_t rdata, length_t rlen) {
	FlashWaitForReady();
	uint8_t cmd[] = {FLASH_CMD_READ, (addr >> 16), (addr >> 8), (addr >> 0)};
	SPIWriteRead(cmd, sizeof (cmd), rdata, rlen);
}

void FlashErasePage(uint32_t address) {
	FlashWaitForWriteEnable();
	uint8_t cmd[] = {FLASH_CMD_ERASE, address >> 16, address >> 8, address};
	SPIWrite(cmd, sizeof (cmd));
}

void FlashWaitForWriteEnable() {

	uint8_t cmdenable[] = {FLASH_CMD_WREN};
	uint8_t cmdread[] = {FLASH_CMD_RSTATUS};
	uint8_t status = 0;

	FlashWaitForReady();

	SPIWrite(cmdenable, sizeof (cmdenable));
	SPIWriteRead(cmdread, sizeof (cmdread), &status, 1);

	while (!(status & FLASH_FLAG_WREN)) {
		#ifdef DELAY_US
		DELAY_US(10);
		#endif
		SPIWrite(cmdenable, sizeof (cmdenable));
		#ifdef DELAY_US
		DELAY_US(5);
		#endif
		SPIWriteRead(cmdread, sizeof (cmdread), &status, 1);
	}

}

void FlashWaitForReady() {

	uint8_t cmd[] = {FLASH_CMD_RSTATUS};
	uint8_t status;

	SPIExchange(cmd, sizeof (cmd), &status, 1);

	while (status & FLASH_FLAG_BUSY) {
		#ifdef DELAY_US
		DELAY_US(10);
		#endif
		SPIWriteRead(cmd, sizeof (cmd), &status, 1);
	}

}
