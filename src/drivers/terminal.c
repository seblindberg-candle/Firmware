#include <drivers/terminal.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

static int
  term_getchar(FILE *stream);
  
static int
  term_putchar(char data, FILE *stream);

/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

void
terminal__ctor(terminal_t *terminal, USART_t *module)
{
  bool_t r = usart__is_read_enabled(terminal->module);
  bool_t w = usart__is_write_enabled(terminal->module);
  
  /* Setup the stream */
  if (r && w) {
    fdev_setup_stream(&terminal->stream, term_putchar, term_getchar, _FDEV_SETUP_RW);
  } else if (r) {
    fdev_setup_stream(&terminal->stream, NULL, term_getchar, _FDEV_SETUP_READ);
  } else if (w) {
    fdev_setup_stream(&terminal->stream, term_putchar, NULL, _FDEV_SETUP_WRITE);
  }
  
  fdev_set_udata(&terminal->stream, terminal);
    
  terminal->module = module;
}

void
terminal__attatch(terminal_t *terminal, FILE **in, FILE **out, FILE **err)
{
  if (in != NULL)  *in  = &terminal->stream;
  if (out != NULL) *out = &terminal->stream;
  if (err != NULL) *err = &terminal->stream;
}

int
term_putchar(char data, FILE *stream)
{
  terminal_t* terminal = fdev_get_udata(stream);

  usart__write_fast(terminal->module, data);

  if (data == '\n') {
    usart__write_fast(terminal->module, '\r');
  }

  return 0;
}

int
term_getchar(FILE *stream)
{
  terminal_t* terminal = fdev_get_udata(stream);

  return usart__read_fast(terminal->module);
}

