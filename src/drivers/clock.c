#include <drivers/clock.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

/* Macros ----------------------------------------+--------+----------------- */

#ifndef NDEBUG
# define CLOCK__INITIALIZED                       (0x6D)
#endif

#ifdef NDEBUG
# define clock__assert_initialized()              (void)
#else
# define clock__assert_initialized()                       \
  assert(clock.is_initialized == CLOCK__INITIALIZED)
#endif

/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

static bool_t
  alarm_insert_handler(const clock__alarm_t *alarm,
                       const clock__alarm_t *compare_to);

/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

static clock_t clock;


/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
clock__init()
{
  s_list__ctor(&clock.alarms);
  s_list__ctor(&clock.alarms_ovf);
  
  /* Start the clock device */
  clock__device__init(0, CLOCK__DEVICE_PRESCALER);
  
#ifndef NDEBUG
  clock.is_initialized = CLOCK__INITIALIZED;
#endif
}

/*
 * Setting a timeout of 0 schedules the timeout exactly one period from now.
 */
void
clock__set_alarm(clock__alarm_t *alarm,
                 clock__timestamp_t timeout)
{
  clock__assert_initialized();
  
  clock__timestamp_t now = clock__time();
  
  alarm->timestamp = now + timeout;
  
  if (alarm->timestamp > now) {
    if (s_list__is_empty(&clock.alarms)) {
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

ISR(RTC_OVF_vect)
{
  clock__alarm_t *first_alarm;
  
  clock__device__disable_overflow_interrupt();

  first_alarm = (clock__alarm_t *) s_list__first(&clock.alarms_ovf);

  if (first_alarm == NULL) {
    s_list__ctor(&clock.alarms);
    clock__device__disable_compare_interrupt();
    return;
  }

  /* Reset the lists to a known state */
  s_list__ctor_with_list_item(&clock.alarms, &first_alarm->_super);
  s_list__ctor(&clock.alarms_ovf);

  clock__device__set_alarm(first_alarm->timestamp);
  clock__device__enable_compare_interrupt(
    CLOCK__DEVICE_COMPARE_INTERRUPT_LVL);
}

ISR(RTC_COMP_vect)
{
  /* Assume that head is the next listener to be called */
  clock__alarm_t *alarm;
  const clock__timestamp_t now = clock__time();
  
  clock__assert_initialized();
  
  for (;;) {
    /* Look at the next alarm */
    alarm = (clock__alarm_t *) s_list__first(&clock.alarms);

    if (alarm == NULL) {
      clock__device__disable_compare_interrupt();
      break;
    }
    
    if (alarm->timestamp > now) {
      /* Schedule the next alarm */
      clock__device__set_alarm(alarm->timestamp - now);
      break;
    }

    s_list__shift(&clock.alarms);
    
    /* Should this run interruptable? */
    //ATOMIC_BLOCK(ATOMIC_FORCEON) {
      clock__alarm__call(alarm);
    //}
  }
}
