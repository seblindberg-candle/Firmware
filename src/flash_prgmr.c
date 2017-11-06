#include <flash_prgmr.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

static bool_t
  parser(flash_prgmr_t *prmgr, uint8_t c);
  
static inline bool_t
  write_buffer(flash_prgmr_t *prmgr);


/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
flash_prgmr__ctor(flash_prgmr_t *prgmr, const mmc_t *memory)
{
  prgmr->memory       = memory;
  prgmr->parser_state = FLASH_PRGMR__PARSER_STATE_INITIAL;
}

void
flash_prgmr__feed(flash_prgmr_t *prmgr, uint8_t byte)
{
  
}

/* Parser
 * Looks for a string containing the pattern "WdddAxxxx.....", where
 *   - ddd (decimal) is the length of the data
 *   - xxxx (hex) is the target address
 */
bool_t
parser(flash_prgmr_t *prmgr, uint8_t c)
{
  flash_prgmr__parser_state_t state = prmgr->parser_state;
  /* TODO: Place these in a flash_programmer struct */
  static uint16_t size;
  static uint8_t d;
  static uint8_t addr[2];
  
  switch (state) {
  /* We want to optimize for this state, since it is likely
     the most common one */
  case FLASH_PRGMR__PARSER_STATE_DATA:
    if (--size == 0) {
      state = FLASH_PRGMR__PARSER_STATE_INITIAL;
    }
    return 1;
  /* Look for the characters 'W' (write) and 'E' (erase) */
  case FLASH_PRGMR__PARSER_STATE_INITIAL:
    if (c == 'W') {
      state = FLASH_PRGMR__PARSER_STATE_SIZE;
      size  = 0;
    } else if (c == 'E') {
      
    }
    break;
  /* Record the size, written in decimal, until we see an 'A' */
  case FLASH_PRGMR__PARSER_STATE_SIZE:
    if (c >= '0' && c <= '9') {
      size = state * 10 + c - '0';
    } else if (c == 'A') {
      /* Check that size is valid [0,256] */
      
      state = FLASH_PRGMR__PARSER_STATE_ADDR + 4;
    } else {
      state = FLASH_PRGMR__PARSER_STATE_INITIAL;
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
      state = FLASH_PRGMR__PARSER_STATE_INITIAL;
      break;
    }
    
    /* TODO: Clean up this mess */
    if ((state - FLASH_PRGMR__PARSER_STATE_ADDR) % 2 == 0) {
      addr[(state - FLASH_PRGMR__PARSER_STATE_ADDR - 1) / 2] = d << 4;
    } else {
      addr[(state - FLASH_PRGMR__PARSER_STATE_ADDR - 1) / 2] |= d;
    }
    
    if (--state == FLASH_PRGMR__PARSER_STATE_ADDR) {
      state = FLASH_PRGMR__PARSER_STATE_DATA;
      /* Now we guarantee that the size and address are both valid */
    }
  }
  
  return 0;
}

bool_t
write_buffer(flash_prgmr_t *prmgr)
{
  return 1;
}

