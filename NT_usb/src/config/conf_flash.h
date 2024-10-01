/*
 * conf_flash.h
 *
 * Created: 12/18/2022 11:06:54 AM
 *  Author: DvirShavit
 */ 


#ifndef CONF_FLASH_H_
#define CONF_FLASH_H_

#include "compiler.h"
#include "efc.h"

/* EFC access mode */
#define FLASH_ACCESS_MODE EFC_ACCESS_MODE_128

/* Test page start address. */
#define PAGE_ADDRESS (IFLASH_ADDR + IFLASH_SIZE - IFLASH_PAGE_SIZE * 4)

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
 extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

static int buffer_length = IFLASH_PAGE_SIZE / sizeof(uint32_t);
static int index_buffer = 0;

static uint32_t page_buffer[IFLASH_PAGE_SIZE / sizeof(uint32_t)] = {0};
	
///*! \name Flash driver return codes */
////! @{
//typedef enum flash_rc {
	//FLASH_RC_OK = 0,        //!< Operation OK
	//FLASH_RC_YES = 1,       //!< Yes
	//FLASH_RC_NO = 0,        //!< No
	//FLASH_RC_ERROR = 0x10,  //!< General error
	//FLASH_RC_INVALID,       //!< Invalid argument input
	//FLASH_RC_NOT_SUPPORT = 0xFFFFFFFF    //!< Operation is not supported
//} flash_rc_t;
////! @}
//
///*! \name Flash erase page num in FARG[1:0] 
  //\note The erase page commands should be cautiouly used as EPA4/EPA32 will not
  //take effect according to errata and EPA8/EPA16 must module 8/16 page addresses.*/
////! @{
//typedef enum flash_farg_page_num {
	///* 4 of pages to be erased with EPA command*/
	//IFLASH_ERASE_PAGES_4=0,
	///* 8 of pages to be erased with EPA command*/
	//IFLASH_ERASE_PAGES_8,
	///* 16 of pages to be erased with EPA command*/
	//IFLASH_ERASE_PAGES_16,
	///* 32 of pages to be erased with EPA command*/
	//IFLASH_ERASE_PAGES_32,
	///* Parameter is not support */
	//IFLASH_ERASE_PAGES_INVALID,
//}flash_farg_page_num_t;
////! @}
//
///*! \name Flash access mode */
////! @{
//#define FLASH_ACCESS_MODE_128    EFC_ACCESS_MODE_128
//#if (!(SAMV71 || SAMV70 || SAMS70 || SAME70))
//#define FLASH_ACCESS_MODE_64     EFC_ACCESS_MODE_64
//#endif
//! @}

//uint32_t flash_init(uint32_t ul_mode, uint32_t ul_fws);
//uint32_t flash_set_wait_state(uint32_t ul_address, uint32_t ul_fws);
//uint32_t flash_set_wait_state_adaptively(uint32_t ul_address);
//uint32_t flash_get_wait_state(uint32_t ul_address);
//uint32_t flash_get_descriptor(uint32_t ul_address,
		//uint32_t *pul_flash_descriptor,	uint32_t ul_size);
//uint32_t flash_get_page_count(const uint32_t *pul_flash_descriptor);
//uint32_t flash_get_page_count_per_region(const uint32_t *pul_flash_descriptor);
//uint32_t flash_get_region_count(const uint32_t *pul_flash_descriptor);
//uint32_t flash_erase_all(uint32_t ul_address);
//
//#if (SAM3S8 || SAM3SD8)
//uint32_t flash_erase_plane(uint32_t ul_address);
//#endif
//
//#if (SAM4S || SAM4E || SAM4N || SAM4C || SAMG || SAM4CP || SAM4CM || \
	 //SAMV71 || SAMV70 || SAMS70 || SAME70)
