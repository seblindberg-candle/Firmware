#include <drivers/display.h>
#include <drivers/uspi/device.h>
#include <avr/interrupt.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

static inline void
  swap_buffers();
  
static inline void
  latch();


/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

static display_t display;


/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
display__init()
{
  display.read_buffer  = display.buffer;
  display.write_buffer = display.buffer + DISPLAY__BUFFER_SIZE;
  
  /* Setup the timer */
  PR.PRPC &= ~PR_TC4_bm;
  TCC4.PER = DISPLAY__PERIOD;
  
  TCC4.INTCTRLA = TC45_OVFINTLVL_HI_gc;
  TCC4.INTCTRLB = TC45_CCBINTLVL_HI_gc | TC45_CCAINTLVL_HI_gc;
  
  TCC4.CTRLA = TC45_CLKSEL_DIV1_gc;
  
  /* Setup the port */
  PORTD.DIRSET = DISPLAY__LE_bm | DISPLAY__OE_bm;
  
  /* Enable high level interrupts */
  PMIC.CTRL |= PMIC_HILVLEN_bm;
}

void
swap_buffers()
{
  uint8_t *tmp         = display.read_buffer;
  display.read_buffer  = display.write_buffer;
  display.write_buffer = tmp;
}

void
latch()
{
  //PORTD.OUTSET = DISPLAY__LE_bm;
  //PORTD.OUTCLR = DISPLAY__LE_bm;
  VPORT2.OUT |= DISPLAY__LE_bm;
  VPORT2.OUT &= ~DISPLAY__LE_bm;
}

ISR(TCC4_OVF_vect)
{
  
}

ISR(TCC4_CCA_vect)
{
  
}

ISR(TCC4_CCB_vect)
{
  latch();
}
