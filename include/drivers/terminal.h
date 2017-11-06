#ifndef TERMINAL_H
#define TERMINAL_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <drivers/usart.h>
#include <stdio.h>

/* Constants -------------------------------------+-------------------------- */

#define TERMINAL__BUFFER_SIZE                     (64)


/* Data Types --------------------------------------------------------------- */

typedef struct {
  usart_t usart;
  FILE    stream;
  uint8_t buffer[TERMINAL__BUFFER_SIZE];
} terminal_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  terminal__ctor(terminal_t *terminal, USART_t *module);
                 
void
  terminal__attatch(terminal_t *terminal, FILE **in, FILE **out, FILE **err);

static inline void
  terminal__rxc_isr(terminal_t *terminal);
  
static inline void
  terminal__dre_isr(terminal_t *terminal);

static inline uint8_t
  terminal__next(terminal_t *terminal);

/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */

void
terminal__rxc_isr(terminal_t *terminal)
{
  usart__rxc_isr(&terminal->usart);
}

void
terminal__dre_isr(terminal_t *terminal)
{
  usart__dre_isr(&terminal->usart);
}

uint8_t
terminal__next(terminal_t *terminal)
{
  return usart__read_fast(&terminal->usart);
}

#endif /* TERMINAL_H */