#include <drivers/button.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

static void
  debounce_callback(void *ctx);


/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

/* Constructor
 * 
 * `mode` can be either one of `BUTTON__MODE_NO` and `BUTTON__MODE_NC`.
 */
void
button__ctor(button_t *button, PORT_t *port, uint8_t pin_bm,
             button__mode_t mode)
{
  gpio__ctor(&button->_super, port, pin_bm, GPIO__MODE_INPUT);
  
  PORTCFG.MPCMASK   = pin_bm;
  if (mode & 0x01) {
    /* Normally closed button, shorting to VCC */
    port->PIN0CTRL = PORT_OPC_PULLDOWN_gc;
  } else {
    /* Normally open button, shorting to ground */
    port->PIN0CTRL = PORT_INVEN_bm | PORT_OPC_PULLUP_gc;
  }
}

/* Register Callback
 *
 * Stores a function handle to the handler that will be called once the button
 * state changes. To detect this the port interrupts are enabled for the button
 * pin(s). Note that `button__isr` must be called within the approriate
 * interrupt vector, like so:
 *
     ISR(PORTx_INT_vect)
     {
       button__isr(button);
     }
 *
 * The reason for using interrupts instead of pure polling is mainly to allow
 * wakeup from sleep.
 */
void
button__register_callback(button_t *button, button__callback_t callback)
{
  button->callback   = callback;
  button->last_state = button__is_down(button);
  
  clock__alarm__ctor(&button->alarm,
                     debounce_callback, button);
  
  gpio__clear_interrupt_flags(&button->_super);
  gpio__enable_interrupts(&button->_super);
  
  /* Enable low level interrupts if they are not already on */
  if (button->_super.port->INTCTRL == 0) {
    button->_super.port->INTCTRL = PORT_INTLVL_LO_gc;
    
    /* The button driver requires low priority interrupts to
       be enabled */
    PMIC.CTRL |= PMIC_LOLVLEN_bm;
  }
}

/* Debounce Callback
 *
 * Called when the debounce timer alarm expires. If the current button state
 * differs from what was seen before the interrupt occured the registerd button
 * handler is called.
 */
void
debounce_callback(void *ctx)
{
  button_t *button = (button_t *) ctx;
  uint8_t state    = gpio__value(&button->_super);
  
  /* Check the button state and compare it with the previous
     state */
  if (state != button->last_state) {
    button->callback(button);
    button->last_state = state;
  }
  
  gpio__clear_interrupt_flags(&button->_super);
  gpio__enable_interrupts(&button->_super);
}