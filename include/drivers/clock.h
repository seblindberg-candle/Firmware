#ifndef DRIVERS_CLOCK_H
#define DRIVERS_CLOCK_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>


/* Constants -------------------------------------+-------------------------- */




/* Data Types --------------------------------------------------------------- */

typedef uint16_t clock__timestamp_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  clock__init_32mhz();
  
void
  clock__init_rtc();

static inline clock__timestamp_t
  clock__time();

/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */



#endif /* DRIVERS_CLOCK_H */