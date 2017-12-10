#ifndef DRIVERS_CLOCK_DEVICE_H
#define DRIVERS_CLOCK_DEVICE_H 1

/* Interrupt vector declaration for the Clock device (RTC)
 *
   ISR(RTC_COMP_vect)
   {
   }

   ISR(RTC_OVF_vect)
   {
   }
 *
 */

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>


/* Constants -------------------------------------+-------------------------- */

#define CLOCK__DEVICE__SYNC_CYCLES                (3)


/* Data Types --------------------------------------------------------------- */

typedef uint16_t clock__device__timestamp_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

/* Init
 *
 * Starts the 32 kHz oscillator and enables the RTC device. When it is ready it
 * is configured with the given period and prescaler.
 *
 * Note that the 1 kHz output of the oscilator is used.
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
                      RTC_PRESCALER_t prescaler);

/* Set Alarm
 */
void
  clock__device__set_alarm(clock__device__timestamp_t timeout);
  
/* Set Alarm At
 */
static inline void
  clock__device__set_alarm_at(clock__device__timestamp_t timestamp);

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
  clock__device__enable_overflow_interrupt(RTC_OVFINTLVL_t level);
  
/* Disable Overflow Interrupt
 */
void
  clock__device__disable_overflow_interrupt();
  
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
  clock__device__enable_compare_interrupt(RTC_COMPINTLVL_t level);
 
/* Disable Compare Interrupt
 */
void
  clock__device__disable_compare_interrupt();
              
/* Is Busy
 *
 * Returns non-zero when the device is busy syncronizing. Call this before writing to any of the registers CNT, PER, COMP
 */
static inline bool_t
  clock__device__is_busy();

/* Get Count
 */
static inline clock__device__timestamp_t
  clock__device__get_count();
  
/* Set Count
 * Note: caller must wait until clock__device__is_busy() returns 0 before
 * setting the count.
 */
static inline void
  clock__device__set_count(clock__device__timestamp_t count);

/* Get Period
 */
static inline clock__device__timestamp_t
  clock__device__get_period();

/* Set Period
 * Note: caller must wait until clock__device__is_busy() returns 0 before
 * setting the period.
 */
static inline void
  clock__device__set_period(clock__device__timestamp_t period);
  
/* Get Compare Value
 */
static inline clock__device__timestamp_t
  clock__device__get_compare_value();

/* Set Compare Value
 * Note: caller must wait until clock__device__is_busy() returns 0 before
 * setting the compare value.
 */
static inline void
  clock__device__set_compare_value(clock__device__timestamp_t cmp);
  
/* Get Overflow Flag
 * Returns non-zero if the flag is set.
 */
static inline bool_t
  clock__device__get_overflow_flag();
  
/* Get Compare Flag
 * Returns non-zero if the flag is set.
 */
static inline bool_t
  clock__device__get_compare_flag();
  
/* Set Interrupts
 *
 * Set the interrupt levels directly. See
 * clock__device__enable_overflow_interrupt and
 * clock__device__enable_compare_interrupt for posible levels.
 */
static inline void
  clock__device__set_interrupts(RTC_OVFINTLVL_t ovf_level,
                                RTC_COMPINTLVL_t comp_level);


/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */

void
clock__device__set_alarm_at(clock__device__timestamp_t timestamp)
{
  while (clock__device__is_busy()) ;
  clock__device__set_compare_value(timestamp);
}

bool_t
clock__device__is_busy()
{
  return ( RTC.STATUS & RTC_SYNCBUSY_bm );
}

clock__device__timestamp_t
clock__device__get_count()
{
  return RTC.CNT;
}

void
clock__device__set_count(clock__device__timestamp_t count)
{
  RTC.CNT = count;
}

clock__device__timestamp_t
clock__device__get_period()
{
  return RTC.PER;
}

void
clock__device__set_period(clock__device__timestamp_t period)
{
  RTC.PER = period;
}

clock__device__timestamp_t
clock__device__get_compare_value()
{
  return RTC.COMP;
}

void
clock__device__set_compare_value(clock__device__timestamp_t cmp)
{
  RTC.COMP = cmp;
}

bool_t
clock__device__get_overflow_flag()
{
  return RTC.INTFLAGS & RTC_OVFIF_bm;
}

bool_t
clock__device__get_compare_flag()
{
  return RTC.INTFLAGS & RTC_COMPIF_bm;
}

void
clock__device__set_interrupts(RTC_OVFINTLVL_t ovf_level,
                              RTC_COMPINTLVL_t comp_level)
{
  RTC.INTCTRL = ovf_level | comp_level;
}

#endif /* DRIVERS_CLOCK_DEVICE_H */