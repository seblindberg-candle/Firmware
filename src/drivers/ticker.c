#include <drivers/ticker.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

static bool_t
  insert_handler(s_list_item_t const *item_a,
                 s_list_item_t const *item_b);

static void
  schedule_alert(ticker_t *ticker);

/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
ticker__ctor(ticker_t *ticker)
{
  s_list__ctor(&ticker->listeners);
  
  ticker->period_tail = NULL;
}

void
ticker__add_listener(ticker_t *ticker, ticker__listener_t *listener,
                     ticker__timestamp_t timestamp)
{
  listener->timestamp = timestamp;
  const ticker__listener_t *compare_to;
  //s_list__insert_ordered(&ticker->_super, &listener->_super, insert_handler);
  
  if (timestamp <= current_tick) {
    if (s_list__is_empty(&ticker->listeners)) {
      assert(ticker->period_tail == NULL);
      s_list__add(&ticker->listeners, &listener->_super);
    } else {
      
    }
  } else {
    
  }

  schedule_alert(ticker);
}

bool_t
insert_handler(s_list_item_t const *item_a,
               s_list_item_t const *item_b)
{
  const ticker__listener_t *listener_a = (const ticker__listener_t *) item_a;
  const ticker__listener_t *listener_b = (const ticker__listener_t *) item_b;

  return listener_a->timestamp > listener_b->timestamp;
}

void
schedule_alert(ticker_t *ticker)
{
  if (!ticker__has_listeners(ticker)) {
    return;
  }
  
  const ticker__listener_t *first_listener = s_list__first(&ticker->listeners);
  // first_listener->timestamp;
}