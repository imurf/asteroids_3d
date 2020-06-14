#ifndef _UTIL_H_
#define _UTIL_H_

#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

/**
 * join_path - utility to join the directory path to the file name.
 *
 * note: mallocs bytes for the result; must call free on returned string!
 */
char *
join_path(const char *path, const char *file);

#endif
