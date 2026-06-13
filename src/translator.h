#ifndef __rusm_translator__
#define __rusm_translator__
#include <stdio.h>
typedef struct {
  const char *ru;
  const char *nasm;
  int mode; // 1 - 32bit, 2 - 64bit, 3 - both
} Translate;

extern Translate keywords[];

void parse_line(const char *line, FILE *out, int mode);
void clean_token(char *token);
const char *translate_token(const char *token, char *buffer, int current_mode);
int contains_cyrillic(const char *str);
#endif
