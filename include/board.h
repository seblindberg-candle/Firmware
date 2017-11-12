#ifndef BOARD_H
#define BOARD_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <drivers/led.h>
#include <drivers/clock.h>
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




/* Global Variables --------------------------------------------------------- */

struct {
  led_t      power_led;
  led_t      status_led;
  
  uspi_t     mmc_interface;
  mmc_t      mmc;
  
  terminal_t terminal;
} board;


/* Public Functions --------------------------------------------------------- */

void
  board__init();


/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */



#endif /* BOARD_H */