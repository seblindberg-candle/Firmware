/* Name: main.c
 * Author: Sebastian Lindberg
 * License:
 */

/* Includes ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

#include <compiler.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <util/terminal.h>

#include <drivers/mmc.h>
#include <drivers/led.h>
#include <drivers/usart.h>
#include <board.h>

// #include <drivers/clock/device.h>
#include <drivers/clock.h>
#include <drivers/clock/alarm.h>

#include <drivers/button.h>

#include <programmer.h>

/* Pin definitions –––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */


/* Globals –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */


/* Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
pulse_handler(void *ctx)
{
  clock__alarm_t *alarm = (clock__alarm_t *) ctx;
  led__toggle(&board.status_led);
  
  clock__set_alarm(alarm, 1000);
}

void
button_handler(button_t *button)
{
  struct power_button *power_button = (struct power_button *) button;
  if (button__is_down(button)) {
    if (power_button->active) {
      clock__set_alarm(&power_button->timeout_alarm, 3000);
    } else {
      clock__set_alarm(&power_button->timeout_alarm, 1);
    }
    
    terminal__puts(&board.terminal, "Down\n\r");
  } else {
    clock__cancel_alarm(&power_button->timeout_alarm);
    terminal__puts(&board.terminal, "Up\n\r");
  }
}

void
button_timeout_handler(void *ctx)
{
  clock__alarm_t *pulse_alarm = (clock__alarm_t *) ctx;
  terminal__puts(&board.terminal, "Long hold\n\r");
  
  if (board.power_button.active) {
    clock__cancel_alarm(pulse_alarm);
    led__off(&board.status_led);
    led__off(&board.power_led);
    board.power_button.active = false;
  } else {
    led__on(&board.power_led);
    clock__set_alarm(pulse_alarm, 1000);
    board.power_button.active = true;
  }
}

int main(void)
{
  board__init();
  
  clock__init();
  clock__alarm_t pulse_alarm;
  clock__alarm__ctor(&pulse_alarm, pulse_handler, &pulse_alarm);
  clock__set_alarm(&pulse_alarm, 1000);
  
  clock__alarm__ctor(&board.power_button.timeout_alarm,
                     button_timeout_handler, &pulse_alarm);
  button__register_callback(&board.power_button._super, button_handler);
    
  programmer__init();
  
  sei();

  for (;;) {
    clock__spin_once();
    terminal__spin_once(&board.terminal);
  }

  return 0;
}


/* Interupts –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

ISR(RTC_OVF_vect)
{
  clock__overflow_isr();
}

ISR(RTC_COMP_vect)
{
  clock__compare_isr();
}

ISR(USARTD0_RXC_vect)
{
  terminal__rxc_isr(&board.terminal);
}

ISR(USARTD0_DRE_vect)
{
  terminal__dre_isr(&board.terminal);
}

ISR(PORTC_INT_vect)
{
  button__isr(&board.power_button._super);
}