#include "chip.h"
#include "board.h"
#include "stdlib.h"
#include "assert.h"
#include "flash_wrapper.h"

/**
 * \brief This function initializes the flash. It updates the regions in the
 *        flash to full access. Sets 6 cycles for further initialization of the
 *        EEFC.
 * \param void
 * \retval void
 */
void flash_initialize() {
    /* Update internal flash Region to Full Access */
    MPU_UpdateRegions(MPU_DEFAULT_IFLASH_REGION, IFLASH_START_ADDRESS, \
                      MPU_AP_FULL_ACCESS |
                      INNER_NORMAL_WB_NWA_TYPE( NON_SHAREABLE ) |
                      MPU_CalMPURegionSize(IFLASH_END_ADDRESS - IFLASH_START_ADDRESS) |
                      MPU_REGION_ENABLE);

    /* Set 6 WS for internal Flash writing (refer to errata) */
    EFC_SetWaitState(EFC, 6);

    /* Initialize flash driver */
    FLASHD_Initialize(BOARD_MCK, 0);
}

/**
 * \brief This function unlocks the regions specified between the given
 *        address range. It also asserts if the unlocking is successful.
 * \param[in] start_addr Start address of region to be unlocked
 * \param[in] end_addr End address of region to be unlocked
 * \retval void
 * \note Region unlocked is from start address to end address - 1
 */
void flash_unlock(uint32_t start_addr, uint32_t end_addr) {

    /* Find the actual start address for the lock region */
    uint32_t actual_start_addr = FLASH_GET_BOUND(start_addr, FLASH_LOCK_REGION_SIZE);

    /* Find the actual end address for the lock region */
    uint32_t actual_end_addr = FLASH_GET_BOUND(end_addr, FLASH_LOCK_REGION_SIZE);

    /* Check if the actual end address is same as the end address   */
    if (actual_end_addr != end_addr) {
        /* If not then get the lower boundary of the lock region */
        actual_end_addr += FLASH_LOCK_REGION_SIZE;
    }

    uint32_t error = FLASHD_Unlock(actual_start_addr, actual_end_addr, NULL, NULL);
    assert(!error);
}

/**
 * \brief This function locks the regions specified between the given
 *        address range. It also asserts if the locking is successful.
 * \param[in] start_addr Start address of the region to be locked
 * \param[in] end_addr End address of the region to be locked
 * \retval void
 * \note Region locked is from start address to end address - 1
 */
void flash_lock(uint32_t start_addr, uint32_t end_addr) {

    /* Find the actual start address for the lock region */
    uint32_t actual_start_addr = FLASH_GET_BOUND(start_addr, FLASH_LOCK_REGION_SIZE);

    /* Find the actual end address for the lock region */
    uint32_t actual_end_addr = FLASH_GET_BOUND(end_addr, FLASH_LOCK_REGION_SIZE);

    /* Check if the actual end addr is same as the end addr   */
    if (actual_end_addr != end_addr) {
        /* If not then get the lower boundary of the lock region */
        actual_end_addr += FLASH_LOCK_REGION_SIZE;
    }

    uint32_t error = FLASHD_Lock(actual_start_addr, actual_end_addr, NULL, NULL);
    assert(!error);
}

/**
 * \brief This function checks if the regions lying between the given address
 *        range are locked.
 * \param[in] start_addr Start address of the region to be checked
 * \param[in] end_addr End address of the region to be checked
 * \retval int The number of regions locked between the given address range
 */
uint32_t flash_is_locked(uint32_t start_addr, uint32_t end_addr) {

    /* Find the actual start address for the lock region */
    uint32_t actual_start_addr = FLASH_GET_BOUND(start_addr, FLASH_LOCK_REGION_SIZE);

    /* Find the actual end address for the lock region */
    uint32_t actual_end_addr = FLASH_GET_BOUND(end_addr, FLASH_LOCK_REGION_SIZE);

    /* Check if the actual end addr is same as the end addr   */
    if (actual_end_addr != end_addr) {
        /* If not then get the lower boundary of the lock region */
        actual_end_addr += FLASH_LOCK_REGION_SIZE;
    }

    return FLASHD_IsLocked(actual_start_addr, actual_end_addr);
}

/**
 * \brief This function writes the specified number of bytes from the given
 *        buffer starting from the address passed as an argument. Write
 *        operation must be performed only if the region to be written is
 *        unlocked, hence this function unlocks the region if it is locked,
 *        writes the specified number of bytes and then locks the region
 *        again if it was previously locked.
 * \param[in] addr Start address in Flash from where to write
 * \param[in] buffer Buffer consisting of the data
 * \param[in] buffer_len Number of bytes to be written from the buffer
 * \retval void
 */
