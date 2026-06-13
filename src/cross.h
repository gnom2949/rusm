#ifndef __rusm_cross__
#define __rusm_cross__

#if defined(_WIN32) || defined(_WIN64)
#define RUSM_PLAT_WINDOWS
#include <windows.h>
char* find_vs_linker();
#define strcasecmp _stricmp
static const char *pathtonasm = "C:/Program Files/NASM/nasm.exe";
static char *linker = NULL;
#else
  #define RUSM_PLAT_UNIX
  #include <unistd.h>
  static const char *pathtonasm = "/usr/bin/nasm";
  static const char *linker = "/usr/bin/ld";
#endif

typedef enum {
    FILE_TYPE_UNKNOWN,
    FILE_TYPE_OBJECT,
    FILE_TYPE_EXECUTABLE
} FileType;

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void get_temp_path(const char *input, char *output);
FileType check_file_type(const char *filename);
#endif /*__rusm_cross__*/