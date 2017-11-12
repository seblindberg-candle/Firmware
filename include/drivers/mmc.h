#ifndef MMC_H
#define MMC_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <drivers/uspi.h>


/* Constants -------------------------------------+-------------------------- */

#define MMC__CS_PORT                              PORTC
#define MMC__CS_PIN_bm                            PIN4_bm
#define MMC__HOLD_PORT                            PORTC
#define MMC__HOLD_PIN_bm                          PIN0_bm
#define MMC__WP_PORT                              PORTC
#define MMC__WP_PIN_bm                            PIN5_bm

#define MMC__S25FL032P_MANUFACTURER_ID            (0x01)
#define MMC__S25FL032P_DEVICE_ID                  (0x15)

#define MMC__STATUS_WIP_bm                        (1 << 0)
#define MMC__STATUS_WEL_bm                        (1 << 1)
#define MMC__STATUS_E_ERR_bm                      (1 << 5)
#define MMC__STATUS_P_ERR_bm                      (1 << 6)
#define MMC__STATUS_SRWD_bm                       (1 << 7)

#define MMC__PAGE_SIZE                            (256)

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


/* Data Types --------------------------------------------------------------- */

/* Main mmc data type.
 */
typedef struct {
  uspi_t * const interface;
} mmc_t;

/* The full 24 bit address can be expressed with three
 * different resolutions:
 *
 *   [xx xx xx] - address
 *   [xx xx 00] - page
 *   [xx 00 00] - sector
 */
typedef uint32_t mmc__address_t;
typedef uint16_t mmc__page_t;
typedef uint8_t  mmc__sector_t;

/* Common return type for many mmc functions.
 */
typedef enum {
  MMC__OK                 = 0,
  MMC__INVALID_CHIP_ID,
  MMC__BUSY,
} mmc__result_t;

/* Structure of the id field.
 */
typedef struct {
  uint8_t manufacurer_id;
  uint8_t device_id;
} mmc__id_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  mmc__ctor(mmc_t *mmc, uspi_t *interface)
  NONNULL;
  
mmc__result_t
  mmc__verify(const mmc_t *mmc)
  NONNULL;
  
uint8_t
  mmc__read_register(const mmc_t *mmc, mmc__cmd_t cmd)
  NONNULL;
  
void
  mmc__write_registers(const mmc_t *mmc, uint8_t status, uint8_t conf)
  NONNULL;
  
void
  mmc__clear_status_register(const mmc_t *mmc)
  NONNULL;
  
mmc__result_t
  mmc__read_id(const mmc_t *mmc, mmc__id_t *id)
  NONNULL;
  
mmc__result_t
  mmc__read(const mmc_t *mmc, mmc__address_t addr,
            void *data, size_t data_len)
  NONNULL;

mmc__result_t
  mmc__program_page(const mmc_t *mmc, mmc__address_t addr,
                    const void *src, size_t src_len)
  NONNULL;
           
mmc__result_t
  mmc__program(const mmc_t *mmc, mmc__address_t addr,
               const void *src, size_t src_len)
  NONNULL;
               
mmc__result_t
  mmc__erase(const mmc_t *mmc)
  NONNULL;

uint8_t
  mmc__wait_until_ready(const mmc_t *mmc)
  NONNULL;

static inline uint8_t
  mmc__read_status_register(const mmc_t *mmc)
  NONNULL;

static inline uint8_t
  mmc__read_configuration_register(const mmc_t *mmc)
  NONNULL;
  
static inline bool_t
  mmc__is_busy(const mmc_t *mmc)
  NONNULL;


/* Macros ----------------------------------------+--------+----------------- */

#define MMC__PAGE_TO_ADDRESS(page)                ((mmc__address_t) page << 8)
#define MMC__ADDRESS_TO_PAGE(addr)                ((mmc__page_t) addr >> 8)

#define MMC__SECTOR_TO_ADDRESS(sect)              ((mmc__address_t) sect << 16)
#define MMC__ADDRESS_TO_SECTOR(addr)              ((mmc__sector_t) addr >> 16)

#define MMC__PAGE_TO_SECTOR(page)                 ((mmc__sector_t) page << 8)
#define MMC__SECTOR_TO_PAGE(sect)                 ((mmc__page_t) addr >> 8)


/* Inline Function Definitions ---------------------------------------------- */

/* Read Status Register
 */
uint8_t
mmc__read_status_register(const mmc_t *mmc)
{
  return mmc__read_register(mmc, MMC__CMD_RDSR);
}

/* Read Configuration Register
 */
uint8_t
mmc__read_configuration_register(const mmc_t *mmc)
{
  return mmc__read_register(mmc, MMC__CMD_RCR);
}

bool_t
mmc__is_busy(const mmc_t *mmc)
{
  uint8_t status = mmc__read_status_register(mmc);
  return (status & MMC__STATUS_WIP_bm);
}



#endif /* MMC_H */