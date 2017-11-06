#include <drivers/uspi.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
uspi__ctor(uspi_t *uspi, USART_t *device, PORT_t *cs_port, uint8_t cs_bm)
{
  uspi->device  = device;
  uspi->cs_port = cs_port;
  uspi->cs_bm   = cs_bm;
}

void
uspi__device__init(USART_t *device, uspi__baudctrl_t baudctrl, uspi__mode_t mode)
{
  PORT_t *port;
  
  device->CTRLB = USART_TXEN_bm
                | USART_RXEN_bm;
  
  /* Enable SPI mode */
  device->CTRLC = USART_CMODE_MSPI_gc
                | (mode & 0x01 ? USART_UCPHA_bm : 0)
                | (mode & 0x04 ? USART_UDORD_bm : 0);
  
  device->BAUDCTRLA = (uint8_t) baudctrl;
  device->BAUDCTRLB = (baudctrl >> 8);
  
  /* Configure GPIO */
  if (device == &USARTC0) {
    port = &PORTC;
  } else {
    assert(device == &USARTD0);
    port = &PORTD;
  }
  
  port->DIRCLR = PIN2_bm;
  port->DIRSET = PIN3_bm | PIN1_bm;
  /* Enable pull-up on the MISO pin (for devices with high-z output) */
  port->PIN2CTRL = PORT_OPC_PULLUP_gc;
  
  /* Invert clock polarity */
  if (mode & 0x02) {
    /* TODO */
    //port->PIN1CTRL = PORT_INVEN_bm;
  }
}

void
uspi__device__read(USART_t *device, void *data, size_t data_len)
{
  assert(device != NULL);
  assert(data != NULL);
  
  if (data_len == 0) {
    return;
  }
  
  /* Start reading the first byte */
  usart__device__write_fast(device, 0xFF);
  
  do {
    if (--data_len == 0) {
      break;
    }
    
    /* Initiate the next write */
    usart__device__write_fast(device, 0xFF);
    
    /* Read the previous byte */
    usart__device__wait_tx_ready(device);
    *((uint8_t *) data) = device->DATA;
    data ++;
  } while (1);
  
  /* Read the last byte */
  usart__device__wait_tx_ready(device);
  *((uint8_t *) data) = device->DATA;
}

uint8_t
uspi__select_auto(uspi_t *uspi)
{
  uspi__select(uspi);
  return 1;
}

void
uspi__deselect_auto(uint8_t **uspi_p)
{
  uspi_t *uspi = (uspi_t *) *uspi_p;
  uspi__deselect(uspi);
}
