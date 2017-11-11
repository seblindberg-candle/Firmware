#include <drivers/mmc.h>
#include <util/delay.h>
#include <stdio.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */
  
static inline void
  write_cmd(const mmc_t *mmc, mmc__cmd_t cmd, mmc__address_t addr);


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

/* TODO: This is a mess that needs to be cleaned up later */
mmc__result_t
mmc__verify(const mmc_t *mmc)
{
  mmc__id_t chip_id;
  mmc__read_id(mmc, &chip_id);
    
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
    
    id->manufacurer_id = 0xFF;
    id->device_id      = 0xFF;
    
    uspi__read(mmc->interface, id, sizeof(mmc__id_t));
  }
  
  return MMC__OK;
}

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

mmc__result_t
mmc__write_enable(const mmc_t *mmc)
{
  /* First make sure the device is not busy */
  if (mmc__is_busy(mmc)) {
    return MMC__BUSY;
  }
  
  uspi__transaction(mmc->interface) {
    uspi__write_fast(mmc->interface, MMC__CMD_WREN);
  }
  
  return MMC__OK;
}

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
 * Program (only change 0 -> 1) a single page of data.
 */
mmc__result_t
mmc__program_page(const mmc_t *mmc, mmc__page_t page,
                  const void *src, size_t src_len)
{
  mmc__result_t res;
  assert(src_len <= 265);
  
  if (src_len == 0) {
    return MMC__OK;
  }
  
  res = mmc__write_enable(mmc);
  if (res != MMC__OK) {
    return res;
  }
  
  uspi__transaction(mmc->interface) {
    write_cmd(mmc, MMC__CMD_PP, MMC__PAGE_TO_ADDRESS(page));
    uspi__write(mmc->interface, src, src_len);
  }
  
  return MMC__OK;
}

/* Program
 *
 * Program (only change 0 -> 1) any section of data.
 */
mmc__result_t
mmc__program(const mmc_t *mmc, mmc__address_t addr,
             const void *src, size_t src_len)
{
  mmc__page_t page;
    
  if (src_len == 0) {
    return MMC__OK;
  }
  
  if (mmc__is_busy(mmc)) {
    return MMC__BUSY;
  }
  
  /* Program the first page (if partial) */
  if ((uint8_t) addr) {
    /* TODO: Figure out how to program with a 24 bit address */
    //mmc__program_page(mmc)
  }
  
  page = MMC__ADDRESS_TO_PAGE(addr) + 1;
  
  while (src_len >= 256) {
    mmc__program_page(mmc, page, src, 256);
    src_len -= 256;
    page ++;
  }
  
  /* Program the last page of data */
  mmc__program_page(mmc, page, src, src_len);
  
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