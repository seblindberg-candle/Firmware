#ifndef MMC_H
#define MMC_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <drivers/uspi.h>


/* Constants -------------------------------------+-------------------------- */

/* Command table */
typedef enum {
  /* Read */
  MMC__CMD_READ      = 0x03, /* Read Data bytes */
  MMC__CMD_FAST_READ = 0x0B, /* Read Data bytes at Fast Speed */
  MMC__CMD_DOR       = 0x3B, /* Dual Output Read */
  MMC__CMD_QOR       = 0x6B, /* Quad Output Read */
  MMC__CMD_DIOR      = 0xBB, /* Dual I/O High Performance Read */
  MMC__CMD_QIOR      = 0xEB, /* Quad I/O High Performance Read */
  MMC__CMD_RDID      = 0x9F, /* Read Identification */
  MMC__CMD_READ_ID   = 0x90, /* Read Manufacturer and Device Identification */
  /* Write Control */
  MMC__CMD_WREN      = 0x06, /* Write Enable */
  MMC__CMD_WRDI      = 0x04, /* Write Disable */
  /* Erase */
  MMC__CMD_P4E       = 0x20, /* 4 KB Parameter Sector Erase */
  MMC__CMD_P8E       = 0x40, /* 8 KB (two 4KB) Parameter Sector Erase */
  MMC__CMD_SE        = 0xD8, /* 64KB Sector Erase */
  MMC__CMD_BE        = 0x60, /* Bulk Erase */
  /* Program */
  MMC__CMD_PP        = 0x02, /* Page Programming */
  MMC__CMD_QPP       = 0x32, /* Quad Page Programming */
  /* Status & Configuration Register */
  MMC__CMD_RDSR      = 0x05, /* Read Status Register */
  MMC__CMD_WRR       = 0x01, /* Write (Status & Configuration) Register */
  MMC__CMD_RCR       = 0x35, /* Read Configuration Register (CFG) */
  MMC__CMD_CLSR      = 0x30, /* Reset the Erase and Program Fail Flag (SR5 and
                                SR6) and restore normal operation)*/
  /* Power Saving */
  MMC__CMD_DP        = 0xB9, /* Deep Power-Down */
  MMC__CMD_RES       = 0xAB, /* Release from Deep Power-Down Mode */
  /* OTP */
  MMC__CMD_OTPP      = 0x42, /* Program one byte of data in OTP memory space */
  MMC__CMD_OTPR      = 0x4B  /* Read data in the OTP memory space */
} mmc__cmd_t;

#define MMC__S25FL032P_MANUFACTURER_ID            0x01
#define MMC__S25FL032P_DEVICE_ID                  0x15


/* Data Types --------------------------------------------------------------- */

typedef struct {
  USART_t *module;
  uspi__chip_select cs;
} mmc_t;

typedef union {
  uint8_t  addr[3];
  // uint8_t  uint8;
  // uint16_t uint16;
} mmc__address_t;

typedef enum {
  MMC__OK                 = 0,
  MMC__INVALID_CHIP_ID
} mmc__result_t;

typedef struct {
  uint8_t manufacurer_id;
  uint8_t device_id;
} mmc__id_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  mmc__ctor(mmc_t *mmc, USART_t *module, uspi__chip_select cs);
  
mmc__result_t
  mmc__verify(const mmc_t *mmc);
  
uint8_t
  mmc__read_register(const mmc_t *mmc, mmc__cmd_t cmd);
  
mmc__result_t
  mmc__read_id(const mmc_t *mmc, mmc__id_t *id);

mmc__result_t
#if NDEBUG
  mmc__program_page_fast(mmc_t *mmc, uint16_t page, uint8_t *src);
#define mmc__program_page(mmc, page, src, len) \
  mmc__program_page_fast(mmc, page, src)
#else
  mmc__program_page(mmc_t *mmc, uint16_t page, uint8_t *src, size_t src_len);
#endif

static inline uint8_t
  mmc__read_status_register(const mmc_t *mmc);

static inline uint8_t
  mmc__read_configuration_register(const mmc_t *mmc);


/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */

uint8_t
mmc__read_status_register(const mmc_t *mmc)
{
  return mmc__read_register(mmc, MMC__CMD_RDSR);
}

uint8_t
mmc__read_configuration_register(const mmc_t *mmc)
{
  return mmc__read_register(mmc, MMC__CMD_RCR);
}

#endif /* MMC_H */