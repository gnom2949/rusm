#ifndef __rusm_cross__
#define __rusm_cross__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum {
    FILE_TYPE_UNKNOWN,
    FILE_TYPE_OBJECT,
    FILE_TYPE_EXECUTABLE
} FileType;

#if defined(_WIN32) || defined(_WIN64)
#define RUSM_PLAT_WINDOWS
#include <windows.h>
void init_paths(void);
#define strcasecmp _stricmp
#define _(String) (String)
#define N_(String) String
#define bindtextdomain(Domain, Directory)
#define textdomain(Domain)
#define bind_textdomain_codeset(Name, Codeset)
extern char pathtonasm[MAX_PATH];
extern char pathtolinker[MAX_PATH];
extern char *linker;
#else
  #include <limits.h>
  #ifndef MAX_PATH
  #define MAX_PATH PATH_MAX
  #endif
  #define _(String) gettext (String)
  #define N_(String) String
  #include <libintl.h>
  #define RUSM_PLAT_UNIX
  #include <unistd.h>
  extern const char *pathtonasm;
  extern const char *linker;
#endif

void get_temp_path(const char *input, char *output);
FileType check_file_type(const char *filename);
#endif /*__rusm_cross__*/
