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
static terminal_t terminal;


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
  uspi_t     mmc_interface;
  mmc_t      mmc;
  uint8_t    data[256];
  size_t     i;
  
  //clock__init_32mhz();
  
  /* Setup the terminal */
  usart__device__init(&USARTD0, USART__DEVICE__B57600,
                      USART__DEVICE__WRITE_ONLY);
  usart__device__map_io(&USARTD0, USART__DEVICE__REMAP);
  
  terminal__ctor(&terminal, &USARTD0);
  terminal__attatch(&terminal, &stdin, &stdout, &stderr);
  
  puts("Booted");
  
  uspi__ctor(&mmc_interface, &USARTC0, &PORTC, PIN4_bm);
  
  /* Configure SPI for the flash memory */
  uspi__init(&mmc_interface, USPI__B1MHz, USPI__MODE0_MSb);
  mmc__ctor(&mmc, &mmc_interface);
  
  /* TODO: Move into mmc library */
  PORTC.OUTSET = PIN5_bm | PIN4_bm | PIN0_bm; /* !WriteProtect, !CS, !HOLD */
  PORTC.DIRSET = PIN5_bm | PIN4_bm | PIN0_bm;
  PORTC.PIN2CTRL = PORT_OPC_PULLUP_gc;
  
  /* Verrify the chip */
  //assert(mmc__verify(&mmc) == MMC__OK);
  
  // mmc__write(mmc, address, write_buffer, 512);
  //flash_prgmr__ctor(&flash_prgrm, &mmc);
  
  /* Prepare some data */
  for (i = 0; i < sizeof(data); i ++) {
    data[i] = (uint8_t) i;
  }
  
  while (mmc__program_page(&mmc, 0x000000, data, 256) != MMC__OK) {
    puts("Failed to write");
    _delay_ms(1000);
  }
    
  for (;;) {
    if (mmc__read(&mmc, 0x000000, data, 256) != MMC__OK) {
      puts("Failed to read");
    }
    
    _delay_ms(1000);
    // data = terminal__next(&terminal);
    //flash_prgmr__feed(&flash_prgrm, data);
  }

  return 0;
}


/* Interupts –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

ISR(USARTD0_RXC_vect)
{
  terminal__rxc_isr(&terminal);
}

ISR(USARTD0_DRE_vect)
{
  terminal__dre_isr(&terminal);
}