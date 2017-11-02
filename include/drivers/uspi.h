#ifndef SPI_H
#define SPI_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <drivers/usart.h>


/* Constants -------------------------------------+-------------------------- */

#define USART_UCPHA_bm                            (1 << 1)
#define USART_UDORD_bm                            (1 << 2)


/* Data Types --------------------------------------------------------------- */

typedef void (*uspi__chip_select)(bool_t select);

typedef enum {
#if F_CPU == 32000000
  USPI__B1MHz   = USART__BAUDCTRL(0, 16),
#elif F_CPU == 2000000
  USPI__B1MHz   = USART__BAUDCTRL(0, 0),
#else
#warning No baud rates defined for this clock frequency
#endif
} uspi__baudctrl_t;

typedef enum {
  USPI__MODE0_MSb = 0,
  USPI__MODE1_MSb = 1,
  USPI__MODE2_MSb = 2,
  USPI__MODE3_MSb = 3,
  USPI__MODE0_LSb = 4,
  USPI__MODE1_LSb = 5,
  USPI__MODE2_LSb = 6,
  USPI__MODE3_LSb = 7,
} uspi__mode_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  uspi__init(USART_t *module, uspi__baudctrl_t baudctrl, uspi__mode_t mode);
  
static inline void
  uspi__write(USART_t *module, const void *data, size_t data_len);
  
void
  uspi__read(USART_t *module, void *data, size_t data_len);
  
static inline void
  uspi__write_fast(USART_t *module, uint8_t data);
    
static inline uint8_t
  uspi__read_fast(USART_t *module, uint8_t data);


/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */

void
  uspi__write_fast(USART_t *module, uint8_t data)
{
  usart__write_fast(module, data);
}

uint8_t
  uspi__read_fast(USART_t *module, uint8_t data)
{
  usart__write_fast(module, data);
  usart__wait_rx_ready(module);
  
  return module->DATA;
}

void
uspi__write(USART_t *module, const void *data, size_t data_len)
{
  usart__write(module, data, data_len);
}



#endif /* SPI_H */