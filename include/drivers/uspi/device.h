#ifndef USPI_DEVICE_H
#define USPI_DEVICE_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <drivers/usart/device.h>


/* Constants -------------------------------------+-------------------------- */

#ifndef USART_UCPHA_bm
#  define USART_UCPHA_bm                          (1 << 1)
#endif

#ifndef USART_UDORD_bm
#  define USART_UDORD_bm                          (1 << 2)
#endif

#define USPI__DEVICE__INV_CLOCK_bm                (1 << 1)
#define USPI__DEVICE__MODE_MSb_bm                 (0 << 2)
#define USPI__DEVICE__MODE_LSb_bm                 (1 << 2)
#define USPI__DEVICE__MODE_REMAP_bm               (1 << 3)
#define USPI__DEVICE__MODE_W_bm                   (0 << 4)
#define USPI__DEVICE__MODE_RW_bm                  (1 << 4)

/* Data Types --------------------------------------------------------------- */

typedef enum {
#if F_CPU == 32000000
  USPI__DEVICE__B1MHz   = USART__DEVICE__BAUDCTRL(0, 16),
#elif F_CPU == 2000000
  USPI__DEVICE__B1MHz   = USART__DEVICE__BAUDCTRL(0, 0),
#else
#warning No baud rates defined for this clock frequency
#endif
} uspi__device__baudctrl_t;

typedef enum {
  USPI__DEVICE__MODE0 = 0x00,
  USPI__DEVICE__MODE1 = 0x01,
  USPI__DEVICE__MODE2 = 0x02,
  USPI__DEVICE__MODE3 = 0x03,
} uspi__device__mode_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  uspi__device__init(USART_t *device, uspi__device__baudctrl_t baudctrl,
                     uspi__device__mode_t mode)
  NONNULL;
  
void
  uspi__device__write(USART_t *device, const void *data, size_t data_len)
  NONNULL;

void
  uspi__device__read(USART_t *device, void *data, size_t data_len)
  NONNULL;

static inline uint8_t
  uspi__device__exchange(USART_t *device, uint8_t data);


/* Macros ----------------------------------------+--------+----------------- */



/* Inline Function Definitions ---------------------------------------------- */

uint8_t
uspi__device__exchange(USART_t *device, uint8_t data)
{
  usart__device__write_fast(device, data);
  return usart__device__read_fast(device);
}


#endif /* USPI_DEVICE_H */