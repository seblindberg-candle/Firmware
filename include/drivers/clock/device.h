#ifndef DRIVERS_CLOCK_DEVICE_H
#define DRIVERS_CLOCK_DEVICE_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>


/* Constants -------------------------------------+-------------------------- */




/* Data Types --------------------------------------------------------------- */

typedef enum {
  CLOCK__DEVICE__PRESCALER_0 = 0
} clock__device__prescaler_t;

typedef uint16_t clock__device__timestamp_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  clock__device__init(clock__device__timestamp_t period,
                      uint8_t prescaler);


/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */



#endif /* DRIVERS_CLOCK_DEVICE_H */