#include <util/terminal.h>

/* Macros ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */



/* Private Functions –––––––––––––––––––––––––––––––––––––––––––––––––––––––– */

#if TERMINAL__SUPPORT_STREAM
static int
  term_getchar(FILE *stream);
  
static int
  term_putchar(char data, FILE *stream);
#endif

/* Global Variables ––––––––––––––––––––––––––––––––––––––––––––––––––––––––– */




/* Function Definitions ––––––––––––––––––––––––––––––––––––––––––––––––––––– */

/* Constructor
 *
 * Initializes a buffered usart and connects it to a stream that can then be
 * interfaced with by the standard library (printf-family).
 *
 * The USART device given should be configured, either for both reading and
 * writing or for any one of them.
 */
void
terminal__ctor(terminal_t *terminal, USART_t *device)
{
  s_list__ctor(&terminal->cmd_list);
  terminal->cmd_active = NULL;
  
#if TERMINAL__SUPPORT_STREAM
  bool_t r = usart__device__is_read_enabled(device);
  bool_t w = usart__device__is_write_enabled(device);
  
  /* Setup the stream */
  if (r && w) {
    fdev_setup_stream(&terminal->stream, term_putchar, term_getchar,
                      _FDEV_SETUP_RW);
  } else if (r) {
    fdev_setup_stream(&terminal->stream, NULL, term_getchar, _FDEV_SETUP_READ);
  } else if (w) {
    fdev_setup_stream(&terminal->stream, term_putchar, NULL, _FDEV_SETUP_WRITE);
  }
  
  fdev_set_udata(&terminal->stream, &terminal->usart);
#endif
    
  usart__ctor(&terminal->usart, device, terminal->buffer,
    TERMINAL__BUFFER_SIZE);
}

/* Attach
 *
 * Connect the internal terminal stream to the provided in, out and error
 * streams.
 */
#if TERMINAL__SUPPORT_STREAM
void
terminal__attatch(terminal_t *terminal, FILE **in, FILE **out, FILE **err)
{
  if (in != NULL)  *in  = &terminal->stream;
  if (out != NULL) *out = &terminal->stream;
  if (err != NULL) *err = &terminal->stream;
}
#endif

void
terminal__spin_once(terminal_t *terminal)
{
  uint8_t c;
  terminal__cmd_t *cmd;
  
  /* Do nothing if there are no commands registered */
  if (s_list__is_empty(&terminal->cmd_list)) {
    return;
  }
  
  if (terminal__next(terminal, &c) == false) {
    return;
  }
  
  if (terminal->cmd_active != NULL) {
    if (terminal__cmd__parse(terminal->cmd_active, c) == false) {
      terminal->cmd_active = NULL;
    }
    
    return;
  }
  
  /* Try to find a command */
  cmd = (terminal__cmd_t *) s_list__first(&terminal->cmd_list);
  
  do {
    if (terminal__cmd__detect(cmd, c)) {
      terminal->cmd_active = cmd;
      break;
    }
    cmd = terminal__cmd__next(cmd);
  } while (cmd != NULL);
}

/* Puts
 *
 * More effient than calling the stdio function puts.
 */
void
terminal__puts(terminal_t *terminal, const char *str)
{
  size_t str_len = 0;
  const char *ptr = str;
  
  while (*(ptr++) != '\0') {
    ++str_len;
  }
  
  usart__write(&terminal->usart, str, str_len);
}

/* Term Putchar
 *
 * Private function used by the terminal stream.
 */
#if TERMINAL__SUPPORT_STREAM
int
term_putchar(char data, FILE *stream)
{
  usart_t *usart = fdev_get_udata(stream);

  usart__write_fast(usart, data);

  if (data == '\n') {
    usart__write_fast(usart, '\r');
  }

  return 0;
}
#endif

/* Term Getchar
 *
 * Private function used by the terminal stream.
 */
#if TERMINAL__SUPPORT_STREAM
int
term_getchar(FILE *stream)
{
  usart_t* usart = fdev_get_udata(stream);

  return usart__read_fast(usart);
}
#endif

