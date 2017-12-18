#include <drivers/clock.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

/* Macros ----------------------------------------+--------+----------------- */

#ifndef NDEBUG
# define CLOCK__INITIALIZED                       (0x6D)
#endif

#ifdef NDEBUG
# define clock__assert_initialized()
#else
# define clock__assert_initialized()                       \
  assert(clock.is_initialized == CLOCK__INITIALIZED)
#endif

/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

static bool_t
  alarm_insert_handler(const clock__alarm_t *alarm,
                       const clock__alarm_t *compare_to);

/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

clock_t clock;


/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

/* Init
 *
 * Initializes the global clock and starts the RTC device.
 */
void
clock__init()
{
  s_list__ctor(&clock.alarms);
  s_list__ctor(&clock.alarms_ovf);
  s_list__ctor(&clock.call_list);
  
  /* Start the clock device */
  clock__device__init(0, CLOCK__DEVICE_PRESCALER);
  
#ifndef NDEBUG
  clock.is_initialized = CLOCK__INITIALIZED;
#endif
}

void
clock__spin_once()
{
  clock__alarm_t *alarm;
  
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    alarm = (clock__alarm_t *) s_list__shift(&clock.call_list);
    
    if (alarm == NULL) {
      return;
    }
  }
  
  clock__alarm__call(alarm);
}

/*
 * Setting a timeout of 0 schedules the timeout exactly one period from now.
 */
void
clock__set_alarm(clock__alarm_t *alarm,
                 clock__timestamp_t timeout)
{
  clock__assert_initialized();
  
  clock__timestamp_t now = clock__now();
  
  alarm->timestamp = now + timeout;
  
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    if (alarm->timestamp > now) {
      /* If the alarm is set before the timer overflows */
      if (s_list__is_empty(&clock.alarms)) {
        /* If there are no other alarms we assume that the
           compare interrupt is disabled */
        s_list__ctor_with_list_item(&clock.alarms, &alarm->_super);
    
        clock__device__set_alarm(timeout);
        clock__device__enable_compare_interrupt(
          CLOCK__DEVICE_COMPARE_INTERRUPT_LVL);
      } else {
        s_list__insert_ordered(&clock.alarms, &alarm->_super,
                               (s_list__insert_handler_t) alarm_insert_handler);
        
        if ((clock__alarm_t *) s_list__first(&clock.alarms) == alarm) {
          clock__device__set_alarm(timeout);
        }
      }
    } else {
      if (s_list__is_empty(&clock.alarms_ovf)) {
        s_list__ctor_with_list_item(&clock.alarms_ovf, &alarm->_super);
    
        clock__device__enable_overflow_interrupt(
          CLOCK__DEVICE_OVERFLOW_INTERRUPT_LVL);
      } else {
        s_list__insert_ordered(&clock.alarms_ovf, &alarm->_super,
                               (s_list__insert_handler_t) alarm_insert_handler);
      }
    }
  }
}

void
clock__cancel_alarm(clock__alarm_t *alarm)
{
  /* Look through the lists to find the alarm. Most often it
     will be in the alarms list. */
  if (s_list__delete(&clock.alarms, &alarm->_super)) {
    return;
  }
  
  /* The second most likely place we can find it in should
     be the call list. */
  if (s_list__delete(&clock.call_list, &alarm->_super)) {
    return;
  }
  
  /* Last of all, look in the overflow list */
  s_list__delete(&clock.alarms_ovf, &alarm->_super);
}

/*
 * The insert function will "insert `item` before the first item in the list
 * for which `handler` returns non zero".
 */
bool_t
alarm_insert_handler(const clock__alarm_t *alarm,
                     const clock__alarm_t *compare_to)
{
  return alarm->timestamp < compare_to->timestamp;
}

