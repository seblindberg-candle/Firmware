#ifndef DRIVERS_TICKER_H
#define DRIVERS_TICKER_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <list/s_list.h>
#include <drivers/ticker_listener.h>


/* Constants -------------------------------------+-------------------------- */




/* Data Types --------------------------------------------------------------- */

typedef struct {
  s_list_t listeners;
  ticker_listener_t *period_tail;
} ticker_t;

typedef uint16_t ticker__timestamp_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  ticker__ctor(ticker_t *ticker);
  NONNULL;

void
  ticker__add_listener(ticker_t *ticker, ticker_listener_t *listener,
                       ticker__timestamp_t timestamp)
  NONNULL;

static inline bool_t
  ticker__has_listeners(ticker_t * const ticker)
  NONNULL;
  
static inline void
  ticker__isr(ticker_t *ticker)
  NONNULL;

/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */

bool_t
ticker__has_listeners(ticker_t * const ticker)
{
  return !s_list__is_empty(&ticker->listeners);
}

void
ticker__isr(ticker_t *ticker)
{
  /* Assume that head is the next listener to be called */
  const ticker_listener_t *listener =
    (const ticker_listener_t *) s_list__shift(&ticker->listeners);
  ticker__timestamp_t current_time;
  
  if (listener != NULL) {
    ticker_listener__call(listener);
    
    /* Schedule the next ISR */
    listener = s_list__first(&ticker->listeners);
    
    if (listener != NULL) {
      /* Schedule at listener->timestamp */
      
      return;
    }
  }
  
  /* Disable the interrupt */
  
}

#endif /* DRIVERS_TICKER_H */