//uint32_t flash_erase_page(uint32_t ul_address, uint8_t uc_page_num);
//uint32_t flash_erase_sector(uint32_t ul_address);
//#endif
//
//uint32_t flash_write(uint32_t ul_address, const void *p_buffer,
		//uint32_t ul_size, uint32_t ul_erase_flag);
//uint32_t flash_lock(uint32_t ul_start, uint32_t ul_end,
		//uint32_t *pul_actual_start, uint32_t *pul_actual_end);
//uint32_t flash_unlock(uint32_t ul_start, uint32_t ul_end,
		//uint32_t *pul_actual_start, uint32_t *pul_actual_end);
//uint32_t flash_is_locked(uint32_t ul_start, uint32_t ul_end);
//uint32_t flash_set_gpnvm(uint32_t ul_gpnvm);
//uint32_t flash_clear_gpnvm(uint32_t ul_gpnvm);
//uint32_t flash_is_gpnvm_set(uint32_t ul_gpnvm);
//uint32_t flash_enable_security_bit(void);
//uint32_t flash_is_security_bit_enabled(void);
//uint32_t flash_read_unique_id(uint32_t *pul_data, uint32_t ul_size);
//uint32_t flash_read_user_signature(uint32_t *p_data, uint32_t ul_size);
//uint32_t flash_write_user_signature(const void *p_buffer, uint32_t ul_size);
//uint32_t flash_erase_user_signature(void);

int hexCharToInt(char hex) {
    if (hex >= '0' && hex <= '9')
        return hex - '0';
    else if (hex >= 'a' && hex <= 'f')
        return 10 + (hex - 'a');
    else if (hex >= 'A' && hex <= 'F')
        return 10 + (hex - 'A');
    else
        return -1; // Invalid character
}

static void print_page_buffer()
{
	usb_printf("\r\npage buffer:\r\n");
	for (int i = 0; i<IFLASH_PAGE_SIZE / sizeof(uint32_t);i++)
	{
		usb_printf("%d: %d\r\n",i,page_buffer[i]);
	}
}

