#include <drivers/clock.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void clock__init_32mhz()
{
  /* Activate the 32 MHz oscillator */
  CPU_CCP  = CCP_IOREG_gc;
  OSC.CTRL = OSC_RC32MEN_bm | OSC_RC32KEN_bm | OSC_RC2MEN_bm;

  /* Wait for oscillator to become ready */
  while (!(OSC.STATUS & OSC_RC32MRDY_bm)) ;

  /* Switch over */
  CPU_CCP  = CCP_IOREG_gc;
  CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
}


