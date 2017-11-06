#ifndef FLASH_PRGRM_H
#define FLASH_PRGRM_H 1

/* Includes ----------------------------------------------------------------- */

#include <compiler.h>
#include <drivers/mmc.h>


/* Constants -------------------------------------+-------------------------- */




/* Data Types --------------------------------------------------------------- */

typedef enum {
  FLASH_PRGMR__PARSER_STATE_INITIAL = 0,
  FLASH_PRGMR__PARSER_STATE_SIZE    = 1,
  FLASH_PRGMR__PARSER_STATE_DATA    = 2,
  FLASH_PRGMR__PARSER_STATE_ADDR    = 3  /* Must be last */
} flash_prgmr__parser_state_t;

typedef struct {
  mmc_t                      *memory;
  mmc__address_t              bytes_left;
  flash_prgmr__parser_state_t parser_state;
  uint8_t                     buffer[MMC__PAGE_SIZE];
} flash_prgmr_t;


/* Global Variables --------------------------------------------------------- */




/* Public Functions --------------------------------------------------------- */

void
  flash_prgmr__ctor(flash_prgmr_t *prgmr, const mmc_t *memory);
  
void
  flash_prgmr__feed(flash_prgmr_t *prmgr, uint8_t byte);


/* Macros ----------------------------------------+--------+----------------- */




/* Inline Function Definitions ---------------------------------------------- */



#endif /* FLASH_PRGRM_H */