static void WriteFlash()
{
	usb_printf("WriteFlash\r\n");
	uint32_t ul_test_page_addr = 0x0040594A; //PAGE_ADDRESS;
	uint32_t *pul_test_page = (uint32_t *) ul_test_page_addr;
	uint32_t ul_rc;
	uint32_t ul_idx;
	uint8_t uc_key;
	uint32_t ul_page_buffer[IFLASH_PAGE_SIZE / sizeof(uint32_t)];
	typedef unsigned long UL;
	
	/* Initialize flash: 6 wait states for flash writing. */
	ul_rc = flash_init(FLASH_ACCESS_MODE_128, 6);
	if (ul_rc != FLASH_RC_OK) {
		//printUSB("-F- Initialization error %lu\n\r", (UL)ul_rc);
		usb_printf("flash_init\r\n");
		return 0;
	}
	
	/* Unlock page */
	//printUSB("-I- Unlocking test page: 0x%08x\r\n", ul_test_page_addr);
	ul_rc = flash_unlock(ul_test_page_addr,
	ul_test_page_addr + IFLASH_PAGE_SIZE - 1, 0, 0);
	if (ul_rc != FLASH_RC_OK) {
		//printUSB("-F- Unlock error %lu\n\r", (UL)ul_rc);
		usb_printf("flash_unlock\r\n");
		return 0;
	}	

	/* Write page */
	//printUSB("-I- Writing test page with walking bit pattern\n\r");
	for (ul_idx = 0; ul_idx < (IFLASH_PAGE_SIZE / 4); ul_idx++) {
		ul_page_buffer[ul_idx] = page_buffer[ul_idx]; //	1 << (ul_idx % 32);
	}
	usb_printf("Write page\r\n");
	
	/* The EWP command is not supported for non-8KByte sectors in all devices
	 *  SAM4 series, so an erase command is requried before the write operation.
	 */
	ul_rc = flash_erase_sector(ul_test_page_addr);
	if (ul_rc != FLASH_RC_OK) {
		//printUSB("-F- Flash programming error %lu\n\r", (UL)ul_rc);
		usb_printf("flash_erase_sector\r\n");
		return 0;
	}

	ul_rc = flash_write(ul_test_page_addr, ul_page_buffer,
			IFLASH_PAGE_SIZE, 0);
	usb_printf("flash_write\r\n");
			
	if (ul_rc != FLASH_RC_OK) {
		//printUSB("-F- Flash programming error %lu\n\r", (UL)ul_rc);
		usb_printf("WriteFlash error\r\n");
		return 0;
	}
	/* Validate page */
	//printUSB("-I- Checking page contents ");
	for (ul_idx = 0; ul_idx < (IFLASH_PAGE_SIZE / 4); ul_idx++) {
		//printUSB(".");
		if (pul_test_page[ul_idx] != ul_page_buffer[ul_idx]) {
			//printUSB("\n\r-F- data error\n\r");
			usb_printf("Validate page\r\n");
			return 0;
		}
	}
	//printUSB("OK\n\r");
	
	/* The EWP command is not supported for non-8KByte sectors in some SAM4
	 * series, so an erase command is requried before the write operation.
	 */
	ul_rc = flash_erase_sector(ul_test_page_addr);
	if (ul_rc != FLASH_RC_OK) {
		//printUSB("-F- Flash programming error %lu\n\r", (UL)ul_rc);
		usb_printf("flash_erase_sector\r\n");
		return 0;
	}
	
	/* Lock page */
	//printUSB("-I- Locking test page\n\r");
	ul_rc = flash_lock(ul_test_page_addr,
	ul_test_page_addr + IFLASH_PAGE_SIZE - 1, 0, 0);
	if (ul_rc != FLASH_RC_OK) {
		//printUSB("-F- Flash locking error %lu\n\r", (UL)ul_rc);
		usb_printf("Lock page\r\n");
		return 0;
	}
	
	/* Check if the associated region is locked. */
	//printUSB("-I- Try to program the locked page ...\n\r");
	ul_rc = flash_write(ul_test_page_addr, ul_page_buffer,
	IFLASH_PAGE_SIZE, 0);
	
	if (ul_rc != FLASH_RC_OK) {
		usb_printf("-I- The page to be programmed belongs to locked region. Error %lu\n\r",
		(UL)ul_rc);
		ul_rc = 0;
	}
	
	///* Set security bit */
	//printUSB("-I- Setting security bit \n\r");
	//ul_rc = flash_enable_security_bit();
	//if (ul_rc != FLASH_RC_OK) {
	//printUSB("-F- Set security bit error %lu\n\r", (UL)ul_rc);
	//}
}

#define TEST_PAGE_ADDRESS (IFLASH_ADDR + IFLASH_SIZE - IFLASH_PAGE_SIZE * 4)
#define FLASH_START_ADDRESS (IFLASH_ADDR + IFLASH_SIZE - IFLASH_PAGE_SIZE * 159)
#define HEX_SIZE 80000

