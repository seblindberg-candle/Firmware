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
  SPI__DEVICE__MODE0_MSb = 0,
  SPI__DEVICE__MODE1_MSb = 1,
  SPI__DEVICE__MODE2_MSb = 2,
  SPI__DEVICE__MODE3_MSb = 3,
  SPI__DEVICE__MODE0_LSb = 4,
  SPI__DEVICE__MODE1_LSb = 5,
  SPI__DEVICE__MODE2_LSb = 6,
  SPI__DEVICE__MODE3_LSb = 7,
} spi__device__mode_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  spi__device__init(SPI_t *device, spi__device__baudctrl_t baudctrl,
                    spi__device__mode_t mode)
  NONNULL;

void
  spi__device__write(SPI_t *device, const void *data, size_t data_len)
  NONNULL_ARGS(1);

void
  spi__device__read(SPI_t *device, void *data, size_t data_len)
  NONNULL_ARGS(1);

static inline void
  spi__device__write_fast(SPI_t *device, uint8_t data)
  NONNULL;

static inline uint8_t
  spi__device__read_fast(SPI_t *device, uint8_t data)
  NONNULL;



/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */

void
spi__device__write_fast(SPI_t *device, uint8_t data)
{

}

uint8_t
spi__device__read_fast(SPI_t *device, uint8_t data)
{
  return 0;
}

#endif /* SPI_DEVICE_H */