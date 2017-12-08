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

//extern clock_t clock;


/* Public Functions --------------------------------------------------------- */

void
  clock__init();

static inline clock__timestamp_t
  clock__time();
  
void
  clock__spin_once();
  
void
  clock__overflow_isr();

void
  clock__compare_isr();

void
  clock__set_alarm(clock__alarm_t *alarm,
                   clock__timestamp_t timestamp)
  NONNULL;
  
void
  clock__cancel_alarm(clock__alarm_t *alarm)
  NONNULL;

/* Macros ----------------------------------------+--------+----------------- */



/* Inline Function Definitions ---------------------------------------------- */

clock__timestamp_t
clock__time()
{
  return clock__device__get_count();
}

#endif /* DRIVERS_CLOCK_H */