static void testFlash()
{
	typedef unsigned long UL;
	uint32_t ul_test_page_addr = FLASH_START_ADDRESS;
	uint32_t ul_hex_size = HEX_SIZE;
	uint32_t *pul_test_page = (uint32_t *) ul_test_page_addr;
	uint32_t ul_rc;
	uint32_t ul_idx;
	uint8_t uc_key;
	uint32_t ul_page_buffer[20*IFLASH_PAGE_SIZE / sizeof(uint32_t)];
	
	/* Initialize flash: 6 wait states for flash writing. */
	ul_rc = flash_init(FLASH_ACCESS_MODE_128, 6);
	if (ul_rc != FLASH_RC_OK) {
		usb_printf("-F- Initialization error %lu\n\r", (UL)ul_rc);
		return 0;
	}
	/* Unlock page */
	usb_printf("-I- Unlocking test page: 0x%08x\r\n", ul_test_page_addr);

	ul_rc = flash_unlock(ul_test_page_addr,
	ul_test_page_addr + IFLASH_PAGE_SIZE - 1, 0, 0);
	if (ul_rc != FLASH_RC_OK) {
		usb_printf("-F- Unlock error %lu\n\r", (UL)ul_rc);
		return 0;
	}

	/* Write page */
	usb_printf("-I- Writing test page with walking bit pattern\n\r");
	for (ul_idx = 0; ul_idx < (IFLASH_PAGE_SIZE / 4); ul_idx++) {
		ul_page_buffer[ul_idx] = 1 << (ul_idx % 32);
		//printUSB("%lu: %lu\n\r", ul_idx, ul_page_buffer[ul_idx]);
	}
	/* The EWP command is not supported for non-8KByte sectors in all devices
	 *  SAM4 series, so an erase command is requried before the write operation.
	 */
	ul_rc = flash_erase_sector(ul_test_page_addr);
	if (ul_rc != FLASH_RC_OK) {
		usb_printf("-F- Flash programming error %lu\n\r", (UL)ul_rc);
		return 0;
	}

	ul_rc = flash_write(ul_test_page_addr, ul_page_buffer,
			IFLASH_PAGE_SIZE, 0);
	if (ul_rc != FLASH_RC_OK) {
		usb_printf("-F- Flash programming error %lu\n\r", (UL)ul_rc);
		return 0;
	}

	/* Validate page */
	usb_printf("-I- Checking page contents ");
	for (ul_idx = 0; ul_idx < (IFLASH_PAGE_SIZE / 4); ul_idx++) {
		usb_printf(".");
		if (pul_test_page[ul_idx] != ul_page_buffer[ul_idx]) {
			usb_printf("\n\r-F- data error\n\r");
			return 0;
		}
	}
	usb_printf("OK\n\r");		
	
	/* The EWP command is not supported for non-8KByte sectors in some SAM4
	 * series, so an erase command is requried before the write operation.
	 */
	ul_rc = flash_erase_sector(ul_test_page_addr);
	if (ul_rc != FLASH_RC_OK) {
		usb_printf("-F- Flash programming error %lu\n\r", (UL)ul_rc);
		return 0;
	}
	
	/* Lock page */
	//printUSB("-I- Locking test page\n\r");
	//ul_rc = flash_lock(ul_test_page_addr,
	//ul_test_page_addr + IFLASH_PAGE_SIZE - 1, 0, 0);
	//if (ul_rc != FLASH_RC_OK) {
		//printUSB("-F- Flash locking error %lu\n\r", (UL)ul_rc);
		//return 0;
	//}
//
	///* Check if the associated region is locked. */
	//printUSB("-I- Try to program the locked page ...\n\r");
	//ul_rc = flash_write(ul_test_page_addr, ul_page_buffer,
	//IFLASH_PAGE_SIZE,0);
	//
	//if (ul_rc != FLASH_RC_OK) {
		//printUSB("-I- The page to be programmed belongs to locked region. Error %lu\n\r",
		//(UL)ul_rc);
	//}
//
	//printUSB("-I- Please open Segger's JMem program \n\r");
	//printUSB("-I- Read memory at address 0x%08lx to check contents\n\r",
	//(UL)ul_test_page_addr);
//
	//printUSB("-I- Good job!\n\r"
	//"-I- Now set the security bit \n\r"
	//"-I- Press any key to continue to see what happened...\n\r");
//
	///* Set security bit */
	//printUSB("-I- Setting security bit \n\r");
	//ul_rc = flash_enable_security_bit();
	//if (ul_rc != FLASH_RC_OK) {
		//printUSB("-F- Set security bit error %lu\n\r", (UL)ul_rc);
	//}

	usb_printf("-I- All tests done\n\r");
}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

#endif /* CONF_FLASH_H_ */