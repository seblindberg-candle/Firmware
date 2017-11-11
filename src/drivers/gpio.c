#include <drivers/gpio.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

/* Constructor
 *
 * Can be used to batch set multiple pins.
 */
void
gpio__ctor(gpio_t *gpio, PORT_t *port, uint8_t pin_bm, gpio__mode_t mode)
{
  assert(pin_bm != 0);
  
  WRITE_CONST(gpio->port,   PORT_t*, port);
  WRITE_CONST(gpio->pin_bm, uint8_t, pin_bm);
  
  if (mode & GPIO__MODE_OUTPUT_bm) {
    port->DIRSET = pin_bm;
  } else {
    port->DIRCLR = pin_bm;
  }
  
  if (mode & GPIO__MODE_INVERT_bm) {
    /* From the documentation:
         When one or more bits in the MPCMASK register is
         set, writing any of the PINnCTRL registers will
         update only the PINnCTRL registers matching the
         mask in the MPCMASK register for that port. The
         MPCMASK register is automatically cleared after
         any PINnCTRL register is written.
     */
    PORTCFG.MPCMASK = pin_bm;
    port->PIN0CTRL  = PORT_INVEN_bm;
  }
}

