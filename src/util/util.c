#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "log.h"

/**
 * join_path - utility to join the directory path to the file name.
 *
 * note: mallocs bytes for the result; must call free on returned string!
 */
char *
join_path(const char *path, const char *file)
{
  int len1 = strlen(path), len2 = strlen(file);
  char* full_path = malloc(len1 + len2 + 2); /* + 2 to include terminating null bytes */
  if(full_path == NULL)
    return NULL;
  strncpy(full_path, path, len1);
  full_path[len1] = '/';
  strncpy(full_path + len1 + 1, file, len2);
  full_path[len1 + len2 + 1] = '\0';
  return full_path;
}

/**
 * search_file_dir - searches the directory 'path' for a file
 *   named 'file'.
 *
 * returns - 0 on success, i.e. found file and non-zero on error; errors
 *   logged to log file.
 */
int
search_file_in_dir(const char *path, const char *file)
{
  struct dirent *entry;
  int ret;
  DIR *dir;
  dir = opendir(path);
  errno = 0;
  while((entry = readdir(dir)) != NULL)
  {
    if(strcmp(entry->d_name, file) == 0)
    {
      ret = 0;
      break;
    }
  }
  if(errno && !entry)
    log_write(LOG_ERROR, "readir fail: %s\n", strerror(errno));
  closedir(dir);
  return ret;
}





