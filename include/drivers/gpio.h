#ifndef GPIO_H
#define GPIO_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>


/* Constants -------------------------------------+-------------------------- */

#define GPIO__MODE_INPUT_bm                        (0 << 3)
#define GPIO__MODE_OUTPUT_bm                       (1 << 3)
#define GPIO__MODE_INVERT_bm                       (1 << 4)


/* Data Types --------------------------------------------------------------- */

typedef struct {
  PORT_t * const port;
  const uint8_t  pin_bm;
} gpio_t;

typedef enum {
  GPIO__MODE_INPUT  = GPIO__MODE_INPUT_bm,
  GPIO__MODE_INPUT_INVERT = (GPIO__MODE_INPUT_bm | GPIO__MODE_INVERT_bm),
  GPIO__MODE_OUTPUT = GPIO__MODE_OUTPUT_bm,
  GPIO__MODE_OUTPUT_INVERT = (GPIO__MODE_OUTPUT_bm | GPIO__MODE_INVERT_bm),
} gpio__mode_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  gpio__ctor(gpio_t *gpio, PORT_t *port, uint8_t pin_bm, gpio__mode_t mode)
  NONNULL;

static inline void
  gpio__set(const gpio_t *gpio)
  NONNULL;
  
static inline void
  gpio__clr(const gpio_t *gpio)
  NONNULL;
  
static inline void
  gpio__tgl(const gpio_t *gpio)
  NONNULL;
  
static inline uint8_t
  gpio__value(const gpio_t *gpio)
  NONNULL;
    
static inline void
  gpio__enable_interrupts(gpio_t *gpio)
  NONNULL;
  
static inline void
  gpio__disable_interrupts(gpio_t *gpio)
  NONNULL;
  
static inline void
  gpio__clear_interrupt_flags(gpio_t *gpio)
  NONNULL;
  
static inline uint8_t
  gpio__get_interrupt_flags(const gpio_t *gpio)
  NONNULL;

/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */

void
gpio__set(const gpio_t *gpio)
{
  gpio->port->OUTSET = gpio->pin_bm;
}

void
gpio__clr(const gpio_t *gpio)
{
  gpio->port->OUTCLR = gpio->pin_bm;
}

void
gpio__tgl(const gpio_t *gpio)
{
  gpio->port->OUTTGL = gpio->pin_bm;
}

uint8_t
gpio__value(const gpio_t *gpio)
{
  return gpio->port->IN & gpio->pin_bm;
}

void
gpio__enable_interrupts(gpio_t *gpio)
{
  gpio->port->INTMASK |= gpio->pin_bm;
}

void
gpio__disable_interrupts(gpio_t *gpio)
{
  gpio->port->INTMASK &= ~gpio->pin_bm;
}

uint8_t
gpio__get_interrupt_flags(const gpio_t *gpio)
{
  return gpio->port->INTFLAGS & gpio->pin_bm;
}

void
gpio__clear_interrupt_flags(gpio_t *gpio)
{
  gpio->port->INTFLAGS = gpio->pin_bm;
}

#endif /* GPIO_H */