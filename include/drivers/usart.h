#ifndef USART_H
#define USART_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>


/* Constants -------------------------------------+-------------------------- */

#define USART__BAUDCTRL(bscale, bsel)             \
                                    (uint16_t)((bscale << 12) | (bsel & 0x0FFF))

#define USART__CLK2X_bm                           (1 << 2)

/* Data Types --------------------------------------------------------------- */

typedef uint32_t usart__baud_rate_t;
typedef uint16_t usart__bsel_t;
typedef int8_t   usart__bscale_t;

typedef enum {
  USART__NO_REMAP = 0,
  USART__REMAP    = 1
} usart__remap_t;

typedef enum {
  USART__CLK1X = 0,
  USART__CLK2X = 1
} usart__clk_t;

typedef enum {
  USART__RW            = USART_TXEN_bm | USART_RXEN_bm,
  USART__READ_ONLY     = USART_RXEN_bm,
  USART__WRITE_ONLY    = USART_TXEN_bm,
  USART__RW_X2         = USART_TXEN_bm | USART_RXEN_bm | USART_CLK2X_bm,
  USART__READ_ONLY_X2  = USART_RXEN_bm | USART_CLK2X_bm,
  USART__WRITE_ONLY_X2 = USART_TXEN_bm | USART_CLK2X_bm,
} usart__config_t;

typedef enum {
#if F_CPU == 32000000
  USART__B9600   = USART__BAUDCTRL(4, 12),
  USART__B57600  = USART__BAUDCTRL(-2, 135),
  USART__B115200 = USART__BAUDCTRL(-3, 131),
#elif F_CPU == 2000000
  USART__B57600  = USART__BAUDCTRL(-6, 75),
#else
#warning No baud rates defined for this clock frequency
#endif
} usart__baudctrl_t;

/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  usart__init(USART_t *module, usart__baudctrl_t baudctrl,
              usart__config_t conf);

void
  usart__map_io(USART_t *module, usart__remap_t remap);

void
  usart__write(USART_t *module, const void *data, size_t data_len);
  
void
  usart__read(USART_t *module, void *data, size_t data_len);
  
static inline void
  usart__wait_dr_ready(USART_t *module);
  
static inline void
  usart__wait_rx_ready(USART_t *module);
  
static inline void
  usart__wait_tx_ready(USART_t *module);
  
static inline bool_t
  usart__is_read_enabled(USART_t *module);

static inline bool_t
  usart__is_write_enabled(USART_t *module);

static inline void
  usart__write_fast(USART_t *module, const uint8_t data);
  
static inline uint8_t
  usart__read_fast(USART_t *module);

/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */


void
usart__wait_dr_ready(USART_t *module)
{
  while (!(module->STATUS & USART_DREIF_bm)) ;
}

void
usart__wait_rx_ready(USART_t *module)
{
  while (!(module->STATUS & USART_RXCIF_bm)) ;
}

void
usart__wait_tx_ready(USART_t *module)
{
  while (!(module->STATUS & USART_TXCIF_bm)) ;
}

bool_t
usart__is_read_enabled(USART_t *module)
{
  return (module->CTRLB & USART_RXEN_bm);
}

bool_t
usart__is_write_enabled(USART_t *module)
{
  return (module->CTRLB & USART_TXEN_bm);
}

void
usart__write_fast(USART_t *module, const uint8_t data)
{
  usart__wait_dr_ready(module);
  module->DATA = data;
}


uint8_t
usart__read_fast(USART_t *module)
{
  usart__wait_rx_ready(module);
  return module->DATA;
}

#endif /* USART_H */