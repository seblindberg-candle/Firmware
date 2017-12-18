#ifndef DRIVERS_DISPLAY_H
#define DRIVERS_DISPLAY_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>


/* Constants -------------------------------------+--------+----------------- */

#define DISPLAY__WIDTH                            (8)
#define DISPLAY__HEIGHT                           (16)
#define DISPLAY__NUM_PIXELS                                \
  (DISPLAY__WIDTH * DISPLAY__HEIGHT)
#define DISPLAY__BUFFER_SIZE                      (2 * DISPLAY__NUM_PIXELS)

#define DISPLAY__T                                (131)
#define DISPLAY__BIT_DEPTH                        (8)
#define DISPLAY__PERIOD                                   \
  (((1 << DISPLAY__BIT_DEPTH) - 1) * DISPLAY__T)

#define DISPLAY__COMP_BIT_0                       (DISPLAY__T)
#define DISPLAY__COMP_BIT_1                               \
  (DISPLAY__COMP_BIT_0 + 2 * DISPLAY__T)
#define DISPLAY__COMP_BIT_2                               \
  (DISPLAY__COMP_BIT_1 + 4 * DISPLAY__T)
#define DISPLAY__COMP_BIT_3                               \
  (DISPLAY__COMP_BIT_2 + 8 * DISPLAY__T)
#define DISPLAY__COMP_BIT_4                               \
  (DISPLAY__COMP_BIT_3 + 16 * DISPLAY__T)
#define DISPLAY__COMP_BIT_5                               \
  (DISPLAY__COMP_BIT_4 + 32 * DISPLAY__T)
#define DISPLAY__COMP_BIT_6                               \
  (DISPLAY__COMP_BIT_5 + 64 * DISPLAY__T)
#define DISPLAY__COMP_BIT_7                               \
  (DISPLAY__COMP_BIT_6 + 128 * DISPLAY__T)

#define DISPLAY__LE_bm                           PIN2_bm
#define DISPLAY__OE_bm                           PIN0_bm

/* Data Types --------------------------------------------------------------- */

typedef struct {
  /* Allocate a buffer big enough for two frames to allow
     for reading while writing */
  uint8_t buffer[DISPLAY__BUFFER_SIZE];
  uint8_t *read_buffer;
  uint8_t *write_buffer;
} display_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  display__init();

bool_t
  display__need_update();

void
  display__update();


/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */



#endif /* DRIVERS_DISPLAY_H */