#include <drivers/mmc.h>
#include <util/delay.h>
#include <stdio.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */
  
static inline void
  write_cmd(const mmc_t *mmc, mmc__cmd_t cmd, mmc__address_t addr);
  
static void
  program_page(const mmc_t *mmc, mmc__address_t *addr,
               const void **src, size_t *src_len);


/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
mmc__ctor(mmc_t *mmc, uspi_t *interface)
{
  WRITE_CONST(mmc->interface, uspi_t *, interface);
  
  /* Make sure the CS is inactive */
  uspi__deselect(interface);
  
  /* Configure GPIO */
  MMC__HOLD_PORT.OUTSET = MMC__HOLD_PIN_bm;
  MMC__HOLD_PORT.DIRSET = MMC__HOLD_PIN_bm;
  
  MMC__WP_PORT.OUTSET   = MMC__WP_PIN_bm;
  MMC__WP_PORT.DIRSET   = MMC__WP_PIN_bm;
  
  /* Clear the device status register */
  mmc__clear_status_register(mmc);
}

/* Verify
 *
 * Read the the manufacturer and device id of the chip and check them against
 * the preset values. Returns MMC__INVALID_CHIP_ID if the values do not match
 * and MMC__OK otherwise.
 */
mmc__result_t
mmc__verify(const mmc_t *mmc)
{
  mmc__id_t     chip_id;
  mmc__result_t res;
  
  res = mmc__read_id(mmc, &chip_id);
  
  if (res != MMC__OK) {
    return res;
  }
    
  if (chip_id.manufacurer_id != MMC__S25FL032P_MANUFACTURER_ID
        || chip_id.device_id != MMC__S25FL032P_DEVICE_ID) {
    return MMC__INVALID_CHIP_ID;
  }
  
  return MMC__OK;
}

mmc__result_t
mmc__read_id(const mmc_t *mmc, mmc__id_t *id)
{
  uspi__transaction(mmc->interface) {
    write_cmd(mmc, MMC__CMD_READ_ID, 0);
    
    //id->manufacurer_id = 0xFF;
    //id->device_id      = 0xFF;
    
    uspi__read(mmc->interface, id, sizeof(mmc__id_t));
  }
  
  return MMC__OK;
}

/* Read
 *
 * Access memory starting from address addr.
 */
mmc__result_t
mmc__read(const mmc_t *mmc, mmc__address_t addr, void *data, size_t data_len)
{
  if (mmc__is_busy(mmc)) {
    return MMC__BUSY;
  }
  
  uspi__transaction(mmc->interface) {
    write_cmd(mmc, MMC__CMD_READ, addr);
    uspi__read(mmc->interface, data, data_len);
  }
  
  return MMC__OK;
}

/* Write Enable
 */
mmc__result_t
mmc__write_enable(const mmc_t *mmc)
{
  /* First make sure the device is not busy
     TODO: this may not be necessary */
  if (mmc__is_busy(mmc)) {
    return MMC__BUSY;
  }
  
  uspi__transaction(mmc->interface) {
    uspi__write_fast(mmc->interface, MMC__CMD_WREN);
  }
  
  return MMC__OK;
}

/* Write Disable
 */
mmc__result_t
mmc__write_disable(const mmc_t *mmc)
{
  uspi__transaction(mmc->interface) {
    uspi__write_fast(mmc->interface, MMC__CMD_WRDI);
  }

  return MMC__OK;
}

uint8_t
mmc__read_register(const mmc_t *mmc, mmc__cmd_t cmd)
{
  uint8_t reg = 0;
  
  uspi__transaction(mmc->interface) {
    uspi__write_fast(mmc->interface, cmd);
    reg = uspi__read_fast(mmc->interface, 0xFF);
  }
  
  return reg;
}

/* Write register
 *
 * Writes to both the status and configuration registers.
 */
void
mmc__write_registers(const mmc_t *mmc, uint8_t status, uint8_t conf)
{
  uspi__transaction(mmc->interface) {
    uspi__write_fast(mmc->interface, MMC__CMD_WRR);
    uspi__write_fast(mmc->interface, status);
    uspi__write_fast(mmc->interface, conf);
  }
}

/* Clear Status Register
 *
 * The Clear Status Register command resets bit SR5 (Erase Fail Flag) and bit
 * SR6 (Program Fail Flag). The WEL bit will be unchanged after this command is
 * executed. This command also resets the State machine and loads latches.
 */
void
mmc__clear_status_register(const mmc_t *mmc)
{
  uspi__transaction(mmc->interface) {
    uspi__write_fast(mmc->interface, MMC__CMD_CLSR);
  }
}

/* Program Page
 *
 * Program (only change 1 -> 0) a single page of data.
 */
