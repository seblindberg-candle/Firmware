#ifndef BUTTON_H
#define BUTTON_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <drivers/gpio.h>
#include <drivers/clock.h>
#include <drivers/clock/alarm.h>


/* Constants -------------------------------------+-------------------------- */

#define BUTTON__MODE_NC_bm
#define BUTTON__MODE_NO_bm

#define BUTTON__DEBOUNCE_TIMEOUT                  (10)


/* Data Types --------------------------------------------------------------- */

typedef struct button_t button_t; /* Forward reference */

typedef void (*button__callback_t)(struct button_t *button);

struct button_t {
  gpio_t            _super;
  volatile uint8_t   last_state;
  button__callback_t callback;
  clock__alarm_t     alarm;
};

typedef enum {
  BUTTON__MODE_NO = 0,
  BUTTON__MODE_NC = 1,
} button__mode_t;

/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

/* Constructor
 *
 * `mode` can be either one of `BUTTON__MODE_NO` and `BUTTON__MODE_NC`.
 */
void
  button__ctor(button_t *button, PORT_t *port, uint8_t pin_bm,
               button__mode_t mode);
  
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
  button__register_callback(button_t *button, button__callback_t callback);
  
/* Is Down
 *
 * Returns non-zero if the button is in its active, pressed state. When multiple
 * pins are selected the bits of the output corresponds to those pins.
 */
static inline uint8_t
  button__is_down(const button_t *button);
  
/* ISR
 *
 * Call within ISR(PORTx_INT_vect) { ... }
 */
static inline void
  button__isr(button_t *button);


/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */

/* Is Down
 *
 * Returns non-zero if the button is in its active, pressed state. When multiple
 * pins are selected the bits of the output corresponds to those pins.
 */
uint8_t
button__is_down(const button_t *button)
{
  return gpio__value(&button->_super);
}

/* ISR
 *
 * Call within ISR(PORTx_INT_vect) { ... }
 */
void
button__isr(button_t *button)
{
  /* Only handle interrupts triggered by us */
  if (gpio__get_interrupt_flags(&button->_super) == 0) {
    return;
  }
  
  /* Disable the isr and clear the interrupt */
  gpio__disable_interrupts(&button->_super);
  gpio__clear_interrupt_flags(&button->_super);
  
  /* Set a timeout to debounce the button. We don't yet know
     if it was just noise. */
  clock__set_alarm(&button->alarm,
                   BUTTON__DEBOUNCE_TIMEOUT);
}
  

#endif /* BUTTON_H */