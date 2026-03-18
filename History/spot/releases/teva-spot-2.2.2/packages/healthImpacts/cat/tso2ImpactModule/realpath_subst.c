#ifdef __WIN32__
#include <windows.h>
#include <limits.h>
char *realpath(const char *pathname, char resolved_path[PATH_MAX])
{
  int size = 0;
  size = GetFullPathNameA(pathname, PATH_MAX, resolved_path, 0);
  if (size > PATH_MAX)
    {
      return NULL;
    }
  else
    {
      return resolved_path;
    }
}
#endif