mmc__result_t
mmc__program_page(const mmc_t *mmc, mmc__address_t addr,
                  const void *src, size_t src_len)
{
  mmc__result_t res;
  const uint8_t page_offset = (uint8_t) addr;
  
  /* This is to guard against writing to the same page over
     and over as the write position wraps around past the
     end of the page */
  assert(src_len <= 265 - page_offset);
  
  if (src_len == 0) {
    return MMC__OK;
  }
  
  res = mmc__write_enable(mmc);
  if (res != MMC__OK) {
    return res;
  }
  
  /* TODO: Calculate how much we actually want to write (to
     avoid wraping around to the beginning of the page) */
  // if (page_offset != 0) {
  //
  // }
  
  uspi__transaction(mmc->interface) {
    write_cmd(mmc, MMC__CMD_PP, addr);
    uspi__write(mmc->interface, src, src_len);
  }
  
  return MMC__OK;
}

void
program_page(const mmc_t *mmc, mmc__address_t *addr,
             const void **src, size_t *src_len)
{
  mmc__result_t res;
  const uint8_t page_offset = (uint8_t) *addr;
  mmc__page_t to_write;
     
  res = mmc__write_enable(mmc);
  if (res != MMC__OK) {
    return;
  }

  /* Calculate how much we actually want to write (to avoid
     wraping around to the beginning of the page) */
  if (page_offset == 0) {
    to_write = MMC__PAGE_SIZE;
  } else {
    to_write = MMC__PAGE_SIZE - page_offset;
  }
  
  if (to_write > *src_len) {
    to_write = *src_len;
    *src_len = 0;
  } else {
    *src_len -= to_write;
  }

  uspi__transaction(mmc->interface) {
    write_cmd(mmc, MMC__CMD_PP, *addr);
    uspi__write(mmc->interface, *src, to_write);
  }
  
  *addr += to_write;
  *src  += to_write;

  return;
}

/* Program
 *
 * Program (only change 1 -> 0) any section of data.
 */
mmc__result_t
mmc__program(const mmc_t *mmc, mmc__address_t addr,
             const void *src, size_t src_len)
{
  const uint8_t page_offset = (uint8_t) addr;
    
  if (src_len == 0) {
    return MMC__OK;
  }
  
  do {
    program_page(mmc, &addr, &src, &src_len);
    /* Wait for the programming to finish */
    
  } while (src_len > 0);
  
  if (mmc__is_busy(mmc)) {
    return MMC__BUSY;
  }
  
  /* Program the first page (if partial) */
  if (page_offset) {
    const uint8_t to_write = (uint8_t) ((uint16_t) 256 - page_offset);
    
    mmc__program_page(mmc, addr, src, to_write);
    
    addr    += to_write;
    src     += to_write;
    src_len -= to_write;
  }
    
  while (src_len >= 256) {
    mmc__program_page(mmc, addr, src, 256);
    
    addr    += 256;
    src     += 256;
    src_len -= 256;
  }
  
  /* Program the last page of data */
  mmc__program_page(mmc, addr, src, src_len);
  
  return MMC__OK;
}

mmc__result_t
mmc__erase_sector(const mmc_t *mmc, mmc__sector_t sector)
{
  if (mmc__is_busy(mmc)) {
    return MMC__BUSY;
  }
  
  uspi__transaction(mmc->interface) {
    write_cmd(mmc, MMC__CMD_SE, MMC__SECTOR_TO_ADDRESS(sector));
  }
  
  return MMC__OK;
}

/* Erase
 *
 * Sets all the bits in the memory array to logic 1.
 */
mmc__result_t
mmc__erase(const mmc_t *mmc)
{
  const uint8_t res = mmc__write_enable(mmc);
  
  if (res != MMC__OK) {
    return res;
  }
  
  uspi__transaction(mmc->interface) {
    uspi__write_fast(mmc->interface, MMC__CMD_BE);
  }
  
  return MMC__OK;
}

/* Wait Until Ready
 *
 * Continously reads the status register until the WIP bit is cleared. Returns
 * the status register.
 */
uint8_t
mmc__wait_until_ready(const mmc_t *mmc)
{
  uint8_t status = 0;
  
  uspi__transaction(mmc->interface) {
    uspi__write_fast(mmc->interface, MMC__CMD_RDSR);
    do {
      status = uspi__read_fast(mmc->interface, 0xFF);
    } while (status & MMC__STATUS_WIP_bm);
  }
  
  return status;
}

void
write_cmd(const mmc_t *mmc, mmc__cmd_t cmd, mmc__address_t addr)
{
  assert(uspi__is_selected(mmc->interface));
  
  uspi__write_fast(mmc->interface, cmd);
  
  uspi__write_fast(mmc->interface, (uint8_t) (addr << 16));
  uspi__write_fast(mmc->interface, (uint8_t) (addr <<  8));
  uspi__write_fast(mmc->interface, (uint8_t) (addr <<  0));
}