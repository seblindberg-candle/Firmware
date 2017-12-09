#ifndef COMPILER_H
#define COMPILER_H

/* Includes ----------------------------------------------------------------- */

#include <stdint.h>
#include <avr/io.h>

#define NDEBUG

#ifndef NDEBUG
#include <string.h>
#include <stdio.h>
#endif

//#define __ASSERT_USE_STDERR
#include <assert.h>


/* Data Types --------------------------------------------------------------- */

typedef uint_fast8_t bool_t;


/* Macros ---------------------------------------+---------+----------------- */

#define true                                      (1)
#define false                                     (0)

#define GLOBAL_INTERRUPTS_ENABLED                 (SREG & CPU_I_bm)

#define ENTER_CRITICAL_REGION()
#define LEAVE_CRITICAL_REGION()

#define WRITE_CONST(field, type, value)                     \
  *((type *) &(field)) = value

#define WRITE_TO_PROTECTED_IOREG()                          \
  do { CPU_CCP  = CCP_IOREG_gc; } while (0)

#define PACKED                                    __attribute__((__packed__))
#define UNUSED                                    __attribute__((__unused__))
#define DEPRICATED                                          \
  __attribute__((__deprecated__))
#define NONNULL                                   __attribute__((__nonnull__))
#define NONNULL_ARGS(...)                                   \
  __attribute__((__nonnull__ (__VA_ARGS__)))
#define PURE                                      __attribute__((__pure__))
#define CLEANUP(fn)                                         \
  __attribute__((__cleanup__(fn)))

#endif /* COMPILER_H */