#pragma once
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR '/'
#endif

#define path_join(dst, path_a, path_b) sprintf(dst, "%s%c%s", path_a, PATH_SEPARATOR, path_b)

