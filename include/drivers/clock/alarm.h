#ifndef DRIVERS_CLOCK_ALARM_H
#define DRIVERS_CLOCK_ALARM_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <list/s_list_item.h>


/* Constants -------------------------------------+-------------------------- */




/* Data Types --------------------------------------------------------------- */

typedef uint16_t clock__timestamp_t;
typedef void  (*clock__alarm__callback_t)(void *ctx);

typedef struct {
  s_list_item_t           _super;
  clock__timestamp_t       timestamp;
  clock__alarm__callback_t callback;
  void                    *ctx;
} clock__alarm_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  clock__alarm__ctor(clock__alarm_t *alarm,
                     clock__alarm__callback_t callback,
                     void *ctx);

static inline void
  clock__alarm__call(const clock__alarm_t *alarm);

/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */

void
clock__alarm__call(const clock__alarm_t *alarm)
{
  alarm->callback(alarm->ctx);
}

#endif /* DRIVERS_CLOCK_ALARM_H */