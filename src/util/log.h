#ifndef _LOG_H_
#define _LOG_H_

#include <stdbool.h>

enum log_type {LOG_ERROR = 0, LOG_FATAL = 1, LOG_WARNING = 2, LOG_INFO = 3};

/*
 * log_init - prepare log for writing.
 */
void
log_init();

void 
log_quit();

void
log_write(enum log_type t, const char *fmt, ...);

#endif
