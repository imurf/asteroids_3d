#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "log.h"
#include "util.h"
#include "defs.h"

static const char *prefix[4] = {"ERROR: ", "FATAL ERROR: ", "WARNING: ", "INFO: "};

static bool is_init;
static bool is_stderr;
static FILE *log_stream; /* file to output logs */
static const char *fpath;

void
log_init()
{
  char *path = join_path(DIR_LOG, FILENAME_LOG);
  log_stream = NULL;//fopen(path, "w");
  if(!log_stream)
  {
    fprintf(stderr, "error: failed to initialise log file '%s': ", path);
    perror("fopen");
    fprintf(stderr, "logging to stderr instead\n");
    log_stream = stderr;
    is_stderr = true;
  }
  is_init = true;
  free(path);
}

void
log_quit()
{
  if(is_init && !is_stderr)
  {
    fclose(log_stream);
  }
  is_init = false;
}

/*
 * bug fix: turns out that printing the 'va_list args' more than once causes one of the
 * prints to fail; 'va_list args' is seemingly 'consumed' in the printing.
 */
void
log_write(enum log_type type, const char *format, ...)
{
  assert(is_init);
  assert(LOG_ERROR <= type && type <= LOG_INFO);

  fprintf(log_stream, prefix[(int)type]);
  va_list args;
  va_start(args, format);
  vfprintf(log_stream, format, args);
  va_end(args);
  fprintf(log_stream, "\n");
}
