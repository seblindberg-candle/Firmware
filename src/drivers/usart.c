#include <drivers/usart.h>
#include <drivers/usart/device.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

/* Init
 */
void
usart__ctor(usart_t *usart, USART_t *device, void *buffer, size_t buffer_len)
{
  const bool_t r = usart__device__is_read_enabled(device);
  const bool_t w = usart__device__is_write_enabled(device);
  void *buffer_w = buffer;
  
  assert(usart  != NULL);
  assert(device != NULL);
  assert(buffer != NULL);
  /* TODO: Check that buffer len is a power of 2 <= 256 and >= 8 */
  
  WRITE_CONST(usart->device, USART_t *, device);
  
  if (r && w) {
    buffer_len /= 2;
    buffer_w += buffer_len;
  }
  
  if (r) {
    fifo__ctor(&usart->r_fifo, buffer, buffer_len);
    
    /* Enable rx interrupts */
    device->CTRLA |= USART_RXCINTLVL_LO_gc;
  } else {
    fifo__ctor(&usart->r_fifo, NULL, 0);
  }
  
  if (w) {
    fifo__ctor(&usart->w_fifo, buffer_w, buffer_len);
  } else {
    fifo__ctor(&usart->w_fifo, NULL, 0);
  }
}

/* Write
 *
 * Writes bytes to the usart tx buffer. The function blocks until all bytes have
 * been written.
 */
void
usart__write(usart_t *usart, const void *src, size_t src_len)
{
  const uint8_t *data = (const uint8_t *) src;
  size_t         res;
  
  if (src_len == 0) {
    return;
  }
  
  assert(usart != NULL);
  assert(src != NULL);
  
  if (fifo__is_empty(&usart->w_fifo)) {
    usart__device__write_fast(usart->device, *data);
    
    /* Only enable interrupts if there are more than one
       byte to write */
    if (--src_len == 0) {
      return;
    } else {
      usart->device->CTRLA |= USART_DREINTLVL_LO_gc;
    }
  }
  
  /* At this point gloabl interrupts must be enabled, or we
     may get stuck in the loop */
  assert(GLOBAL_INTERRUPTS_ENABLED);
  
  /* Loop until all bytes have been written */
  for (;;) {
    res = fifo__write(&usart->w_fifo, data, src_len);
    src_len -= res;
    
    if (src_len == 0) {
      break;
    } else {
      data += res;
    }
  }
}

void
usart__write_fast(usart_t *usart, uint8_t data)
{
  size_t res;
  assert(usart != NULL);
  
  if (fifo__is_empty(&usart->w_fifo)) {
    usart__device__write_fast(usart->device, data);
  } else {
    /* At this point gloabl interrupts must be enabled, or we
       may get stuck in the loop */
    assert(GLOBAL_INTERRUPTS_ENABLED);
    
    do {
      res = fifo__write(&usart->w_fifo, &data, 1);
    } while (res == 0);
  }
}

void
usart__read(usart_t *usart, void *dest, size_t dest_len)
{
  uint8_t *data = (uint8_t *) dest;
  size_t   res;
  
  if (dest_len == 0) {
    return;
  }
  
  assert(usart != NULL);
  assert(dest != NULL);
  
  /* At this point gloabl interrupts must be enabled, or we
     may get stuck in the loop */
  assert(GLOBAL_INTERRUPTS_ENABLED);
  
  /* Loop until all bytes have been read */
  for (;;) {
    res = fifo__read(&usart->w_fifo, data, dest_len);
    dest_len -= res;
    
    if (dest_len == 0) {
      break;
    } else {
      data += res;
    }
  }
}

uint8_t
usart__read_fast(usart_t *usart)
{
  uint8_t data;
  size_t  res;
  assert(usart != NULL);
  
  /* At this point gloabl interrupts must be enabled, or we
     may get stuck in the loop */
  assert(GLOBAL_INTERRUPTS_ENABLED);
  
  do {
    res = fifo__read(&usart->w_fifo, &data, 1);
  } while (res == 0);
  
  return data;
}
