#include <drivers/clock.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

static void
  schedule_alert(clock_t *clock);

/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
clock__ctor(clock_t *clock)
{
  s_list__ctor(&clock->alarms);
  
  clock->period_tail = NULL;
}

void
clock__set_alarm(clock_t *clock, clock__alarm_t *alarm,
                 clock__timestamp_t timestamp)
{
  alarm->timestamp = timestamp;
  uint16_t current_tick;
  const clock__alarm_t *compare_to;
  //s_list__insert_ordered(&clock->_super, &listener->_super, insert_handler);
  
  if (timestamp <= current_tick) {
    if (s_list__is_empty(&clock->alarms)) {
      assert(clock->period_tail == NULL);
      s_list__unshift(&clock->alarms, &alarm->_super);
    } else {
      
    }
  } else {
    
  }

  schedule_alert(clock);
}

void
schedule_alert(clock_t *clock)
{
  if (!clock__has_alarms(clock)) {
    return;
  }
  
  const clock__alarm_t *first_alarm =
   (const clock__alarm_t *) s_list__first(&clock->alarms);
   
  // first_listener->timestamp;
}