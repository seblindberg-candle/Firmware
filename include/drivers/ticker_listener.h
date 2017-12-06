#ifndef DRIVERS_TICKER_LISTENER_H
#define DRIVERS_TICKER_LISTENER_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <list/s_list_item.h>


/* Constants -------------------------------------+-------------------------- */




/* Data Types --------------------------------------------------------------- */

typedef uint16_t ticker__timestamp_t;
typedef void  (*ticker__callback_t)(void *ctx);

typedef struct {
  s_list_item_t       _super;
  ticker__timestamp_t  timestamp;
  ticker__callback_t   callback;
  void                *ctx;
} ticker_listener_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  ticker_listener__ctor(ticker_listener_t *listener,
                        ticker__callback_t callback,
                        void *ctx);

static inline void
  ticker_listener__call(const ticker_listener_t *listener);

/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */

void
ticker_listener__call(const ticker_listener_t *listener)
{
  listener->callback(listener->ctx);
}

#endif /* DRIVERS_TICKER_LISTENER_H */