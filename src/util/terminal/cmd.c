#include <util/terminal/cmd.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
terminal__cmd__ctor(terminal__cmd_t *cmd, terminal__cmd__detector_t detector,
                    terminal__cmd__parser_t parser)
{
  s_list_item__ctor(&cmd->_super);
  
  cmd->detector = detector;
  cmd->parser   = parser;
}
