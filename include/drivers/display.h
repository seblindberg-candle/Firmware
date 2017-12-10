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