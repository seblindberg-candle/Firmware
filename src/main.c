/* Name: main.c
 * Author: Sebastian Lindberg
 * License:
 */

/* Includes ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

#include <compiler.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <drivers/terminal.h>
#include <drivers/mmc.h>
#include <drivers/led.h>
#include <drivers/usart.h>
#include <flash_prgmr.h>

#include <board.h>

/* Pin definitions –––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */


/* Globals –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */


/* Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

int main(void)
{
  flash_prgmr_t flash_prgmr;
  board__init();
  sei();
  
  /* Verrify the chip */
  if (mmc__verify(&board.mmc) != MMC__OK) {
    terminal__puts(&board.terminal, "Failed to verrify MMC\n\r");
  }
  
  // mmc__write(mmc, address, write_buffer, 512);
  flash_prgmr__ctor(&flash_prgmr, &board.mmc);
  
  /* Prepare some data */
  // uint8_t data[256];
  // for (i = 0; i < sizeof(data); i ++) {
  //   data[i] = (uint8_t) i;
  // }
  //
  // while (mmc__program_page(&mmc, 0x000000, data, 256) != MMC__OK) {
  //   puts("Failed to write");
  //   _delay_ms(1000);
  // }
  
  uint8_t c;
  
  for (;;) {
//     puts("Reading");
//     if (mmc__read(&board.mmc, 0x000000, data, 256) != MMC__OK) {
//       puts("Failed to read");
//     }
//
//     puts("Done");
//     counter ++;
//
// #ifndef NDEBUG
//     for (counter = 0; counter < 255; counter ++) {
//       printf("%02x ", data[counter]);
//     }
//
//     printf("status = %02x\n", USARTC0.STATUS);
// #endif
//
//     _delay_ms(1000);
    
    led__toggle(&board.status_led);
    c = terminal__next(&board.terminal);
    /* Echo the character back for now */
    usart__write_fast(&board.terminal.usart, c);
    
    if (flash_prgmr__feed(&flash_prgmr, c)) {
      terminal__puts(&board.terminal, "OK\n\r");
    }
  }

  return 0;
}


/* Interupts –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

ISR(USARTD0_RXC_vect)
{
  terminal__rxc_isr(&board.terminal);
}

ISR(USARTD0_DRE_vect)
{
  terminal__dre_isr(&board.terminal);
}