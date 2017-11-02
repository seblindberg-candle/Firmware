#include <drivers/mmc.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

static inline void
  enable_cs(const mmc_t *mmc);
  
static inline void
  disable_cs(const mmc_t *mmc);
  
static inline void
  write_cmd(const mmc_t *mmc, mmc__cmd_t cmd, mmc__address_t *addr);


/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
mmc__ctor(mmc_t *mmc, USART_t *module, uspi__chip_select cs)
{
  mmc->module = module;
  mmc->cs     = cs;
  
  cs(0);
}

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
  mmc__address_t addr;
  
  addr.addr[0] = 0x00;
  addr.addr[1] = 0x00;
  addr.addr[2] = 0x00;
  
  enable_cs(mmc);
  write_cmd(mmc, MMC__CMD_READ_ID, &addr);
  
  id->manufacurer_id = 0;
  id->device_id = 0;
  
  uspi__read(mmc->module, id, sizeof(mmc__id_t));
  
  disable_cs(mmc);
  
  return MMC__OK;
}

mmc__result_t
mmc__write_enable(const mmc_t *mmc)
{
  enable_cs(mmc);
  uspi__write_fast(mmc->module, MMC__CMD_WREN);
  disable_cs(mmc);
  
  return MMC__OK;
}

mmc__result_t
mmc__write_disable(const mmc_t *mmc)
{
  enable_cs(mmc);
  uspi__write_fast(mmc->module, MMC__CMD_WRDI);
  disable_cs(mmc);

  return MMC__OK;
}

uint8_t
mmc__read_register(const mmc_t *mmc, mmc__cmd_t cmd)
{
  uint8_t reg;
  
  enable_cs(mmc);
  uspi__write_fast(mmc->module, cmd);
  reg = uspi__read_fast(mmc->module, 0);
  disable_cs(mmc);
  
  return reg;
}

mmc__result_t
mmc__write_register(const mmc_t *mmc)
{
  return MMC__OK;
}

mmc__result_t
#if NDEBUG
mmc__program_page_fast(mmc_t *mmc, uint16_t page, uint8_t *src)
#else
mmc__program_page(mmc_t *mmc, uint16_t page, uint8_t *src, size_t src_len)
#endif
{
  assert(src_len == 265);
  return MMC__OK;
}

mmc__result_t
mmc__program(mmc_t *mmc, uint32_t addr, uint8_t *src, size_t src_len)
{
  return MMC__OK;
}

mmc__result_t
mmc__erase_sector(mmc_t *mmc, uint16_t sector)
{
  return MMC__OK;
}

mmc__result_t
mmc__erase(mmc_t *mmc)
{
  return MMC__OK;
}

void
enable_cs(const mmc_t *mmc)
{
  mmc->cs(1);
}

void
disable_cs(const mmc_t *mmc)
{
  mmc->cs(0);
}

void
write_cmd(const mmc_t *mmc, mmc__cmd_t cmd, mmc__address_t *addr)
{
  uspi__write_fast(mmc->module, cmd);
  
  uspi__write_fast(mmc->module, addr->addr[2]);
  uspi__write_fast(mmc->module, addr->addr[1]);
  uspi__write_fast(mmc->module, addr->addr[0]);
  
  //uspi__write(mmc->module, addr, sizeof(mmc__address_t));
}