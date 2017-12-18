#ifndef DRIVERS_CLOCK_H
#define DRIVERS_CLOCK_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <list/s_list.h>
#include <drivers/clock/device.h>
#include <drivers/clock/alarm.h>


/* Constants -------------------------------------+-------------------------- */

#define CLOCK__DEVICE_PRESCALER                   RTC_PRESCALER_DIV1_gc
#define CLOCK__DEVICE_OVERFLOW_INTERRUPT_LVL      RTC_OVFINTLVL_LO_gc
#define CLOCK__DEVICE_COMPARE_INTERRUPT_LVL       RTC_COMPINTLVL_LO_gc


/* Data Types --------------------------------------------------------------- */

typedef struct {
  s_list_t         alarms;
  s_list_t         alarms_ovf;
  
  s_list_t         call_list;
  
#ifndef NDEBUG
  bool_t           is_initialized;
#endif
} clock_t;

typedef clock__device__timestamp_t clock__timestamp_t;


/* Global Variables --------------------------------------------------------- */

extern clock_t clock;


/* Public Functions --------------------------------------------------------- */

/* Init
 *
 * Configures the clock device to run with its full period, at a frequency such
 * that the timestamp roughly corresponds to milliseconds.
 */
void
  clock__init();

/* Set Alarm
 *
 * Schedules an alarm to be called after the specified timeout.
 */
void
  clock__set_alarm(clock__alarm_t *alarm,
                   clock__timestamp_t timeout)
  NONNULL;

/* Cancel Alarm
 *
 * Removes the alarm from the clock, if it is set.
 */
void
  clock__cancel_alarm(clock__alarm_t *alarm)
  NONNULL;
  
/* Spin Once
 *
 * Calls the next alarm scheduled to run.
 */
void
  clock__spin_once();

/* Overflow ISR
 *
 * Call within ISR(RTC_OVF_vect) { ... }
 */
static inline void
  clock__overflow_isr();

/* Compare ISR
 *
 * Call within ISR(RTC_COMP_vect) { ... }
 */
static inline void
  clock__compare_isr();
  
/* Clock time
 *
 * Returns the current timestamp.
 */
static inline clock__timestamp_t
  clock__now();


/* Macros ----------------------------------------+--------+----------------- */



/* Inline Function Definitions ---------------------------------------------- */

/* Clock time
 *
 * Returns the current timestamp.
 */
clock__timestamp_t
clock__now()
{
  return clock__device__get_count();
}

void
clock__overflow_isr()
{
  clock__alarm_t *alarm;

  clock__device__disable_overflow_interrupt();

  /* Add any remaining alarms to the call list */
  alarm = (clock__alarm_t *) s_list__first(&clock.alarms);
  if (alarm != NULL) {
    s_list__push(&clock.call_list, &alarm->_super);
  }

  alarm = (clock__alarm_t *) s_list__first(&clock.alarms_ovf);
  if (alarm == NULL) {
    s_list__ctor(&clock.alarms);
    clock__device__disable_compare_interrupt();
    return;
  }

  /* Reset the lists to a known state */
  s_list__ctor_with_list_item(&clock.alarms, &alarm->_super);
  s_list__ctor(&clock.alarms_ovf);

  clock__device__set_alarm(alarm->timestamp);
  clock__device__enable_compare_interrupt(
    CLOCK__DEVICE_COMPARE_INTERRUPT_LVL);
}

void
clock__compare_isr()
{
  /* Assume that head is the next listener to be called */
  clock__alarm_t *alarm;
  const clock__timestamp_t now = clock__now() + CLOCK__DEVICE__SYNC_CYCLES;

  for (;;) {
    /* Look at the next alarm */
    alarm = (clock__alarm_t *) s_list__first(&clock.alarms);

    if (alarm == NULL) {
      clock__device__disable_compare_interrupt();
      break;
    }

    if (alarm->timestamp > now) {
      /* Schedule the next alarm */
      clock__device__set_alarm_at(alarm->timestamp);
      break;
    }

    s_list__shift(&clock.alarms);

    /* Add to the call list */
    s_list__push(&clock.call_list, &alarm->_super);
  }
}

#endif /* DRIVERS_CLOCK_H */