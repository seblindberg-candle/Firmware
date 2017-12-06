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
  volatile uint16_t  last_updated;
  button__callback_t callback;
  clock_t           *clock;
  clock__alarm_t     alarm;
};

typedef enum {
  BUTTON__MODE_NO = 0,
  BUTTON__MODE_NC = 1,
} button__mode_t;

/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  button__ctor(button_t *button, PORT_t *port, uint8_t pin_bm);
  
void
  button__register_callback(button_t *button, button__callback_t callback);
  
static inline uint8_t
  button__state(const button_t *button);
  
static inline void
  button__isr(button_t *button);


/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */

uint8_t
button__state(const button_t *button)
{
  return gpio__value(&button->_super);
}

/* ISR
 *
 * Call within the ISR(PORTx_vect) { ... }
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
  
  /* Set a timeout to debounce the button. We don't yet know
     if it was just noise. */
  clock__set_alarm(button->clock, &button->alarm,
                   BUTTON__DEBOUNCE_TIMEOUT);
  
  //button->last_updated = now;
}
  

#endif /* BUTTON_H */