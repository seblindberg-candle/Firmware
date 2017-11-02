#include <flash_prgmr.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

typedef enum {
  PARSER_STATE_INITIAL = 0,
  PARSER_STATE_SIZE    = 1,
  PARSER_STATE_DATA    = 2,
  PARSER_STATE_ADDR    = 3  /* Must be last */
} parser_state_t;

/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

bool_t
  parser(uint8_t c);


/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
flash_prgmr__feed(uint8_t byte)
{
  
}

/* Parser
 * Looks for a string containing the pattern "WdddAxxxx.....", where
 *   - ddd (decimal) is the length of the data
 *   - xxxx (hex) is the target address
 */
bool_t
parser(uint8_t c)
{
  static parser_state_t state = PARSER_STATE_INITIAL;
  /* TODO: Place these in a flash_programmer struct */
  static uint16_t size;
  static uint8_t d;
  static uint8_t addr[2];
  
  switch (state) {
  /* We want to optimize for this state, since it is likely
     the most common one */
  case PARSER_STATE_DATA:
    if (--size == 0) {
      state = PARSER_STATE_INITIAL;
    }
    return 1;
  /* Look for the characters 'W' (write) and 'E' (erase) */
  case PARSER_STATE_INITIAL:
    if (c == 'W') {
      state = PARSER_STATE_SIZE;
      size  = 0;
    } else if (c == 'E') {
      
    }
    break;
  /* Record the size, written in decimal, until we see an 'A' */
  case PARSER_STATE_SIZE:
    if (c >= '0' && c <= '9') {
      size = state * 10 + c - '0';
    } else if (c == 'A') {
      /* Check that size is valid [0,256] */
      
      state = PARSER_STATE_ADDR + 4;
    } else {
      state = PARSER_STATE_INITIAL;
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
      state = PARSER_STATE_INITIAL;
      break;
    }
    
    /* TODO: Clean up this mess */
    if ((state - PARSER_STATE_ADDR) % 2 == 0) {
      addr[(state - PARSER_STATE_ADDR - 1) / 2] = d << 4;
    } else {
      addr[(state - PARSER_STATE_ADDR - 1) / 2] |= d;
    }
    
    if (--state == PARSER_STATE_ADDR) {
      state = PARSER_STATE_DATA;
      /* Now we guarantee that the size and address are both valid */
    }
  }
  
  return 0;
}

