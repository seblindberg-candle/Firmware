#include <drivers/uspi/device.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
uspi__device__init(USART_t *device, uspi__device__baudctrl_t baudctrl,
                   uspi__device__mode_t mode)
{
  PORT_t *port;

  /* Reset the device */
  device->CTRLA = 0;
  device->CTRLB = 0;
  device->CTRLD = 0;
  
  /* Enable SPI mode */
  device->CTRLC = USART_CMODE_MSPI_gc
                | (mode & 0x01 ? USART_UCPHA_bm : 0)
                | (mode & 0x04 ? USART_UDORD_bm : 0);

  device->BAUDCTRLA = (uint8_t) baudctrl;
  device->BAUDCTRLB = (baudctrl >> 8);

  port = usart__device__port(device);

  if (true) { //mode & USPI__DEVICE__MODE_REMAP_bm) {
    port->DIRCLR = PIN2_bm;           /* RX */
    port->DIRSET = PIN3_bm | PIN1_bm; /* TX and CLK */
    
    /* Enable pull-up on the MISO pin (for devices with high-z output) */
    // port->PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc;
    port->PIN2CTRL = PORT_OPC_PULLUP_gc;
    // port->PIN3CTRL = PORT_ISC_INPUT_DISABLE_gc;
  
    /* Invert clock polarity */
    if (mode & USPI__DEVICE__INV_CLOCK_bm) {
      port->PIN1CTRL = PORT_INVEN_bm;
    }
  } else {
    // port->DIRCLR = PIN6_bm;           /* RX */
    // port->DIRSET = PIN7_bm | PIN5_bm; /* TX and CLK */
    //
    // /* Enable pull-up on the MISO pin (for devices with high-z output) */
    // // port->PIN5CTRL = PORT_ISC_INPUT_DISABLE_gc;
    // port->PIN6CTRL = PORT_OPC_PULLUP_gc;
    // // port->PIN7CTRL = PORT_ISC_INPUT_DISABLE_gc;
    //
    // /* Invert clock polarity */
    // if (mode & 0x02) {
    //   port->PIN5CTRL = PORT_INVEN_bm;
    // }
  }
  
  /* Clear flags */
  device->STATUS = USART_TXCIF_bm | USART_RXCIF_bm;
  
  /* Enable both the transmitter and receiver */
  device->CTRLB = USART_TXEN_bm | USART_RXEN_bm;
}

void
uspi__device__write(USART_t *device, const void *src, size_t src_len)
{
  size_t rx_count = src_len;
  
  while (src_len) {
    usart__device__wait_dr_ready(device);
    device->DATA = *((uint8_t *) src++);
    --src_len;
    if (device->STATUS & USART_RXCIF_bm) {
      device->STATUS = USART_RXCIF_bm;
      --rx_count;
    }
  }
  
  while (rx_count) {
    usart__device__wait_rx_ready(device);
    device->STATUS = USART_RXCIF_bm;
    --rx_count;
  }
}

void
uspi__device__read(USART_t *device, void *dest, size_t dest_len)
{
  size_t rxcount = dest_len;
  
  while (dest_len) {
    while (!(device->STATUS & USART_DREIF_bm)) ;
    device->DATA = 0xFF;     //dummy write
    --dest_len;                  //bytes left to write
    if (device->STATUS & USART_RXCIF_bm) {
      *((uint8_t *) dest++) = device->DATA;
      --rxcount;       //bytes left to read
    }
  }
  while (rxcount) {        //wait for any pending bytes
    while (!(device->STATUS & USART_RXCIF_bm)) ;
    *((uint8_t *) dest++) = device->DATA;
    --rxcount;
  }
}

// void
// uspi__device__read(USART_t *device, void *dest, size_t dest_len)
// {
//   uint8_t *data = (uint8_t *) dest;
//
//   if (dest_len == 0) {
//     return;
//   }
//
//   /* Start reading the first byte */
//   usart__device__write_fast(device, 0xFF);
//
//   do {
//     if (-- dest_len == 0) {
//       break;
//     }
//
//     /* Initiate the next write */
//     usart__device__write_fast(device, 0xFF);
//
//     /* Read the previous byte */
//     usart__device__wait_rx_ready(device);
//     *data = device->DATA;
//     data ++;
//   } while (1);
//
//   /* Read the last byte */
//   usart__device__wait_rx_ready(device);
//   *data = device->DATA;
// }
