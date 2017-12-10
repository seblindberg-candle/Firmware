#ifndef PROGRAMMER_H
#define PROGRAMMER_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <util/terminal/cmd.h>
#include <drivers/clock.h>


/* Constants -------------------------------------+-------------------------- */




/* Data Types --------------------------------------------------------------- */

typedef struct {
  struct programmer__write_cmd {
    terminal__cmd_t    _super;
    clock__timestamp_t  initiated_at;
  } write_cmd;
} interface_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  programmer__init();


/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */



#endif /* PROGRAMMER_H */