#ifndef TERMINAL_CMD_H
#define TERMINAL_CMD_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <list/s_list_item.h>


/* Constants -------------------------------------+-------------------------- */




/* Data Types --------------------------------------------------------------- */


typedef struct terminal__cmd_t terminal__cmd_t;
typedef bool_t (*terminal__cmd__detector_t)(terminal__cmd_t *cmd, uint8_t c);
typedef bool_t (*terminal__cmd__parser_t)(terminal__cmd_t *cmd, uint8_t c);

struct terminal__cmd_t {
  s_list_item_t _super;
  terminal__cmd__detector_t detector;
  terminal__cmd__parser_t   parser;
};


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  terminal__cmd__ctor(terminal__cmd_t *cmd, terminal__cmd__detector_t detector,
                      terminal__cmd__parser_t parser)
  NONNULL;
  
static inline bool_t
  terminal__cmd__detect(terminal__cmd_t *cmd, uint8_t c)
  NONNULL;
  
static inline bool_t
  terminal__cmd__parse(terminal__cmd_t *cmd, uint8_t c)
  NONNULL;
  
static inline terminal__cmd_t *
  terminal__cmd__next(const terminal__cmd_t *cmd)
  NONNULL;

/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */

bool_t
terminal__cmd__detect(terminal__cmd_t *cmd, uint8_t c)
{
  return cmd->detector(cmd, c);
}

bool_t
terminal__cmd__parse(terminal__cmd_t *cmd, uint8_t c)
{
  return cmd->parser(cmd, c);
}

terminal__cmd_t *
terminal__cmd__next(const terminal__cmd_t *cmd)
{
  return (terminal__cmd_t *) s_list_item__next(&cmd->_super);
}

#endif /* TERMINAL_CMD_H */