#ifndef TERMINAL_H
#define TERMINAL_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <drivers/usart.h>


/* Constants -------------------------------------+-------------------------- */

#define TERMINAL__BUFFER_SIZE                     (64)

#ifdef fdev_setup_stream
# ifndef TERMINAL__SUPPORT_STREAM
#  define TERMINAL__SUPPORT_STREAM                (1)
# endif
#endif

#if !TERMINAL__SUPPORT_STREAM
# warning Compiling without stream support
#endif

/* Data Types --------------------------------------------------------------- */

typedef struct {
  usart_t usart;
#if TERMINAL__SUPPORT_STREAM
  FILE    stream;
#endif
  uint8_t buffer[TERMINAL__BUFFER_SIZE];
} terminal_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  terminal__ctor(terminal_t *terminal, USART_t *module)
  NONNULL;

#if TERMINAL__SUPPORT_STREAM
void
  terminal__attatch(terminal_t *terminal, FILE **in, FILE **out, FILE **err)
  NONNULL_ARGS(1);
#else
# define terminal__attatch(...)                  (void)
#endif

static inline void
  terminal__rxc_isr(terminal_t *terminal)
  NONNULL;
  
static inline void
  terminal__dre_isr(terminal_t *terminal)
  NONNULL;

void
  terminal__puts(terminal_t *terminal, const char *str)
  NONNULL;

static inline uint8_t
  terminal__next(terminal_t *terminal)
  NONNULL;

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