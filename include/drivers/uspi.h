#ifndef SPI_H
#define SPI_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <drivers/usart/device.h>


/* Constants -------------------------------------+-------------------------- */

#define USART_UCPHA_bm                            (1 << 1)
#define USART_UDORD_bm                            (1 << 2)


/* Data Types --------------------------------------------------------------- */

typedef struct {
  USART_t *device;
  PORT_t  *cs_port;
  uint8_t  cs_bm;
} uspi_t;

typedef void (*uspi__chip_select)(bool_t select);

typedef enum {
#if F_CPU == 32000000
  USPI__B1MHz   = USART__DEVICE__BAUDCTRL(0, 16),
#elif F_CPU == 2000000
  USPI__B1MHz   = USART__DEVICE__BAUDCTRL(0, 0),
#else
#warning No baud rates defined for this clock frequency
#endif
} uspi__baudctrl_t;

typedef enum {
  USPI__MODE0_MSb = 0x00,
  USPI__MODE1_MSb = 0x01,
  USPI__MODE2_MSb = 0x02,
  USPI__MODE3_MSb = 0x03,
  USPI__MODE0_LSb = 0x04,
  USPI__MODE1_LSb = 0x05,
  USPI__MODE2_LSb = 0x06,
  USPI__MODE3_LSb = 0x07,
  USPI__REMAP     = 0x08,
} uspi__mode_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  uspi__ctor(uspi_t *uspi, USART_t *device, PORT_t *cs_port, uint8_t cs_bm);

static inline void
  uspi__init(uspi_t *uspi, uspi__baudctrl_t baudctrl, uspi__mode_t mode);
  
static inline void
  uspi__write(uspi_t *uspi, const void *data, size_t data_len);

static inline void
  uspi__read(uspi_t *uspi, void *data, size_t data_len);

static inline void
  uspi__write_fast(uspi_t *uspi, uint8_t data);

static inline uint8_t
  uspi__read_fast(uspi_t *uspi, uint8_t data);
  
static inline void
  uspi__select(const uspi_t *uspi);

static inline void
  uspi__deselect(const uspi_t *uspi);

uint8_t
  uspi__select_auto(uspi_t *uspi);

void
  uspi__deselect_auto(uint8_t **uspi_p);
  
static inline bool_t
  uspi__is_selected(uspi_t *uspi);
  
  
/* Module Specific Functions */

void
  uspi__device__init(USART_t *device, uspi__baudctrl_t baudctrl, uspi__mode_t mode);
  
static inline void
  uspi__device__write(USART_t *device, const void *data, size_t data_len);
  
void
  uspi__device__read(USART_t *device, void *data, size_t data_len);
    
static inline void
  uspi__device__write_fast(USART_t *device, uint8_t data);
    
static inline uint8_t
  uspi__device__read_fast(USART_t *device, uint8_t data);



/* Macros ----------------------------------------+--------+----------------- */

#define uspi__transaction(uspi)                            \
  for (uint8_t                                             \
    *uspi_save CLEANUP(uspi__deselect_auto) = (uint8_t *) uspi, \
    _internal = uspi__select_auto(uspi);                   \
    _internal; _internal = 0)


/* Inline Function Definitions ---------------------------------------------- */

/* Init
 * See uspi__device__init.
 */
void
uspi__init(uspi_t *uspi, uspi__baudctrl_t baudctrl, uspi__mode_t mode)
{
  uspi__device__init(uspi->device, baudctrl, mode);
}

/* Write
 * See uspi__device__write.
 */
void
uspi__write(uspi_t *uspi, const void *data, size_t data_len)
{
  uspi__device__write(uspi->device, data, data_len);
}

/* Read
 * See uspi__device__write.
 */
void
uspi__read(uspi_t *uspi, void *data, size_t data_len)
{
  uspi__device__read(uspi->device, data, data_len);
}

/* Write Fast
 * See uspi__device__write_fast.
 */
void
uspi__write_fast(uspi_t *uspi, uint8_t data)
{
  uspi__device__write_fast(uspi->device, data);
}

/* Read Fast
 * See uspi__device__read_fast.
 */
uint8_t
uspi__read_fast(uspi_t *uspi, uint8_t data)
{
  return uspi__device__read_fast(uspi->device, data);
}

/* Select
 * Select the SPI interface by asserting the CS GPIO pin.
 */
void
uspi__select(const uspi_t *uspi)
{
  assert(uspi != NULL);
  assert(uspi->cs_port != NULL);

  uspi->cs_port->OUTCLR = uspi->cs_bm;
}

/* Deslect
 * Deselect the SPI interface by deasserting the CS GPIO pin.
 */
void
uspi__deselect(const uspi_t *uspi)
{
  assert(uspi != NULL);
  assert(uspi->cs_port != NULL);

  /* Make sure we are done transmitting */
  usart__device__wait_tx_ready(uspi->device);

  uspi->cs_port->OUTSET = uspi->cs_bm;
}

/* Is Selected
 * Return non-zero if the SPI CS GPIO is selected.
 */
bool_t
uspi__is_selected(uspi_t *uspi)
{
  assert(uspi != NULL);
  assert(uspi->cs_port != NULL);

  return !(uspi->cs_port->OUT & uspi->cs_bm);
}


/* Module Specific Functions */


void
uspi__device__write_fast(USART_t *device, uint8_t data)
{
  usart__device__write_fast(device, data);
}

uint8_t
uspi__device__read_fast(USART_t *device, uint8_t data)
{
  usart__device__write_fast(device, data);
  usart__device__wait_tx_ready(device);
  
  return device->DATA;
}

void
uspi__device__write(USART_t *device, const void *data, size_t data_len)
{
  usart__device__write(device, data, data_len);
  //usart__wait_tx_ready(device);
}

#endif /* SPI_H */