#include "translator.h"
#include "cross.h"

void help(void)
{
  printf("\033[35mRusm\033[0m the russian assembly\n");
  printf("\033[36mOptions\033[0m:\n");
  printf("\t-o/--output\tSpecify output file\n");
  printf("\t-f/--format\tSpecify output assembly format\n");
  printf("\t-v/--version\tView version of rusm\n");
  printf("\t-i/--info\tLearn info about rusm\n");
  printf("\t-h/--help\tShow this help\n\n");
}

void info(void)
{
  printf("The \033[35mrusm\033[0m, an Russian Assembly\n");
  printf("Licensed under MIT License\n");
  printf("Copyright(c)2026-present Alexander Silaev\n");
}

void supported_formats(void)
{
  printf("Rusm's supported formats:\n");
  printf("\t- \033[32mUnix\033[0m\tELF32\n");
  printf("\t- \033[32mUnix\033[0m\tELF64\n");
  printf("\t- \033[33mMicrosoft\033[0m\tWIN32\n");
  printf("\t- \033[33mMicrosoft\033[0m\tWIN64\n");
  printf("\t- \033[34mApple\033[0m\tMACHO64\n");
  printf("\t- \033[35mBinary\033[0m\n\n");
}

int main(int argc, char **argv)
{
  char *input_file = NULL;
  char *output_file = "output.o";
  char *format = "";
  const char *_valid_formats[] = {"elf32", "elf64", "win32", "win64", "macho64", "bin"};
  int is_valid = 0;
  char tempfile[256];

  if (argc < 2)
  {
    printf ("Usage: %s <input.rusm> [-o output] [-f format]\n", argv[0]);
    return 1;
  }

  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) { help(); return 0; }
    else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--info") == 0) { info(); return 0; }
    else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) { printf("\033[35mRusm\033[0m v0.1.1\n"); return 0; }
    else if (strcmp(argv[i], "-sF") == 0 || strcmp(argv[i], "--supported-formats") == 0) { supported_formats(); return 0; }
    else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) { if (i + 1 < argc) output_file = argv[++i]; }
    else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--format") == 0) { if (i + 1 < argc) format = argv[++i]; }
    else
    {
      if (argv[i][0] != '-') input_file = argv[i];
    }
  }

  if (!input_file)
  {
    fprintf(stderr, "Rusm \033[31m[Error\033[0m: input file not specified\n");
    return 1;
  }

  if (strlen(format) == 0) {
#if defined(RUSM_PLAT_WINDOWS)
    format = "win64";
#elif defined(__linux__) || defined(__unix__)
    format = "elf64";
#elif defined(__APPLE__)
    format = "macho64";
#else
    format = "binary";
#endif
  }

  for (int java = 0; java < sizeof(_valid_formats)/sizeof(char*); java++)
  {
    if (strcmp(format, _valid_formats[java]) == 0) { is_valid = 1; break; }
  }

  if (!is_valid) { fprintf(stderr, "Rusm \033[31mError\033[0m: Unsupported format '%s'. See -sF to supported formats\n", format); return 1;}
  
  get_temp_path(input_file, tempfile);
  FILE *in = fopen(input_file, "r");
  FILE *temp = fopen(tempfile, "w");
  if (!in)
  { fprintf (stderr, "Rusm \033[31mError\033[0m: cannot open input file\n"); return 1;}
  if (!temp) { fprintf(stderr, "Rusm \033[31mError\033[0m: cannot create temporary file!\n"); return 1; }
  
  char ln[1024];
  int mode = 1;
  if (strcmp(format, "elf64") == 0 || strcmp(format, "win64") == 0 || strcmp(format, "macho64") == 0) { mode = 2; }
  
  while (fgets(ln, sizeof(ln), in))
  {
    parse_line(ln, temp, mode);
  }
  fclose(in);
  fclose(temp);

  int is_nasm_available = 0;
  char check_cmd[256];
#if defined(RUSM_PLAT_WINDOWS)
  snprintf(check_cmd, sizeof(check_cmd), "where \"%s\" >nul 2>nul", pathtonasm);
  if (system(check_cmd) != 0) {
      if (system("where nasm >nul 2>nul") == 0) { pathtonasm = "nasm"; is_nasm_available = 1; }
  } else { is_nasm_available = 1; }
#else
  snprintf(check_cmd, sizeof(check_cmd), "which \"%s\" >/dev/null 2>&1", pathtonasm);
  if (system(check_cmd) != 0) {
      if (system("which nasm >/dev/null 2>&1") == 0) { pathtonasm = "nasm"; is_nasm_available = 1; }
  } else { is_nasm_available = 1; }
#endif

  if (!is_nasm_available) { 
      fprintf (stderr, "Rusm Error: cannot find nasm, please install nasm or add it to PATH before using rusm.\n"); 
      return 1;
  }

  int need_linking = 0;
  if (strcmp(format, "bin") != 0) {
      FileType out_type = check_file_type(output_file);
      if (out_type == FILE_TYPE_EXECUTABLE) {
          need_linking = 1;
      }
  }

  char fulled[512];
  char intermediate_obj[256];
  
  if (need_linking) {
#if defined(RUSM_PLAT_WINDOWS)
      snprintf(intermediate_obj, sizeof(intermediate_obj), "%s.obj", tempfile);
#else
      snprintf(intermediate_obj, sizeof(intermediate_obj), "%s.o", tempfile);
#endif
      snprintf(fulled, sizeof(fulled), "%s -f %s \"%s\" -o \"%s\"", pathtonasm, format, tempfile, intermediate_obj);
  } else {
      snprintf(fulled, sizeof(fulled), "%s -f %s \"%s\" -o \"%s\"", pathtonasm, format, tempfile, output_file);
  }

  if (system(fulled) != 0) {
      fprintf(stderr, "Rusm Error: NASM compilation failed\n");
      return 1;
  }

  if (need_linking) {
      char link_cmd[512];
#if defined(RUSM_PLAT_WINDOWS)
      char *win_linker = find_vs_linker();
      if (!win_linker) win_linker = "link.exe"; 
      
      snprintf(link_cmd, sizeof(link_cmd), "\"%s\" /subsystem:console /entry:main /out:\"%s\" \"%s\"", win_linker, output_file, intermediate_obj);
#else
      if (!linker) linker = "ld";
      snprintf(link_cmd, sizeof(link_cmd), "%s -o \"%s\" \"%s\"", linker, output_file, intermediate_obj);
#endif

      if (system(link_cmd) != 0) {
          fprintf(stderr, "Rusm \033[31mError\033[0m: Linking failed\n");
          remove(intermediate_obj);
          return 1;
      }
      remove(intermediate_obj);
  }

#ifndef __rusm_keep_temp__
  if (strlen(tempfile) > 0) remove(tempfile);
#endif

  return 0;
}