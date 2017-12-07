#ifndef DRIVERS_CLOCK_H
#define DRIVERS_CLOCK_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <list/s_list.h>
#include <drivers/clock/device.h>
#include <drivers/clock/alarm.h>


/* Constants -------------------------------------+-------------------------- */

#define CLOCK__DEVICE_PRESCALER                   (0)
#define CLOCK__DEVICE_OVERFLOW_INTERRUPT_LVL      (0)
#define CLOCK__DEVICE_COMPARE_INTERRUPT_LVL       (0)


/* Data Types --------------------------------------------------------------- */

typedef struct {
  s_list_t         alarms;
  s_list_t         alarms_ovf;
  
#ifndef NDEBUG
  bool_t           is_initialized;
#endif
} clock_t;

typedef clock__device__timestamp_t clock__timestamp_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  clock__init();

static inline clock__timestamp_t
  clock__time();

void
  clock__set_alarm(clock__alarm_t *alarm,
                   clock__timestamp_t timestamp)
  NONNULL;

/* Macros ----------------------------------------+--------+----------------- */



/* Inline Function Definitions ---------------------------------------------- */

clock__timestamp_t
clock__time()
{
  return clock__device__get_count();
}

#endif /* DRIVERS_CLOCK_H */