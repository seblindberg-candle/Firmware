#include <drivers/display.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

static inline void
  swap_buffers();


/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

static display_t display;


/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
display__init()
{
  display.read_buffer  = display.buffer;
  display.write_buffer = display.buffer + DISPLAY__BUFFER_SIZE;
}

void
swap_buffers()
{
  uint8_t *tmp         = display.read_buffer;
  display.read_buffer  = display.write_buffer;
  display.write_buffer = tmp;
}