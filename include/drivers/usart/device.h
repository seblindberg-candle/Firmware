#ifndef USART_DEVICE_H
#define USART_DEVICE_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>


/* Constants -------------------------------------+-------------------------- */

#define USART__DEVICE__BAUDCTRL(bscale, bsel)             \
                                    (uint16_t)((bscale << 12) | (bsel & 0x0FFF))

#define USART__DEVICE__CLK2X_bm                           (1 << 2)

/* Data Types --------------------------------------------------------------- */

typedef uint32_t usart__device__baud_rate_t;
typedef uint16_t usart__device__bsel_t;
typedef int8_t   usart__device__bscale_t;

typedef enum {
  USART__DEVICE__NO_REMAP = 0,
  USART__DEVICE__REMAP    = 1
} usart__device__remap_t;

typedef enum {
  USART__DEVICE__CLK1X = 0,
  USART__DEVICE__CLK2X = 1
} usart__device__clk_t;

typedef enum {
  USART__DEVICE__RW            = USART_TXEN_bm | USART_RXEN_bm,
  USART__DEVICE__READ_ONLY     = USART_RXEN_bm,
  USART__DEVICE__WRITE_ONLY    = USART_TXEN_bm,
  USART__DEVICE__RW_X2         = USART_TXEN_bm | USART_RXEN_bm | USART_CLK2X_bm,
  USART__DEVICE__READ_ONLY_X2  = USART_RXEN_bm | USART_CLK2X_bm,
  USART__DEVICE__WRITE_ONLY_X2 = USART_TXEN_bm | USART_CLK2X_bm,
} usart__device__config_t;

typedef enum {
#if F_CPU == 32000000
  USART__DEVICE__B9600   = USART__DEVICE__BAUDCTRL(4, 12),
  USART__DEVICE__B57600  = USART__DEVICE__BAUDCTRL(-2, 135),
  USART__DEVICE__B115200 = USART__DEVICE__BAUDCTRL(-3, 131),
#elif F_CPU == 2000000
  USART__DEVICE__B57600  = USART__DEVICE__BAUDCTRL(-6, 75),
#else
#warning No baud rates defined for this clock frequency
#endif
} usart__device__baudctrl_t;

/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  usart__device__init(USART_t *device, usart__device__baudctrl_t baudctrl,
              usart__device__config_t conf);

void
  usart__device__map_io(USART_t *device, usart__device__remap_t remap);

void
  usart__device__write(USART_t *device, const void *data, size_t data_len);

void
  usart__device__read(USART_t *device, void *data, size_t data_len);

static inline void
  usart__device__wait_dr_ready(USART_t *device);

static inline void
  usart__device__wait_rx_ready(USART_t *device);

static inline void
  usart__device__wait_tx_ready(USART_t *device);

static inline bool_t
  usart__device__is_read_enabled(USART_t *device);

static inline bool_t
  usart__device__is_write_enabled(USART_t *device);

static inline void
  usart__device__write_fast(USART_t *device, const uint8_t data);

static inline uint8_t
  usart__device__read_fast(USART_t *device);

/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */


void
usart__device__wait_dr_ready(USART_t *device)
{
  while (!(device->STATUS & USART_DREIF_bm)) ;
}

void
usart__device__wait_rx_ready(USART_t *device)
{
  while (!(device->STATUS & USART_RXCIF_bm)) ;
}

void
usart__device__wait_tx_ready(USART_t *device)
{
  while (!(device->STATUS & USART_TXCIF_bm)) ;
}

bool_t
usart__device__is_read_enabled(USART_t *device)
{
  return (device->CTRLB & USART_RXEN_bm);
}

bool_t
usart__device__is_write_enabled(USART_t *device)
{
  return (device->CTRLB & USART_TXEN_bm);
}

void
usart__device__write_fast(USART_t *device, const uint8_t data)
{
  usart__device__wait_dr_ready(device);
  device->DATA = data;
}


uint8_t
usart__device__read_fast(USART_t *device)
{
  usart__device__wait_rx_ready(device);
  return device->DATA;
}

#endif /* USART_DEVICE_H */