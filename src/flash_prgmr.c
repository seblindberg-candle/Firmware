#include <flash_prgmr.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

static inline bool_t
  parse(flash_prgmr_t *prgmr, uint8_t c);
  
static inline void
  write_buffer(flash_prgmr_t *prgmr);


/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
flash_prgmr__ctor(flash_prgmr_t *prgmr, mmc_t *memory)
{
  WRITE_CONST(prgmr->memory, mmc_t *, memory);
  prgmr->state = FLASH_PRGMR__PARSER_STATE_INITIAL;
}

bool_t
flash_prgmr__feed(flash_prgmr_t *prgmr, uint8_t byte)
{
  if (parse(prgmr, byte)) {
    write_buffer(prgmr);
    return true;
  }
  
  return false;
}

/* Parse
 * Looks for a string containing the pattern "WdddAxxxx.....", where
 *   - ddd (decimal) is the length of the data
 *   - xxxx (hex) is the target address
 */
bool_t
parse(flash_prgmr_t *prgmr, uint8_t c)
{
  /* TODO: Place these in a flash_programmer struct */
  static uint8_t d;
  uint8_t *addr = (uint8_t *) &prgmr->page;
  
  switch (prgmr->state) {
  /* We want to optimize for this state, since it is likely
     the most common one */
  case FLASH_PRGMR__PARSER_STATE_DATA:
    prgmr->buffer[prgmr->bytes_read++] = c;
    
    if (--prgmr->bytes_left == 0) {
      prgmr->state = FLASH_PRGMR__PARSER_STATE_INITIAL;
      return true;
    } else if (prgmr->bytes_read == MMC__PAGE_SIZE) {
      prgmr->bytes_read = 0;
      return true;
    }
    break;
    
  /* Look for the characters 'W' (write) and 'E' (erase) */
  case FLASH_PRGMR__PARSER_STATE_INITIAL:
    if (c == 'W') {
      prgmr->state = FLASH_PRGMR__PARSER_STATE_SIZE;
      prgmr->bytes_left = 0;
    } else if (c == 'E') {
      
    }
    break;
    
  /* Record the size, written in decimal, until we see an 'A' */
  case FLASH_PRGMR__PARSER_STATE_SIZE:
    if (c >= '0' && c <= '9') {
      prgmr->bytes_left = prgmr->bytes_left * 10 + c - '0';
    } else if (c == 'A') {
      /* TODO: Check that size is valid [0,256] */
      
      prgmr->state = FLASH_PRGMR__PARSER_STATE_ADDR + 4;
    } else {
      prgmr->state = FLASH_PRGMR__PARSER_STATE_INITIAL;
    }
    break;
    
  /* Record the address, written in hex */
  default:
    if (c >= '0' && c <= '9') {
      d = c - '0';
    } else if (c >= 'a' && c <= 'f') {
      d = c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
      d = c - 'A' + 10;
    } else {
      prgmr->state = FLASH_PRGMR__PARSER_STATE_INITIAL;
      break;
    }
    
    /* TODO: Clean up this mess */
    if ((prgmr->state - FLASH_PRGMR__PARSER_STATE_ADDR) % 2 == 0) {
      addr[(prgmr->state - FLASH_PRGMR__PARSER_STATE_ADDR - 1) / 2] = d << 4;
    } else {
      addr[(prgmr->state - FLASH_PRGMR__PARSER_STATE_ADDR - 1) / 2] |= d;
    }
    
    if (--prgmr->state == FLASH_PRGMR__PARSER_STATE_ADDR) {
      prgmr->state = FLASH_PRGMR__PARSER_STATE_DATA;
      prgmr->bytes_read = 0;
      /* Now we guarantee that the size and address are both valid */
    }
  }
  
  return 0;
}

void
write_buffer(flash_prgmr_t *prgmr)
{
}

