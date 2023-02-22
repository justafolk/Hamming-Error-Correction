#ifndef FLASH_WRAPPER_H
#define FLASH_WRAPPER_H
#include "board.h"

#define FLASH_BASE_ADDR                     0x400000
#define FLASH_SIZE                          0x200000
#define FLASH_SECTOR_SIZE                   0x020000
#define FLASH_LOCK_REGION_SIZE              0x004000
#define FLASH_PAGE_SIZE                     0x000200
#define BACKUP_SECTION_ADDR                 (0x600000 - 0x2000)
/* Gives the number of bytes occupied by the specified number of pages */
#define FLASH_GET_PAGE_AREA(num_of_pages)   ((num_of_pages) << 9)
/* Gives the page number in which the given address lies */
#define FLASH_GET_PAGE_NUM(addr)            (((addr) - FLASH_BASE_ADDR) >> 9)
/**
 * Gives the upper boundary of the region of specified size in which address lies
 * This specified size must be a power of 2
 */
#define FLASH_GET_BOUND(addr, size)         ((addr) & ~((size) - 1))

void flash_initialize(void);

void flash_unlock(uint32_t startAddr, uint32_t endAddr);

void flash_lock(uint32_t startAddr, uint32_t endAddr);

uint32_t flash_is_locked(uint32_t startAddr, uint32_t endAddr);

void flash_write(uint32_t addr, void *buffer, uint32_t bufferLen);

void flash_erase(void);

void flash_erase_sector(uint32_t addr);

void flash_erase_pages(uint32_t addr, uint32_t numOfPages);

void flash_erase_single_page(uint32_t addr);

void flash_erase_range(uint32_t addr, uint32_t len);

void flash_set_security_bit(void);

void flash_clear_security_bit(void);

void flash_deinitialize(void);

#endif
