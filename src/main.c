/* Name: main.c
 * Author: Sebastian Lindberg
 * License:
 */

/* Includes ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

#include <compiler.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <drivers/clock.h>
#include <drivers/terminal.h>
#include <drivers/mmc.h>
#include <stdio.h>


/* Pin definitions –––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */


/* Globals –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

// static uint8_t write_buffer[512];


/* Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void mmc_cs(bool_t enable)
{
  if (enable) {
    PORTC.OUTCLR = PIN4_bm;
  } else {
    PORTC.OUTSET = PIN4_bm;
  }
}

int main(void)
{
  terminal_t terminal;
  mmc_t      mmc;
  // uint8_t    data;
  
  //clock__init_32mhz();
  
  usart__init(&USARTD0, USART__B57600, USART__WRITE_ONLY);
  usart__map_io(&USARTD0, USART__REMAP);
  
  terminal__ctor(&terminal, &USARTD0);
  terminal__attatch(&terminal, &stdin, &stdout, &stderr);
  
  puts("Booted");
  
  /* Configure SPI for the flash memory */
  uspi__init(&USARTC0, USPI__B1MHz, USPI__MODE0_MSb);
  mmc__ctor(&mmc, &USARTC0, mmc_cs);
  
  PORTC.OUTSET = PIN5_bm | PIN4_bm | PIN0_bm; /* !WriteProtect, !CS, !HOLD */
  PORTC.DIRSET = PIN5_bm | PIN4_bm | PIN3_bm | PIN1_bm | PIN0_bm;
  PORTC.PIN2CTRL = PORT_OPC_PULLUP_gc;
  
  // mmc__write(mmc, address, write_buffer, 512);
  //flash_prgmr__ctor(&flash_prgrm, &mmc);
  
  for (;;) {
    if (mmc__verify(&mmc) != MMC__OK) {
      puts("error");
    }
    
    _delay_ms(1000);
    // data = terminal__next(&terminal);
    //flash_prgmr__feed(&flash_prgrm, data);
  }

  return 0;
}


/* Interupts –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */
