#include <compiler.h>
#include "minunit.h"

/* Globals ------------------------------------------------------------------ */

int tests_run = 0;

/* Tests -------------------------------------------------------------------- */

static char *
test_detector()
{
  mu_assert("detector tests not implemented", 0);
  return NULL;
}

static char *
test_parser()
{
  mu_assert("parser tests not implemented", 0);
  return NULL;
}

/* Test Setup --------------------------------------------------------------- */

static char *
all_tests()
{
  mu_run_test(test_detector);
  mu_run_test(test_parser);
  return 0;
}

int main(int argc, char *argv[])
{
  char *result = all_tests();
  if (result != 0) {
    printf("%s\n", result);
  }
  else {
    printf("ALL TESTS PASSED\n");
  }
  printf("Tests run: %d\n", tests_run);
  
  return result != 0;
}

/* Mocks -------------------------------------------------------------------- */

