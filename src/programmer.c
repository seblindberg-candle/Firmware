#include <programmer.h>
#include <board.h>
#include <drivers/mmc.h>
#include <util/terminal.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

static bool_t
  detect_write_cmd(terminal__cmd_t *cmd, uint8_t c);
  
static bool_t
  parse_write_cmd(terminal__cmd_t *cmd, uint8_t c);


/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

static interface_t interface;


/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
programmer__init()
{
  terminal__cmd__ctor(&interface.write_cmd._super, detect_write_cmd, parse_write_cmd);
  terminal__add_cmd(&board.terminal, &interface.write_cmd._super);
  
  /* Verrify the chip */
  if (mmc__verify(&board.mmc) != MMC__OK) {
    // terminal__puts(&board.terminal, "Failed to verrify MMC\n\r");
  }
}

bool_t
detect_write_cmd(terminal__cmd_t *cmd, uint8_t c)
{
  struct programmer__write_cmd *write_cmd = (struct programmer__write_cmd *) cmd;
  if (c == 'W') {
    write_cmd->initiated_at = clock__time();
    terminal__puts(&board.terminal, "Detected W\n\r");
    return true;
  }

  return false;
}

bool_t
parse_write_cmd(terminal__cmd_t *cmd, uint8_t c)
{
  const struct programmer__write_cmd *write_cmd =
    (const struct programmer__write_cmd *) cmd;
  
  if (clock__time() - write_cmd->initiated_at > 2000) {
    return false;
  }
  
  switch (c) {
  default:
    terminal__puts(&board.terminal, "Parsing once\n\r");
    break;
  }

  return false;
}