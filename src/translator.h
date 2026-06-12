#ifndef __rusm_translator__
#define __rusm_translator__
#include <stdio.h>
typedef struct {
  const char *ru;
  const char *nasm;
} Translate;

extern Translate keywords[];

void parse_line(const char *line, FILE *out);
void clean_token(char *token);
const char *translate_token(const char *token, char *buffer);
int contains_cyrillic(const char *str);
#endif
