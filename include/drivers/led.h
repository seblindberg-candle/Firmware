#ifndef LED_H
#define LED_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <drivers/gpio.h>


/* Constants -------------------------------------+-------------------------- */




/* Data Types --------------------------------------------------------------- */

typedef struct {
  gpio_t _super;
} led_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  led__ctor(led_t *led, PORT_t *port, uint8_t pin_bm);
  
static inline void
  led__on(const led_t *led)
  NONNULL;
  
static inline void
  led__toggle(const led_t *led)
  NONNULL;
  
static inline void
  led__off(const led_t *led)
  NONNULL;


/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */

void
led__on(const led_t *led)
{
  gpio__set(&led->_super);
}

void
led__toggle(const led_t *led)
{
  gpio__tgl(&led->_super);
}

void
led__off(const led_t *led)
{
  gpio__clr(&led->_super);
}

#endif /* LED_H */