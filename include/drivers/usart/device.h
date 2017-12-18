#ifndef USART_DEVICE_H
#define USART_DEVICE_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>


/* Constants -------------------------------------+-------------------------- */

#define USART__DEVICE__BAUDCTRL(bscale, bsel)             \
                                    (uint16_t)((bscale << 12) | (bsel & 0x0FFF))

#define USART__DEVICE__CLK2X_bm                           (1 << 2)

#define USART__DEVICE__PIN_XCK_bm                 PIN1_bm
#define USART__DEVICE__PIN_RX_bm                  PIN2_bm
#define USART__DEVICE__PIN_TX_bm                  PIN3_bm

#define USART__DEVICE__REMAPPED_PIN_XCK_bm        PIN5_bm
#define USART__DEVICE__REMAPPED_PIN_RX_bm         PIN6_bm
#define USART__DEVICE__REMAPPED_PIN_TX_bm         PIN7_bm

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
  USART__DEVICE__B9600   = USART__DEVICE__BAUDCTRL(-6, 769),
  USART__DEVICE__B57600  = USART__DEVICE__BAUDCTRL(-6, 75),
#else
#warning No baud rates defined for this clock frequency
#endif
} usart__device__baudctrl_t;

/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  usart__device__init(USART_t *device, usart__device__baudctrl_t baudctrl,
              usart__device__config_t conf)
  NONNULL;

static inline PORT_t *
  usart__device__port(USART_t *device)
  NONNULL;

static inline void
  usart__device__enable_power(USART_t *device)
  NONNULL;

void
  usart__device__map_gpio(USART_t *device, usart__device__remap_t remap)
  NONNULL;

void
  usart__device__write(USART_t *device, const void *data, size_t data_len)
  NONNULL;

void
  usart__device__read(USART_t *device, void *data, size_t data_len)
  NONNULL;
  
static inline bool_t
  usart__device__is_remaped(USART_t *device)
  NONNULL;

static inline bool_t
  usart__device__is_dre(USART_t *device)
  NONNULL;

static inline void
  usart__device__wait_dr_ready(USART_t *device)
  NONNULL;

static inline void
  usart__device__wait_rx_ready(USART_t *device)
  NONNULL;

static inline void
  usart__device__wait_tx_ready(USART_t *device)
  NONNULL;

static inline bool_t
  usart__device__is_read_enabled(USART_t *device)
  NONNULL;

static inline bool_t
  usart__device__is_write_enabled(USART_t *device)
  NONNULL;

static inline void
  usart__device__write_fast(USART_t *device, const uint8_t data)
  NONNULL;

static inline uint8_t
  usart__device__read_fast(USART_t *device)
  NONNULL;

/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */

/* Port
 *
 * Returns the port of the given USART device.
 */
PORT_t *
usart__device__port(USART_t *device)
{
  if (device == &USARTC0) {
    return &PORTC;
  } else {
    assert(device == &USARTD0);
    return &PORTD;
  }
}

/* Enable Power
 *
 * Clears the USART0 bit in the global power reduction register.
 */
void
usart__device__enable_power(USART_t *device)
{
  if (device == &USARTC0) {
    PR.PRPC &= ~PR_USART0_bm;
  } else {
    assert(device == &USARTD0);
    PR.PRPD &= ~PR_USART0_bm;
  }
}

/* Is Remaped
 */
bool_t
usart__device__is_remaped(USART_t *device)
{
  PORT_t *port = usart__device__port(device);
  return (port->REMAP & PORT_USART0_bm);
}

/* Is Data Register Empty
 */
bool_t
usart__device__is_dre(USART_t *device)
{
  return device->STATUS & USART_DREIF_bm;
}

/* Wait Data Register Empty
 */
void
usart__device__wait_dr_ready(USART_t *device)
{
  while (!usart__device__is_dre(device)) ;
}

/* Wait RX Ready
 */
void
usart__device__wait_rx_ready(USART_t *device)
{
  while (!(device->STATUS & USART_RXCIF_bm)) ;
}

/* Wait TX Ready
 */
void
usart__device__wait_tx_ready(USART_t *device)
{
  while (!(device->STATUS & USART_TXCIF_bm)) ;
}

/* Is Read Enabled
 *
 * Returns non-zero if RX is enabled for the device.
 */
bool_t
usart__device__is_read_enabled(USART_t *device)
{
  return (device->CTRLB & USART_RXEN_bm);
}

/* Is Write Enabled
 *
 * Returns non-zero if TX is enabled for the device.
 */
bool_t
usart__device__is_write_enabled(USART_t *device)
{
  return (device->CTRLB & USART_TXEN_bm);
}

/* Write Fast
 */
void
usart__device__write_fast(USART_t *device, const uint8_t data)
{
  assert(usart__device__is_write_enabled(device));
  usart__device__wait_dr_ready(device);
  device->DATA = data;
}

/* Read Fast
 *
 * Inline function to quickly read one byte from the device.
 */
uint8_t
usart__device__read_fast(USART_t *device)
{
  uint8_t data;
  
  assert(usart__device__is_read_enabled(device));
  usart__device__wait_rx_ready(device);
  data = device->DATA;
  return data;
}

#endif /* USART_DEVICE_H */