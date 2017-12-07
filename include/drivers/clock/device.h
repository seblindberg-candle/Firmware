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




/* Data Types --------------------------------------------------------------- */

typedef uint16_t clock__device__timestamp_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

/* Init
 */
void
  clock__device__init(clock__device__timestamp_t period,
                      RTC_PRESCALER_t prescaler);

/* Set Alarm
 */
void
  clock__device__set_alarm(clock__device__timestamp_t timeout);
  
/* Enable Overflow Interrupt
 */
void
  clock__device__enable_overflow_interrupt(RTC_OVFINTLVL_t level);
  
/* Disable Overflow Interrupt
 */
void
  clock__device__disable_overflow_interrupt();
  
/* Enable Compare Interrupt
 */
void
  clock__device__enable_compare_interrupt(RTC_COMPINTLVL_t level);
 
/* Disable Compare Interrupt
 */
void
  clock__device__disable_compare_interrupt();
            
/* Is Busy
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


/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */

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

#endif /* DRIVERS_CLOCK_DEVICE_H */