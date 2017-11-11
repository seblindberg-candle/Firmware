#ifndef BUTTON_H
#define BUTTON_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <drivers/gpio.h>


/* Constants -------------------------------------+-------------------------- */

#define BUTTON__MODE_NC_bm
#define BUTTON__MODE_NO_bm


/* Data Types --------------------------------------------------------------- */

struct button_t; /* Forward reference */
typedef void (*button__callback_t)(struct button_t *button);

typedef struct {
  gpio_t             gpio;
  volatile uint8_t   last_state;
  volatile uint16_t  last_updated;
  button__callback_t callback;
} button_t;

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
  return gpio__value(&button->gpio);
}

/* ISR
 *
 * Call within the ISR(PORTx_vect) { ... }
 */
void
button__isr(button_t *button)
{
  uint8_t changes;
  
  /* Only handle interrupts triggered by us */
  if ((button->gpio.port->INTFLAGS & button->gpio.pin_bm) == 0) {
  //  gpio__is_interrupt_triggered(&button->gpio)
    return;
  }
  
  button->gpio.port->INTFLAGS = button->gpio.pin_bm;
  // gpio__clear_interrupt(&button->gpio);
  
  changes = button__state(button) ^ button->last_state;
  
  assert(button->callback != NULL);
  button->callback(button);
  
  //button->last_updated = now;
}
  

#endif /* BUTTON_H */