#include <drivers/uspi.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
uspi__init(USART_t *module, uspi__baudctrl_t baudctrl, uspi__mode_t mode)
{
  module->CTRLB = USART_TXEN_bm
                | USART_RXEN_bm;
  
  /* Enable SPI mode */
  module->CTRLC = USART_CMODE_MSPI_gc
                | (mode & 0x01 ? USART_UCPHA_bm : 0)
                | (mode & 0x04 ? USART_UDORD_bm : 0);
                
  /* Invert clock polarity */
  if (mode & 0x02) {
    /* TODO */
  }
  
  module->BAUDCTRLA = (uint8_t) baudctrl;
  module->BAUDCTRLB = (baudctrl >> 8);
}

void
uspi__read(USART_t *module, void *data, size_t data_len)
{
  assert(module != NULL);
  assert(data != NULL);
  
  if (data_len == 0) {
    return;
  }
  
  do {
    usart__write_fast(module, *((uint8_t *) data));
    usart__wait_tx_ready(module);
    *((uint8_t *) data) = module->DATA;
    data ++;
  } while (--data_len);
}