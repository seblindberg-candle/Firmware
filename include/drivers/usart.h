#ifndef USART_H
#define USART_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <fifo.h>
#include <drivers/usart/device.h>

/* Constants -------------------------------------+-------------------------- */



/* Data Types --------------------------------------------------------------- */

typedef struct {
  USART_t * const device;
  fifo_t  w_fifo;
  fifo_t  r_fifo;
} usart_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  usart__ctor(usart_t *usart, USART_t *device, void *buffer, size_t buffer_len)
  NONNULL;
  
static inline void
  usart__rxc_isr(usart_t *usart)
  NONNULL;
  
static inline void
  usart__dre_isr(usart_t *usart)
  NONNULL;

void
  usart__write(usart_t *usart, const void *src, size_t data_len)
  NONNULL;
  
void
  usart__write_fast(usart_t *usart, uint8_t data)
  NONNULL;
  
void
  usart__read(usart_t *usart, void *dest, size_t data_len)
  NONNULL;
  
uint8_t
  usart__read_fast(usart_t *usart)
  NONNULL;
  
static inline bool_t
  usart__is_empty(usart_t *usart)
  NONNULL;
  
static inline bool_t
  usart__is_read_enabled(const usart_t *usart)
  NONNULL;

static inline bool_t
  usart__is_write_enabled(const usart_t *usart)
  NONNULL;

/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */

bool_t
usart__is_empty(usart_t *usart)
{
  return fifo__is_empty(&usart->r_fifo);
}

bool_t
usart__is_read_enabled(const usart_t *usart)
{
  return usart__device__is_read_enabled(usart->device);
}

bool_t
usart__is_write_enabled(const usart_t *usart)
{
  return usart__device__is_write_enabled(usart->device);
}

/* Receive Interrupt Service Routine
 *
 */
void
usart__rxc_isr(usart_t *usart)
{
  uint8_t data;
  size_t  res;

  assert(usart__is_read_enabled(usart));
  assert(usart->device->STATUS & USART_RXCIF_bm);

  /* Reading from the DATA register clears the interrupt */
  data = usart->device->DATA;

  /* TODO: replace with fifo__write_force */
  if (fifo__is_full(&usart->r_fifo)) {
    uint8_t tmp;
    fifo__read(&usart->r_fifo, &tmp, 1);
  }

  /* Insert the received byte into the buffer */
  res = fifo__write(&usart->r_fifo, &data, 1);
  assert(res == 1);
}

/* Data Ready Interrupt Service Routine
 *
 * From the manual:
 *   When interrupt-driven data transmission is used, the Data Register Empty
 *   interrupt routine must either write new data to DATA in order to clear
 *   DREIF or disable the Data Register Empty interrupt. If not, a new interrupt
 *   will occur directly after the return from the current interrupt.
 */
void
usart__dre_isr(usart_t *usart)
{
  assert(usart__is_write_enabled(usart));
  assert(usart->device->STATUS & USART_DREIF_bm);
  
  if (fifo__is_empty(&usart->w_fifo)) {
    /* Disable the interrupt */
    usart->device->CTRLA &= ~USART_DREINTLVL_HI_gc;
  } else {
    size_t  res;
    uint8_t data;

    res = fifo__read(&usart->w_fifo, &data, 1);
    assert(res == 1);
    
    usart->device->DATA = data;
  }
}


#endif /* USART_H */