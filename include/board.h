#ifndef BOARD_H
#define BOARD_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <drivers/led.h>
#include <drivers/clock.h>
#include <drivers/clock/alarm.h>
#include <drivers/button.h>
#include <drivers/mmc.h>
#include <drivers/uspi.h>
#include <util/terminal.h>


/* Constants -------------------------------------+-------------------------- */

#define BOARD__POWER_LED_PORT                     (PORTR)
#define BOARD__POWER_LED_PIN_bm                   (PIN0_bm)

#define BOARD__STATUS_LED_PORT                    (PORTR)
#define BOARD__STATUS_LED_PIN_bm                  (PIN1_bm)

#define BOARD__TERMINAL_DEVICE                    (USARTD0)

#define BOARD__MMC_DEVICE                         (USARTC0)

/* Data Types --------------------------------------------------------------- */

typedef struct {
  led_t      power_led;
  led_t      status_led;

  struct power_button {
    button_t      _super;
    clock__alarm_t timeout_alarm;
    bool_t         active;
  } power_button;

  uspi_t     mmc_interface;
  mmc_t      mmc;

  terminal_t terminal;
} board_t;


/* Global Variables --------------------------------------------------------- */

extern board_t board;


/* Public Functions --------------------------------------------------------- */

void
  board__init();


/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */



#endif /* BOARD_H */