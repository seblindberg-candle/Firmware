#ifndef SPI_DEVICE_H
#define SPI_DEVICE_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>


/* Constants -------------------------------------+-------------------------- */




/* Data Types --------------------------------------------------------------- */

typedef enum {
#if F_CPU == 32000000
  SPI__B1MHz   = 0,
#elif F_CPU == 2000000
  SPI__B1MHz   = 0,
#else
#warning No baud rates defined for this clock frequency
#endif
} spi__device__baudctrl_t;

typedef enum {
  SPI__DEVICE__MODE0 = 0x00,
  SPI__DEVICE__MODE1 = 0x01,
  SPI__DEVICE__MODE2 = 0x02,
  SPI__DEVICE__MODE3 = 0x03,
} spi__device__mode_t;

/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  spi__device__init(SPI_t *device, spi__device__baudctrl_t baudctrl,
                    spi__device__mode_t mode)
  NONNULL;

void
  spi__device__write(SPI_t *device, const void *data, size_t data_len)
  NONNULL;

void
  spi__device__read(SPI_t *device, void *data, size_t data_len)
  NONNULL;

static inline void
  spi__device__wait_tx_ready(SPI_t *device)
  NONNULL;

static inline uint8_t
  spi__device__exchange(SPI_t *device, uint8_t data)
  NONNULL;


/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */

void
spi__device__wait_tx_ready(SPI_t *device)
{
  while ( !(device->STATUS & SPI_IF_bm) ) ;
}

uint8_t
spi__device__exchange(SPI_t *device, uint8_t data)
{
  uint8_t res;

  device->DATA = data;

  spi__device__wait_tx_ready(device);

  /* Read received data. */
  res = device->DATA;

  return res;
}

#endif /* SPI_DEVICE_H */