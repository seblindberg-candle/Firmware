#ifndef TERMINAL_H
#define TERMINAL_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <drivers/usart.h>
#include <stdio.h>

/* Constants -------------------------------------+-------------------------- */




/* Data Types --------------------------------------------------------------- */

typedef struct {
  USART_t *module;
  FILE     stream;
} terminal_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  terminal__ctor(terminal_t *terminal, USART_t *module);
                 
void
  terminal__attatch(terminal_t *terminal, FILE **in, FILE **out, FILE **err);

static inline uint8_t
  terminal__next(const terminal_t *terminal);

/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */

uint8_t
terminal__next(const terminal_t *terminal)
{
  return usart__read_fast(terminal->module);
}

#endif /* TERMINAL_H */