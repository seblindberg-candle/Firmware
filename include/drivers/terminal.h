#ifndef TERMINAL_H
#define TERMINAL_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <drivers/usart.h>
#include <drivers/terminal/cmd.h>
#include <list/s_list.h>


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
  usart_t  usart;
  
  /* Command parser */
  s_list_t         cmd_list;
  terminal__cmd_t *cmd_active;
  
#if TERMINAL__SUPPORT_STREAM
  FILE     stream;
#endif

  /* Data buffer for incomming and outgoing data. If both
     reading and writing are enabled the buffer will be
     split in half. */
  uint8_t  buffer[TERMINAL__BUFFER_SIZE];
} terminal_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  terminal__ctor(terminal_t *terminal, USART_t *device)
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

static inline bool_t
  terminal__next(terminal_t *terminal, uint8_t *next_byte)
  NONNULL;
  
static inline void
  terminal__add_cmd(terminal_t *terminal, terminal__cmd_t *cmd)
  NONNULL;
  
void
  terminal__spin_once(terminal_t *terminal)
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

bool_t
terminal__next(terminal_t *terminal, uint8_t *next_byte)
{
  if (usart__is_empty(&terminal->usart)) {
    return false;
  }
  
  *next_byte = usart__read_fast(&terminal->usart);
  return true;
}

void
terminal__add_cmd(terminal_t *terminal, terminal__cmd_t *cmd)
{
  s_list__unshift(&terminal->cmd_list, &cmd->_super);
}

#endif /* TERMINAL_H */