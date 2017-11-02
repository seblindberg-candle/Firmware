#include <drivers/usart.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

/* Init
 */
void
usart__init(USART_t *module, usart__baudctrl_t baudctrl,
            usart__config_t conf)
{
  module->CTRLB = conf;
                
  module->CTRLC = USART_CMODE_ASYNCHRONOUS_gc
                | USART_PMODE_DISABLED_gc
                | USART_CHSIZE_8BIT_gc;

  module->BAUDCTRLA = (uint8_t) baudctrl;
  module->BAUDCTRLB = (baudctrl >> 8);
}

void
usart__map_io(USART_t *module, usart__remap_t remap)
{
  PORT_t* usart_port;
    
  assert(module != NULL);
  
  if (module == &USARTC0) {
    usart_port = &PORTC;
  } else {
    assert(module == &USARTD0);
    usart_port = &PORTD;
  }
  
  if (remap == USART__NO_REMAP) {
    usart_port->DIRCLR = usart__is_read_enabled(module)  ? PIN2_bm : 0;
    usart_port->DIRSET = usart__is_write_enabled(module) ? PIN3_bm : 0;
  } else {
    usart_port->REMAP = PORT_USART0_bm;
    
    usart_port->DIRCLR = usart__is_read_enabled(module)  ? PIN6_bm : 0;
    usart_port->DIRSET = usart__is_write_enabled(module) ? PIN7_bm : 0;
  }
}

/* Write
 *
 * Write data_len bytes to the usart. Returns when all data has been written.
 */
void
usart__write(USART_t *module, const void *data, size_t data_len)
{
  assert(module != NULL);
  assert(data != NULL);
  
  if (data_len == 0) {
    return;
  }
  
  do {
    usart__write_fast(module, *((uint8_t *) data));
    ++data;
  } while (--data_len);
}

/* Read
 *
 * Read data_len bytes from the usart. Returns when all data has been read.
 */
void
usart__read(USART_t *module, void *data, size_t data_len)
{
  if (data_len == 0) {
    return;
  }

  do {
    *((uint8_t *) data) = usart__read_fast(module);
    ++data;
  } while (--data_len);
}