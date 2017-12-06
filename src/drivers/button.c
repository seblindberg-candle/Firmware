#include <drivers/button.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

static void
  debounce_callback(void *ctx);


/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
button__ctor(button_t *button, PORT_t *port, uint8_t pin_bm)
{
  gpio__ctor(&button->gpio, port, pin_bm, GPIO__MODE_INPUT);
  
  // button->callback = NULL;
}

void
button__register_callback(button_t *button, button__callback_t callback)
{
  button->callback = callback;
  
  ticker_listener__ctor(&button->ticker_listener,
                        debounce_callback, button);
  
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

void
debounce_callback(void *ctx)
{
  button_t *button = (button_t *) ctx;
  uint8_t state    = gpio__value(&button->gpio);
  
  /* Check the button state and compare it with the previous
     state */
  if (state != button->last_state) {
    button->callback(button);
    button->last_state = state;
  }
  
  /* Clear any interrupts and enable the ISR again */
  button->gpio.port->INTFLAGS  = button->gpio.pin_bm;
  button->gpio.port->INTMASK  |= button->gpio.pin_bm;
}