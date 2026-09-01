#include <debug.h>
#include <stdarg.h>
#include <stdio.h>

int logString(const char *format, ...) {
  static char buf[1024];
  va_list args;
  int size;

  va_start(args, format);
  size = vsnprintf(buf, 1024, format, args);
  va_end(args);

  scr_printf(buf, size);
  printf(buf, size);

  return size;
}
