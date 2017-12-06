#ifndef DRIVERS_CLOCK_H
#define DRIVERS_CLOCK_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <list/s_list.h>
#include <drivers/clock/device.h>
#include <drivers/clock/alarm.h>


/* Constants -------------------------------------+-------------------------- */




/* Data Types --------------------------------------------------------------- */

typedef struct {
  s_list_t         alarms;
  clock__alarm_t  *period_tail;
} clock_t;

typedef clock__device__timestamp_t clock__timestamp_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  clock__ctor(clock_t *clock);
  NONNULL;

void
  clock__set_alarm(clock_t *clock, clock__alarm_t *alarm,
                   clock__timestamp_t timestamp)
  NONNULL;

static inline bool_t
  clock__has_alarms(clock_t * const clock)
  NONNULL;
  
static inline void
  clock__isr(clock_t *clock)
  NONNULL;

/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */

bool_t
clock__has_alarms(clock_t * const clock)
{
  return !s_list__is_empty(&clock->alarms);
}

void
clock__isr(clock_t *clock)
{
  /* Assume that head is the next listener to be called */
  const clock__alarm_t *alarm;
  clock__timestamp_t    now;

  for (;;) {
    /* Look at the next alarm */
    alarm = (const clock__alarm_t *) s_list__first(&clock->alarms);
    
    if (alarm == NULL) {
      /* Disable the interrupt */
      break;
    }
    
    if (alarm->timestamp != now) {
      /* Schedule the next alarm */
      break;
    }
    
    s_list__shift(&clock->alarms);
    
    clock__alarm__call(alarm);
  }
}

#endif /* DRIVERS_CLOCK_H */