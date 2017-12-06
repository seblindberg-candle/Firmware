#include <drivers/clock/alarm.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
clock__alarm__ctor(clock__alarm_t *alarm,
                   clock__alarm__callback_t callback,
                   void *ctx)
{
  s_list_item__ctor(&alarm->_super);
  alarm->callback = callback;
  alarm->ctx      = ctx;
}

