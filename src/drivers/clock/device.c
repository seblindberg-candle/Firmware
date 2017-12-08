#include <drivers/clock/device.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

/* Init
 *
 * Starts the 32 kHz oscillator and enables the RTC device. When it is ready it
 * is configured with the given period and prescaler.
 *
 * Set the period to 0 to use the full range of the timer.
 *
 * The posible prescaler values are:
 *
 * - RTC_PRESCALER_OFF_gc
 * - RTC_PRESCALER_DIV1_gc
 * - RTC_PRESCALER_DIV2_gc
 * - RTC_PRESCALER_DIV8_gc
 * - RTC_PRESCALER_DIV16_gc
 * - RTC_PRESCALER_DIV64_gc
 * - RTC_PRESCALER_DIV256_gc
 * - RTC_PRESCALER_DIV1024_gc
 */
void
clock__device__init(clock__device__timestamp_t period,
                    RTC_PRESCALER_t prescaler)
{
  WRITE_TO_PROTECTED_IOREG();
  OSC.CTRL |= OSC_RC32KEN_bm;

  /* Wait for oscillator to become ready */
  while (!(OSC.STATUS & OSC_RC32KRDY_bm)) ;

  /* Enable the RTC device */
  PR.PRGEN   &= ~PR_RTC_bm;
  CLK.RTCCTRL = CLK_RTCSRC_RCOSC_gc | CLK_RTCEN_bm;

  /* Wait while the RTC becoms ready */
  while (clock__device__is_busy()) ;
  
  RTC.PER  = period - 1;
  RTC.CNT  = 0;
  RTC.COMP = 0;
  RTC.CTRL = ( RTC.CTRL & ~RTC_PRESCALER_gm ) | prescaler;
}

void
clock__device__set_alarm(clock__device__timestamp_t timeout)
{
  const uint16_t period = clock__device__get_period();
  uint16_t       cmp;
  
  if (timeout < CLOCK__DEVICE__SYNC_CYCLES) {
    timeout = CLOCK__DEVICE__SYNC_CYCLES;
  }
  
  cmp = clock__device__get_count() + timeout;
  
  while (cmp > period) {
    cmp -= period;
  }
  
  while (clock__device__is_busy()) ;
  
  clock__device__set_compare_value(cmp);
}

/* Enable Overflow Interrupt
 *
 * The posible interrupt levels are:
 *
 * - RTC_OVFINTLVL_OFF_gc
 * - RTC_OVFINTLVL_LO_gc
 * - RTC_OVFINTLVL_MED_gc
 * - RTC_OVFINTLVL_HI_gc
 */
void
clock__device__enable_overflow_interrupt(RTC_OVFINTLVL_t level)
{
  RTC.INTCTRL = ( RTC.INTCTRL & ~RTC_OVFINTLVL_gm ) | level;
}

/* Disable Overflow Interrupt
 */
void
clock__device__disable_overflow_interrupt()
{
  RTC.INTCTRL = ( RTC.INTCTRL & ~RTC_OVFINTLVL_gm );
}

/* Enable Compare Interrupt
 *
 * The posible interrupt levels are:
 *
 * - RTC_COMPINTLVL_OFF_gc
 * - RTC_COMPINTLVL_LO_gc
 * - RTC_COMPINTLVL_MED_gc
 * - RTC_COMPINTLVL_HI_gc
 */
void
clock__device__enable_compare_interrupt(RTC_COMPINTLVL_t level)
{
  RTC.INTCTRL = ( RTC.INTCTRL & ~RTC_COMPINTLVL_gm ) | level;
}

/* Disable Compare Interrupt
 */
void
clock__device__disable_compare_interrupt()
{
  RTC.INTCTRL = ( RTC.INTCTRL & ~RTC_COMPINTLVL_gm );
}
  