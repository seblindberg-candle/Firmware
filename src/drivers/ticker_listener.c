#include <drivers/ticker_listener.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
ticker_listener__ctor(ticker_listener_t *listener,
                      ticker__callback_t callback,
                      void *ctx)
{
  s_list_item__ctor(&listener->_super);
  listener->callback = callback;
  listener->ctx      = ctx;
}