void flash_write(uint32_t addr, void *buffer, uint32_t buffer_len) {
    uint32_t is_locked, error;

    /* Check if the number of bytes to be written is mulitple of 4 bytes */
    assert(!(buffer_len & 3));

    /* Get the status of the region to be written */
    is_locked = flash_is_locked(addr, addr + buffer_len);

    /* If the requested region is locked then unlock */
    if (is_locked) {
        flash_unlock(addr, addr + buffer_len);
    }

    /* Write the buffer to the specified address */
    error = FLASHD_Write(addr, buffer, buffer_len);

    /* If the requested region was locked then lock again */
    if (is_locked) {
        flash_lock(addr, addr + buffer_len);
    }

    assert(!error);
}

/**
 *  \brief This function erases the entire Flash. It unlocks the entire Flash
 *         if locked, erases, locks it again if previously locked and then
 *         asserts the erasure.
 * \param void
 * \retval void
 */
void flash_erase() {
    uint32_t is_locked, error, start_addr, end_addr;

    /* Get the start and end address of the Flash*/
    start_addr = FLASH_BASE_ADDR;
    end_addr = FLASH_BASE_ADDR + FLASH_SIZE;

    /* Get the status of the region to be erased */
    is_locked = flash_is_locked(start_addr, end_addr);

    /* Unlock the region if locked */
    if (is_locked) {
        flash_unlock(start_addr, end_addr);
    }

    /* Erase the Flash */
    error = FLASHD_Erase(start_addr);

    /* Lock the region if previously locked */
    if (is_locked) {
        flash_lock(start_addr, end_addr);
    }

    assert(!error);
}

/**
 * \brief This function erases the sector in which the given address lies. It
 *        unlocks the sector in which the address lies, erases the entire
 *        sector, locks it again if previously locked and asserts the erasure.
 * \param[in] addr Address in Flash
 * \retval void
 */
void flash_erase_sector(uint32_t addr) {
    uint32_t is_locked, error, start_addr, end_addr;

    /* Get the starting and the end address of the sector to be erased */
    start_addr = FLASH_GET_BOUND(addr, FLASH_SECTOR_SIZE);
    end_addr = start_addr + FLASH_SECTOR_SIZE;

    /* Get the status of the region to be erased */
    is_locked = flash_is_locked(start_addr, end_addr);

    /* Unlock the region if locked */
    if (is_locked) {
        flash_unlock(start_addr, end_addr);
    }

    /* Erase the sector */
    error = FLASHD_EraseSector(addr);

    /* Lock the region if previously locked */
    if (is_locked) {
        flash_lock(start_addr, end_addr);
    }

    assert(!error);
}

/**
 * \brief This function erases the specified number of pages where the given
 *        address lies in the Flash, it unlocks the pages if previously
 *        locked, erases them and locks them again if previously locked. It
 *        also asserts the erasure. The number of pages that can be erased
 *        are 32, 16, 8 or 4, if any other number if provided then 4 pages
 *        are erased by default, which may cause error for addresses after
 *        the first 16 KB section.
 * \param[in] addr Address in Flash
 * \param[in] numOfpages Number of pages to be erased
 * \retval void
 */
void flash_erase_pages(uint32_t addr, uint32_t num_of_pages) {
    uint32_t is_locked, error, start_addr, end_addr;

    /* Set the number of pages to be erased */
    if ((num_of_pages != 8) && (num_of_pages != 16) && (num_of_pages != 32)) {
        num_of_pages = 4;
    }

    /* Get the starting address of the section */
    start_addr = FLASH_GET_BOUND(addr, FLASH_GET_PAGE_AREA(num_of_pages));

    /* Get the ending address of the section in which the given address lies */
    end_addr = start_addr + FLASH_GET_PAGE_AREA(num_of_pages);;

    /* Get the status of the region to be erased */
    is_locked = flash_is_locked(start_addr, end_addr);

    /* Unlock the pages if locked */
    if (is_locked) {
        flash_unlock(start_addr, end_addr);
    }

    /* Erase the pages */
    error = FLASHD_ErasePages(addr, num_of_pages);

    /* Lock the pages if locked previously */
    if (is_locked) {
        flash_lock(start_addr, end_addr);
    }

    assert(!error);
}

/**
 * \brief This function erases a single page in which the given address lies.
 *        It copies the section (either 8 KB or 2 KB) to the backup section
 *        (8 KB), erases the entire section (8 KB or 2 KB) and then copies
 *        all but one page back to the original address, hence resulting in
 *        a single page erasure.
 * \param[in] addr Address in Flash
 * \retval void
 * \todo To implement this function in a better and faster way if possible
 */
