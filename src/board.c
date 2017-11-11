#include <board.h>
#include <drivers/usart.h>
#include <drivers/usart/device.h>
#include <drivers/uspi/device.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
board__init()
{
  const char *boot_msg = "Candle [mmc]\n\r\n\r";
  
  //clock__init_32mhz();
  
  led__ctor(&board.power_led,  &BOARD__POWER_LED_PORT,
                                BOARD__POWER_LED_PIN_bm);
  led__ctor(&board.status_led, &BOARD__STATUS_LED_PORT,
                                BOARD__STATUS_LED_PIN_bm);

  // button__ctor(&board.button, &PORTD, 3);
  // button__register_callback(&board.button, &button_callback);
  //
  // button__isr(&board.button);
  

  /* Configure terminal device */
  usart__device__init(&BOARD__TERMINAL_DEVICE, USART__DEVICE__B57600,
                      USART__DEVICE__RW);
  usart__device__map_gpio(&BOARD__TERMINAL_DEVICE, USART__DEVICE__REMAP);
  
  /* Configure terminal */
  terminal__ctor(&board.terminal, &BOARD__TERMINAL_DEVICE);
  terminal__attatch(&board.terminal, &stdin, &stdout, &stderr);
  
  /* Configure SPI for the flash memory */
  uspi__device__init(&BOARD__MMC_DEVICE, USPI__DEVICE__B1MHz,
                     USPI__DEVICE__MODE0);
  uspi__ctor(&board.mmc_interface, &BOARD__MMC_DEVICE, &MMC__CS_PORT,
             MMC__CS_PIN_bm);
  
  mmc__ctor(&board.mmc, &board.mmc_interface);
  
  led__on(&board.power_led);
  terminal__puts(&board.terminal, boot_msg);
}
