#include "translator.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <libgen.h>
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
  printf("\t- \033[32mUnix\033[0m ELF32\n\n");
}

void get_temp_path(const char *input, char *output)
{
  char path_copy[256];
  strncpy(path_copy, input, sizeof(path_copy) - 1);

  char *filename = basename(path_copy);
  char *dot = strrchr (filename, '.');
  if (dot) *dot = '\0';

  snprintf(output, 256, "/tmp/%s.asm", filename);
}

int main(int argc, char **argv)
{
  char *input_file = NULL;
  char *output_file = "output.o";
  char *format = "elf32";
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
    else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) { printf("\033[35mRusm\033[0m v0.1"); return 0; }
    else if (strcmp(argv[i], "-sF") == 0 || strcmp(argv[i], "--supported-formats") == 0) { supported_formats(); return 0; }
    else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) { if (i + 1 < argc) output_file = argv[++i]; }
    else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--format") == 0) {
             if (i + 1 < argc)
             {
                format = argv[++i];
                if (strcmp(format, "elf32") != 0) { fprintf(stderr, "Rusm error: Unsupported format '%s', see the -sF\n", format); return 1; }
             }
    }
    else
    {
      if (argv[i][0] != '-') input_file = argv[i];
    }
  }

  if (!input_file)
  {
    fprintf(stderr, "Rusm Error: input file not specified\n");
    return 1;
  }
  get_temp_path(input_file, tempfile);
  FILE *in = fopen(input_file, "r");
  FILE *temp = fopen(tempfile, "w");
  if (!in)
  { printf ("Rusm Error: cannot open input file\n"); return 1;}
  if (!temp) { printf("Rusm Error: cannot create temporary file!\n"); return 1; }
  char ln[1024];
  while (fgets(ln, sizeof(ln), in))
  {
    parse_line(ln, temp);
  }
  fclose(in);
  fclose(temp);
  int is_nasm_available = 0; // no
  FILE *nasm = fopen("/usr/bin/nasm", "r");
  if (!nasm) { printf ("Rusm Error: cannot find nasm, please install nasm before using rusm.\n"); return 1;}
  if (nasm)
  {
    is_nasm_available = 1; // yes
    fclose(nasm);
  }
  if (is_nasm_available)
  {
    char fulled[128];
    snprintf(fulled, sizeof(fulled), "nasm -f elf32 %s -o %s", tempfile, output_file);
    system(fulled);
  }
#ifndef __rusm_keep_temp__
  if (temp && strlen(tempfile) > 0) remove(tempfile);
#endif
}