void flash_erase_single_page(uint32_t addr) {
    uint32_t start_addr, page_num, backup_addr, num_of_pages;

    /* If addr resides in sectors other than the first two small sectors */
    if (addr >= 0x404000) {
        num_of_pages = 16;
    }
    /* If addr resides in the first two small sectors */
    else {
        num_of_pages = 4;
    }

    /* Get the starting address of the section (8 KB or 2 KB)  */
    start_addr = FLASH_GET_BOUND(addr, FLASH_GET_PAGE_AREA(num_of_pages));

    /** Get the page number in which the given address lies relative to
     *  the section in which the address lies
     */
    page_num = FLASH_GET_PAGE_NUM(addr) - FLASH_GET_PAGE_NUM(start_addr);

    /* Erase the back up section */
    backup_addr = (uint32_t)BACKUP_SECTION_ADDR;
    flash_erase_pages(backup_addr, 16);

    /* Copy the pages before the page to be erased to backup section */
    flash_write(
            backup_addr,
            (void *)start_addr,
            FLASH_GET_PAGE_AREA(page_num));

    /* Copy the pages after the page to be erased to backup section */
    flash_write(
            backup_addr + FLASH_GET_PAGE_AREA((page_num + 1)),
            (void *)(start_addr + FLASH_GET_PAGE_AREA((page_num + 1))),
            FLASH_GET_PAGE_AREA(num_of_pages - page_num - 1));

    /* Erase the section (4 or 16 pages) from the Flash */
    flash_erase_pages(addr, num_of_pages);

    /* Copy the pages before the page to be erased to backup section */
    flash_write(
            start_addr,
            (void *)backup_addr,
            FLASH_GET_PAGE_AREA(page_num));

    /* Copy the pages after the page to be erased to backup section */
    flash_write(
            start_addr + FLASH_GET_PAGE_AREA((page_num + 1)),
            (void *)(backup_addr + FLASH_GET_PAGE_AREA((page_num + 1))),
            FLASH_GET_PAGE_AREA(num_of_pages - page_num - 1));

}

/**
 * \brief This function erases the addresses between the specified address
 *        range and the addresses must be multiple of 4 bytes
 * \param[in] start_addr Start address of the region to be erased
 * \param[in] end_addr End address of the region to be erased.
 * \retval void
 * \note The region erased is from start_addr to end_addr - 1
 * \warning Never use this function in final implementation, it is just for
 *          testing purpose
 */
