#include <drivers/uspi.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
uspi__ctor(uspi_t *uspi, USART_t *device, PORT_t *cs_port, uint8_t cs_bm)
{
  WRITE_CONST(uspi->device,  USART_t *, device);
  
  /* Configure the CS pin as an output
     Make sure it is set high before we initialize it to
     avoid selecting the device */
  cs_port->OUTSET = cs_bm;
  gpio__ctor(&uspi->cs, cs_port, cs_bm, GPIO__MODE_OUTPUT);
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
