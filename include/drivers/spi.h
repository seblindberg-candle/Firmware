#ifndef SPI_H
#define SPI_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>


/* Constants -------------------------------------+-------------------------- */




/* Data Types --------------------------------------------------------------- */

typedef void (*spi__chip_select)(bool_t select);

typedef enum {
#if F_CPU == 32000000
  SPI__B1MHz   = 0,
#elif F_CPU == 2000000
  SPI__B1MHz   = 0,
#else
#warning No baud rates defined for this clock frequency
#endif
} spi__baudctrl_t;

typedef enum {
  SPI__MODE0_MSb = 0,
  SPI__MODE1_MSb = 1,
  SPI__MODE2_MSb = 2,
  SPI__MODE3_MSb = 3,
  SPI__MODE0_LSb = 4,
  SPI__MODE1_LSb = 5,
  SPI__MODE2_LSb = 6,
  SPI__MODE3_LSb = 7,
} spi__mode_t;



typedef struct {
  SPI_t  *module;
  PORT_t *cs_port;
  uint8_t cs_bm;
} spi_t;

/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  spi__init(SPI_t *module, spi__baudctrl_t baudctrl, spi__mode_t mode);

void
  spi__write(SPI_t *module, const void *data, size_t data_len);

void
  spi__read(SPI_t *module, void *data, size_t data_len);

static inline void
  spi__write_fast(SPI_t *module, uint8_t data);

static inline uint8_t
  spi__read_fast(SPI_t *module, uint8_t data);
  
static inline uint8_t
  spi__assert(spi_t *spi);
  
static inline void
  spi__deassert(spi_t *spi);


/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */

void
spi__write_fast(SPI_t *module, uint8_t data)
{
  
}

uint8_t
spi__read_fast(SPI_t *module, uint8_t data)
{
  return 0;
}

uint8_t
spi__assert(spi_t *spi)
{
  spi->cs_port->OUTCLR = spi->cs_bm;
  return 1;
}

void
spi__deassert(spi_t *spi)
{
  spi->cs_port->OUTSET = spi->cs_bm;
}

#endif /* SPI_H */