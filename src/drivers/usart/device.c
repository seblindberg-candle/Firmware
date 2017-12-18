#include <drivers/usart/device.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

/* Init
 */
void
usart__device__init(USART_t *device, usart__device__baudctrl_t baudctrl,
                    usart__device__config_t conf)
{
  usart__device__enable_power(device);
  
  device->CTRLB = conf;

  device->CTRLC = USART_CMODE_ASYNCHRONOUS_gc
                | USART_PMODE_DISABLED_gc
                | USART_CHSIZE_8BIT_gc;

  device->BAUDCTRLA = (uint8_t) baudctrl;
  device->BAUDCTRLB = (baudctrl >> 8);
}

void
usart__device__map_gpio(USART_t *device, usart__device__remap_t remap)
{
  PORT_t* usart_port;
  
  usart_port = usart__device__port(device);

  if (remap == USART__DEVICE__NO_REMAP) {
    usart_port->DIRCLR = usart__device__is_read_enabled(device)  ?
      USART__DEVICE__PIN_RX_bm : 0;
    usart_port->DIRSET = usart__device__is_write_enabled(device) ?
      USART__DEVICE__PIN_TX_bm : 0;
  } else {
    usart_port->REMAP = PORT_USART0_bm;

    usart_port->DIRCLR = usart__device__is_read_enabled(device)  ?
      USART__DEVICE__REMAPPED_PIN_RX_bm : 0;
    usart_port->DIRSET = usart__device__is_write_enabled(device) ?
      USART__DEVICE__REMAPPED_PIN_TX_bm : 0;
  }
}

/* Write
 *
 * Write data_len bytes to the usart. Returns when all data has been written.
 */
void
usart__device__write(USART_t *device, const void *data, size_t data_len)
{
  if (data_len == 0) {
    return;
  }

  do {
    usart__device__write_fast(device, *((uint8_t *) data));
    ++data;
  } while (--data_len);
}

/* Read
 *
 * Read data_len bytes from the usart. Returns when all data has been read.
 */
void
usart__device__read(USART_t *device, void *data, size_t data_len)
{
  if (data_len == 0) {
    return;
  }

  do {
    *((uint8_t *) data) = usart__device__read_fast(device);
    ++data;
  } while (--data_len);
}
