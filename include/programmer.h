#ifndef PROGRAMMER_H
#define PROGRAMMER_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <util/terminal.h>
#include <util/terminal/cmd.h>
#include <drivers/clock.h>
#include <drivers/mmc.h>


/* Constants -------------------------------------+-------------------------- */

typedef enum {
  PROGRAMMER__STATE_IDLE  = 0,
  PROGRAMMER__STATE_SIZE,
  PROGRAMMER__STATE_DATA,
  PROGRAMMER__STATE_ERROR,
  PROGRAMMER__STATE_ADDR0 = 16, /* Must be even */
  PROGRAMMER__STATE_ADDR1 = 17, /* Must be odd */
  PROGRAMMER__STATE_ADDR2 = 18, /* Must be even */
  PROGRAMMER__STATE_ADDR3 = 19, /* Must be odd */
  PROGRAMMER__STATE_ADDR  = 20
} programmer__state_t;


/* Data Types --------------------------------------------------------------- */

typedef struct {
  terminal__cmd_t    _super;
  clock__timestamp_t  initiated_at;
  programmer__state_t state;
  size_t              bytes_left;
  mmc__page_t         page;
} programmer__write_cmd_t;

typedef struct {
  programmer__write_cmd_t write_cmd;
  terminal__cmd_t         erase_cmd;
} programmer_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  programmer__init(terminal_t *terminal)
  NONNULL;


/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */



#endif /* PROGRAMMER_H */