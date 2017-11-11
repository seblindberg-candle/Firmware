#ifndef USPI_H
#define USPI_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <drivers/uspi/device.h>
#include <drivers/gpio.h>

/* Constants -------------------------------------+-------------------------- */



/* Data Types --------------------------------------------------------------- */

typedef struct {
  USART_t * const device;
  gpio_t          cs;
} uspi_t;

/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  uspi__ctor(uspi_t *uspi, USART_t *device, PORT_t *cs_port, uint8_t cs_bm)
  NONNULL;
  
static inline void
  uspi__write(uspi_t *uspi, const void *data, size_t data_len)
  NONNULL;

static inline void
  uspi__read(uspi_t *uspi, void *data, size_t data_len)
  NONNULL;

static inline void
  uspi__write_fast(uspi_t *uspi, uint8_t data)
  NONNULL;

static inline uint8_t
  uspi__read_fast(uspi_t *uspi, uint8_t data)
  NONNULL;
  
static inline void
  uspi__select(const uspi_t *uspi)
  NONNULL;

static inline void
  uspi__deselect(const uspi_t *uspi)
  NONNULL;

uint8_t
  uspi__select_auto(uspi_t *uspi)
  NONNULL;

void
  uspi__deselect_auto(uint8_t **uspi_p)
  NONNULL;
  
static inline bool_t
  uspi__is_selected(uspi_t *uspi)
  NONNULL;
  
  
/* Macros ----------------------------------------+--------+----------------- */

#define uspi__transaction(uspi)                            \
  for (uint8_t                                             \
    *uspi_save CLEANUP(uspi__deselect_auto) = (uint8_t *) uspi, \
    _internal = uspi__select_auto(uspi);                   \
    _internal; _internal = 0)


/* Inline Function Definitions ---------------------------------------------- */

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
 * See uspi__device__exchange.
 */
void
uspi__write_fast(uspi_t *uspi, uint8_t data)
{
  uspi__device__exchange(uspi->device, data);
}

/* Read Fast
 * See uspi__device__exchange.
 */
uint8_t
uspi__read_fast(uspi_t *uspi, uint8_t data)
{
  return uspi__device__exchange(uspi->device, data);
}

/* Select
 * Select the SPI interface by asserting the CS GPIO pin.
 */
void
uspi__select(const uspi_t *uspi)
{
  gpio__clr(&uspi->cs);
}

/* Deslect
 * Deselect the SPI interface by deasserting the CS GPIO pin.
 */
void
uspi__deselect(const uspi_t *uspi)
{
  gpio__set(&uspi->cs);
}

/* Is Selected
 * Return non-zero if the SPI CS GPIO is selected.
 */
bool_t
uspi__is_selected(uspi_t *uspi)
{
  return !gpio__value(&uspi->cs);
}


#endif /* USPI_H */