#include <drivers/clock/device.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
clock__device__init(uint16_t period, uint8_t prescaler)
{
  WRITE_TO_PROTECTED_IOREG();
  OSC.CTRL |= OSC_RC32KEN_bm;

  /* Wait for oscillator to become ready */
  while (!(OSC.STATUS & OSC_RC32KRDY_bm)) ;

  CLK.RTCCTRL = CLK_RTCSRC_RCOSC_gc | CLK_RTCEN_bm;

  /* Wait while the RTC becoms ready */
  while (RTC.STATUS & RTC_SYNCBUSY_bm) ;

  RTC.PER = period - 1;
  RTC.CNT = 0;
  RTC.COMP = 0;
  RTC.CTRL = ( RTC.CTRL & ~RTC_PRESCALER_gm ) | prescaler;
}