void flash_erase_range(uint32_t start_addr, uint32_t end_addr) {
    uint32_t start_sect_num, end_sect_num;
    uint32_t page_num, sect_start, backup_addr;
    uint32_t i;

    /** Check if the addresses specified are multiples of four
     *  if not then change them
     */
    if ((start_addr & 3) != 0)
        start_addr = start_addr & ~(3);
    if ((end_addr & 3) != 0)
        end_addr = (end_addr & ~(3)) + 4;

    /* Get the 8KB section numbers in which the given addresses lie */
    start_sect_num = start_addr >> 13; /* Divide by 8192 */
    end_sect_num = end_addr >> 13;	   /* Divide by 8192 */

    /* Get the backup section address */
    backup_addr = (uint32_t)BACKUP_SECTION_ADDR;

    /* If the start section number and the end section number are not same */
    if (start_sect_num != end_sect_num) {
        /* Get the section boundary for the first section */
        sect_start = FLASH_GET_BOUND(start_addr, FLASH_GET_PAGE_AREA(16));

        /* Get the page num of the start address within the section */
        page_num = FLASH_GET_PAGE_NUM(start_addr) - FLASH_GET_PAGE_NUM(sect_start);

        /* Erase the back up section */
        flash_erase_pages(backup_addr, 16);

        /* Copy the first section in the backup section */
        for (i = 0; i <= page_num; i++) {
            flash_write(
                    backup_addr + FLASH_GET_PAGE_AREA(i),
                    (void *)(sect_start + FLASH_GET_PAGE_AREA(i)),
                    FLASH_PAGE_SIZE);
        }

        /* Erase the first section */
        flash_erase_pages(start_addr, 16);

        /** Write all the pages from the start of the section before the page
         *  in which the start erase address lies
         */
        for (i = 0; i < page_num; i++) {
            flash_write(
                    sect_start + FLASH_GET_PAGE_AREA(i),
                    (void *)(backup_addr + FLASH_GET_PAGE_AREA(i)),
                    FLASH_PAGE_SIZE);
        }

        /** Write data from the page in which the address lies only
         *  upto the start address
         */
        flash_write(
                sect_start + FLASH_GET_PAGE_AREA(page_num),
                (void *)(backup_addr + FLASH_GET_PAGE_AREA(page_num)),
                start_addr & 511);

        /* Erase the middle sections completely if any */
        if ((end_sect_num - start_sect_num) > 1) {
            sect_start = sect_start + FLASH_GET_PAGE_AREA(16);
            for (i = start_sect_num + 1; i < end_sect_num; i++) {
                flash_erase_pages(sect_start, 16);
                sect_start = sect_start + FLASH_GET_PAGE_AREA(16);
            }
        }

        /* Get the section boundary for the last section */
        sect_start = FLASH_GET_BOUND(end_addr, FLASH_GET_PAGE_AREA(16));
        page_num = FLASH_GET_PAGE_NUM(end_addr) - FLASH_GET_PAGE_NUM(sect_start);

        /* Erase the back up section */
        flash_erase_pages(backup_addr, 16);

        /* Copy the end section in the backup section */
        for (i = page_num; i < 16; i++) {
            flash_write(
                    backup_addr + FLASH_GET_PAGE_AREA(i),
                    (void *)(sect_start + FLASH_GET_PAGE_AREA(i)),
                    FLASH_PAGE_SIZE);
        }

        /* Erase the last section */
        flash_erase_pages(end_addr, 16);

        /* Write data from the end erase address to the end of the section */
        flash_write(
                end_addr,
                (void *)(backup_addr + FLASH_GET_PAGE_AREA(page_num) + (end_addr & 511)),
                FLASH_PAGE_SIZE - (end_addr & 511));

        /* Write rest of the pages */
        for (i = page_num + 1; i < 16; i++) {
            flash_write(
                    sect_start + FLASH_GET_PAGE_AREA(i),
                    (void *)(backup_addr + FLASH_GET_PAGE_AREA(i)),
                    FLASH_PAGE_SIZE);
        }
    }
    /* If the start section number and the end section umber are same */
    else {
        sect_start = FLASH_GET_BOUND(start_addr, FLASH_GET_PAGE_AREA(16));

        /* Erase the back up section */
        flash_erase_pages(backup_addr, 16);

        /* Copy the section in the backup section */
        for (i = 0; i < 16; i++) {
            flash_write(
                    backup_addr + FLASH_GET_PAGE_AREA(i),
                    (void *)(sect_start + FLASH_GET_PAGE_AREA(i)),
                    FLASH_PAGE_SIZE);
        }

        /* Erase the required section */
        flash_erase_pages(start_addr, 16);

        /* Get the page number for the start address */
        page_num = FLASH_GET_PAGE_NUM(start_addr) - FLASH_GET_PAGE_NUM(sect_start);

        /** Write the data back from the back up section to the required
         *  section except from start to end of the erase range
         */
        for (i = 0; i < page_num; i++) {
            flash_write(
                    sect_start + FLASH_GET_PAGE_AREA(i),
                    (void *)(backup_addr + FLASH_GET_PAGE_AREA(i)),
                    FLASH_PAGE_SIZE);
        }

        /** Write data from the page in which the address lies only upto the
         *  start address
         */
        flash_write(
                sect_start + FLASH_GET_PAGE_AREA(page_num),
                (void *)(backup_addr + FLASH_GET_PAGE_AREA(page_num)),
                start_addr & 511);

        /* Get the end address page number */
        page_num = FLASH_GET_PAGE_NUM(end_addr) - FLASH_GET_PAGE_NUM(sect_start);

        flash_write(
                end_addr,
                (void *)(backup_addr + FLASH_GET_PAGE_AREA(page_num) + (end_addr & 511)),
                FLASH_PAGE_SIZE - (end_addr & 511));

        /* Write rest of the pages */
        for (i = page_num + 1; i < 16; i++) {
            flash_write(
                    sect_start + FLASH_GET_PAGE_AREA(i),
                    (void *)(backup_addr + FLASH_GET_PAGE_AREA(i)),
                    FLASH_PAGE_SIZE);
        }
    }
}

/**
 * \brief This function sets the GPNVM 0th bit and also asserts the setting.
 * \param void
 * \retval void
 */
void flash_set_security_bit() {
    uint32_t error = FLASHD_SetGPNVM((uint8_t)0);
    assert(!error);
}

/**
 * \brief This function resets the GPNVM 0th bit and also asserts the
 *        resetting.
 * \param void
 * \retval void
 */
void flash_clear_security_bit() {
    uint32_t error = FLASHD_ClearGPNVM((uint8_t)0);
    assert(!error);
}

/**
 * \brief This function resets the Flash back to the original access type,
 *        which is the read only access type.
 * \param void
 * \retval void
 */
void flash_deinitialize() {
    /* Update internal flash Region to previous configurations */
    MPU_UpdateRegions(MPU_DEFAULT_IFLASH_REGION, IFLASH_START_ADDRESS, \
                      MPU_AP_READONLY |
                      INNER_NORMAL_WB_NWA_TYPE( NON_SHAREABLE ) |
                      MPU_CalMPURegionSize(IFLASH_END_ADDRESS - IFLASH_START_ADDRESS) |
                      MPU_REGION_ENABLE);
}
