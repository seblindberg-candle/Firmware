#include <programmer.h>
#include <board.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

#define CASE_IGNORE(c)                            (c & 0xDF)


/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

static bool_t
  detect_write_cmd(terminal__cmd_t *cmd, uint8_t c);
  
static bool_t
  parse_write_cmd(terminal__cmd_t *cmd, uint8_t c);
  
static bool_t
  detect_erase_cmd(terminal__cmd_t *cmd, uint8_t c);
  
static bool_t
  parse_erase_cmd(terminal__cmd_t *cmd, uint8_t c);


/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

static programmer_t interface;


/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
programmer__init(terminal_t *terminal)
{
  terminal__cmd__ctor(&interface.write_cmd._super, detect_write_cmd,
                      parse_write_cmd);
  terminal__add_cmd(terminal, &interface.write_cmd._super);
  
  terminal__cmd__ctor(&interface.erase_cmd, detect_erase_cmd, parse_erase_cmd);
  terminal__add_cmd(terminal, &interface.erase_cmd);
  
  /* Verrify the chip */
  // if (mmc__verify(&board.mmc) != MMC__OK) {
  //   // terminal__puts(&board.terminal, "Failed to verrify MMC\n\r");
  // }
}

bool_t
detect_write_cmd(terminal__cmd_t *cmd, uint8_t c)
{
  programmer__write_cmd_t *write_cmd = (programmer__write_cmd_t *) cmd;
  
  if (CASE_IGNORE(c) == 'W') {
    write_cmd->initiated_at = clock__now();
    write_cmd->state        = PROGRAMMER__STATE_IDLE;
    // terminal__puts(&board.terminal, "Detected w\n\r");
    return true;
  }

  return false;
}

bool_t
parse_write_cmd(terminal__cmd_t *cmd, uint8_t c)
{
  programmer__write_cmd_t *write_cmd = (programmer__write_cmd_t *) cmd;
  
  /* Exit if the write command has expired */
  if (clock__now() - write_cmd->initiated_at > 2000) {
    write_cmd->state = PROGRAMMER__STATE_IDLE;
    return false;
  }
  
  switch (write_cmd->state) {
  case PROGRAMMER__STATE_DATA:
    
    break;
    
  case PROGRAMMER__STATE_IDLE:
    write_cmd->bytes_left = 0;
    write_cmd->state      = PROGRAMMER__STATE_SIZE;
  case PROGRAMMER__STATE_SIZE:
    {
      const uint8_t d = c - '0';
      if (d <= 9) {
        write_cmd->bytes_left = write_cmd->bytes_left * 10 + d;
      } else if (CASE_IGNORE(c) == 'A') {
        /* TODO: Check that size is valid [0,256] */
        write_cmd->state = PROGRAMMER__STATE_ADDR;
      } else {
        write_cmd->state = PROGRAMMER__STATE_ERROR;
        return false;
      }
    }
    break;
    
  case PROGRAMMER__STATE_ERROR:
    return false;
    
  default:
    {
      uint8_t d = c - '0';
      do {
        if (d <= 9) {
          break;
        }
        
        d = CASE_IGNORE(c) - 'A';
        
        if (d <= 5) {
          d += 10;
          break;
        }
        
        write_cmd->state = PROGRAMMER__STATE_ERROR;
        return false;
      } while (0);
      
      /* d is now in the range [0x00, 0x0F] */
      
    }
    
    
    
    break;
  }

  return false;
}

bool_t
detect_erase_cmd(terminal__cmd_t *cmd, uint8_t c)
{
  if (CASE_IGNORE(c) == 'E') {
    terminal__puts(&board.terminal, "Detected E\n\rErase? [y/n]\n\r");
    return true;
  }

  return false;
}

bool_t
parse_erase_cmd(terminal__cmd_t *cmd, uint8_t c)
{
  static bool_t is_done = false;
  
  if (is_done) {
    is_done = false;
    return false;
  }
  
  if (CASE_IGNORE(c) == 'Y') {
    terminal__puts(&board.terminal, "Erasing\n\r");
    is_done = true;
    return true;
  }
  
  return false;
}