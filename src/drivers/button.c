#include <drivers/button.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
button__ctor(button_t *button, PORT_t *port, uint8_t pin_bm)
{
  gpio__ctor(&button->gpio, port, pin_bm, GPIO__MODE_INPUT);
  
  button->callback = NULL;
}

void
button__register_callback(button_t *button, button__callback_t callback)
{
  button->callback = callback;
  
  /* Clear interrupt flags before enabling interrupts */
  button->gpio.port->INTFLAGS  = button->gpio.pin_bm;
  button->gpio.port->INTMASK  |= button->gpio.pin_bm;
  
  if (button->gpio.port->INTCTRL == 0) {
    button->gpio.port->INTCTRL = PORT_INTLVL_LO_gc;
    
    /* The button driver requires low priority interrupts to
       be enabled */
    PMIC.CTRL |= PMIC_LOLVLEN_bm;
  }
}