#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "translator.h"

Translate keywords[] = {
  {"присвоить", "mov", 3},
  {"положить", "push", 3},
  {"взять", "pop", 3},
  {"вычесть", "sub", 3},
  {"сложить", "add", 3},
  {"прыгнуть", "jmp", 3},
  {"сравнить", "cmp", 3},
  {"если_равно", "je", 3},
  {"стоп", "hlt", 3},
  {"вернуть", "ret", 3},
  {"запретить_прерывания", "cli", 3},
  {"разрешить_прерывания", "sti", 3},
  {"позвать", "call", 3},
  {"присвоить_если", "cmov", 3},
  {"положить_всё", "pusha", 1},
  {"взять_всё", "popa", 1},
  {"зайти", "enter", 3},
  {"покинуть", "leave", 3},
  {"адрес", "lea", 3},
  {"подгрузить_дс", "lds", 1},
  {"подгрузить_ес", "les", 1},
  {"подгрузить_фс", "lfs", 3},
  {"подгрузить_гс", "lgs", 3},
  {"подгрузить_сс", "lss", 3},
  {"сложить_с_переносом", "adc", 3},
  {"вычесть_с_переносом", "sbb", 3},
  {"умножить", "mul", 3},
  {"умножить_со_знаком", "imul", 3},
  {"делить", "div", 3},
  {"делить_со_знаком", "idiv", 3},
  {"инкремент", "inc", 3},
  {"и", "and", 3},
  {"или", "or", 3},
  {"не", "not", 3},
  {"проверить", "test", 3},
  {"сдвиг_влево", "shl", 3},
  {"сдвиг_логич_вправо", "shr", 3},
  {"уменьшить", "dec", 3},
  {"отрицание", "neg", 3},
  {"арифм_сдвиг_вправо", "sar", 3},
  {"цикл_влево", "rol", 3},
  {"цикл_вправо", "ror", 3},
  {"цикл_через_перенос_влево", "rcl", 3},
  {"цикл_через_перенос_вправо", "rcr", 3},
  {"двойной_сдвиг_влево", "shld", 3},
  {"двойной_сдвиг_вправо", "shrd", 3},
  {"обменять", "xchg", 3},
  {"байт_в_слово", "cbw", 3},
  {"слово_в_двойное", "cwd", 3},
  {"двойное_в_четверное", "cdq", 3},
  {"четверное_в_десятерное", "cqo", 2},
  {"преобразовать_в_64", "cdqe", 2},
  {"преобразовать_в_32", "cwde", 3},
  {"поменять_порядок_байт", "bswap", 3},
  {"расширить_нулями", "movzx", 3},
  {"расширить_знаком", "movsx", 3},
  {"если_выше", "ja", 3},
  {"если_выше_или_равно", "jae", 3},
  {"если_ниже", "jb", 3},
  {"если_ниже_или_равно", "jbe", 3},
  {"если_больше", "jg", 3},
  {"если_больше_или_равно", "jge", 3},
  {"если_меньше", "jl", 3},
  {"если_меньше_или_равно", "jle", 3},
  {"если_не_равно", "jne", 3},
  {"если_переполнение", "jo", 3},
  {"если_не_переполнение", "jno", 3},
  {"если_знак", "js", 3},
  {"если_не_знак", "jns", 3},
  {"если_перенос", "jc", 3},
  {"если_не_перенос", "jnc", 3},
  {"если_четность", "jp", 3},
  {"если_не_четность", "jnp", 3},
  {"цикл", "loop", 3},
  {"цикл_пока_равно", "loope", 3},
  {"цикл_пока_не_равно", "loopne", 3},
  {"прыжок_если_cx_ноль", "jcxz", 3},
  {"прыжок_если_ecx_ноль", "jecxz", 3},
  {"прыжок_если_rcx_ноль", "jrcxz", 2},
  {"дальний_возврат", "retf", 3},
  {"установить_перенос", "stc", 3},
  {"сбросить_перенос", "clc", 3},
  {"инвертировать_перенос", "cmc", 3},
  {"установить_направление", "std", 3},
  {"сбросить_направление", "cld", 3},
  {"получить_флаги", "lahf", 3},
  {"установить_флаги", "sahf", 3},
  {"положить_флаги", "pushf", 3},
  {"взять_флаги", "popf", 3},
  {"скорректировать_сложение_bcd", "aaa", 1},
  {"скорректировать_вычитание_bcd", "aas", 1},
  {"скорректировать_умножение_bcd", "aam", 1},
  {"скорректировать_деление_bcd", "aad", 1},
  {"скорректировать_после_сложения", "daa", 1},
  {"скорректировать_после_вычитания", "das", 1},
  {"граница", "bound", 1},
  {"прервать", "int", 3},
  {"прервать_если_переполнение", "into", 1},
  {"точка_останова", "int3", 3},
  {"вернуться_из_прерывания", "iret", 1},
  {"вернуться_из_прерывания_64", "iretq", 2},
  {"вызвать_систему_64", "syscall", 3},
  {"вернуться_из_системы_64", "sysret", 3},
  {"вызвать_системное_прерывание", "int 0x80", 1},
  {"вызвать_систему_32", "sysenter", 1},
  {"вернуться_из_системы_32", "sysexit", 1},
  {"обменять_гс", "swapgs", 3},
  {"ожидание", "wait", 3},
  {"нет_операции", "nop", 3},
  {"заблокировать_шину", "lock", 3},
  {"пауза", "pause", 3},
  {"прочитать_счетчик_тактов", "rdtsc", 3},
  {"прочитать_маркер_времени", "rdtscp", 3},
  {"информация_о_процессоре", "cpuid", 3},
  {"очистить_кэш", "clflush", 3},
  {"предварительная_выборка", "prefetch", 3},
  {"инвалидировать_страницу", "invlpg", 3},
  {"записать_мзр", "wrmsr", 3},
  {"прочитать_мзр", "rdmsr", 3},
  {"сбросить_математический", "finit", 3},
  {"очистить_ммкс", "emms", 3},
  {"загрузить_мксиср", "ldmxcsr", 3},
  {"сохранить_мксиср", "stmxcsr", 3},
  {"забор_памяти", "sfence", 3},
  {"забор_памяти_легкий", "lfence", 3},
  {"забор_памяти_полный", "mfence", 3},
  {"секция", "section", 3},
  {"глобальный", "global", 3},
  {"разрядность", "bits", 3},
  {"четверное_слово", "dq", 3},
  {"двойное_слово", "dd", 3},
  {"внешний", "extern", 3},
  {"слово", "dw", 3},
  {"байт", "db", 3},
  {".текст", ".text", 3},
  {".данные", ".data", 3}
};

void parse_line(const char *line, FILE *out, int mode) {
    char token[256];
    char trans_buf[256];
    int t_idx = 0;
    int in_brackets = 0;
    int is_first_token = 1;

    if (line[0] == '\n' || line[0] == ' ' || line[0] == '\0') {
        fprintf(out, "\n");
        return;
    }

    for (int i = 0; line[i] != '\0'; i++) {
        char c = line[i];

        if (c == ';') {
            if (t_idx > 0) {
                token[t_idx] = '\0';
                clean_token(token);
                if (strlen(token) > 0) {
                    fprintf(out, is_first_token ? "    %s" : " %s", translate_token(token, trans_buf, mode));
                }
            }
            fprintf(out, " %s", &line[i]);
            return;
        }

        if (c == ':' && !in_brackets) {
            token[t_idx] = '\0';
            clean_token(token);
            fprintf(out, "%s:\n", translate_token(token, trans_buf, mode));
            return;
        }

        if (c == '[') in_brackets = 1;
        if (c == ']') in_brackets = 0;

        if ((c == ' ' || c == '\t' || c == ',') && !in_brackets) {
            if (t_idx > 0) {
                token[t_idx] = '\0';
                clean_token(token);

                if (strlen(token) > 0) {
                    const char *translated = translate_token(token, trans_buf, mode);
                    if (is_first_token) {
                        fprintf(out, "    %s", translated);
                        is_first_token = 0;
                    } else {
                        fprintf(out, " %s", translated);
                    }
                }
                t_idx = 0;
            }

            if (c == ',') {
                fprintf(out, ",");
            }
            continue;
        }
        token[t_idx++] = c;
        if (t_idx >= 255) t_idx = 255;
    }

    if (t_idx > 0) {
        token[t_idx] = '\0';
        clean_token(token);
        if (strlen(token) > 0) {
            const char *translated = translate_token(token, trans_buf, mode);
            fprintf(out, is_first_token ? "    %s\n" : " %s\n", translated);
        }
    } else {
        fprintf(out, "\n");
    }
}

static void transliterate(const char *src, char *dest)
{
  const char *cyr[] = {"а","б","в","г","д","е","ё","ж","з","и","й","к","л","м","н","о","п","р","с","т","у","ф","х","ц","ч","ш","щ","ъ","ы","ь","э","ю","я","_", "А","Б","В","Г","Д","Е","Ё","Ж","З","И","Й","К","Л","М","Н","О","П","Р","С","Т","У","Ф","Х","Ц","Ч","Ш","Щ","Ъ","Ы","Ь","Э","Ю","Я"};
  const char *lat[] = {"a","b","v","g","d","e","yo","zh","z","i","j","k","l","m","n","o","p","r","s","t","u","f","h","ts","ch","sh","shch","","y","","e","yu","ya","_", "A","B","V","G","D","E","Jo","Zh","Z","i","J","K","L","M","N","O","P","R","S","T","U","F","H","Ts","Ch","Sh","Shch","","Y","","E","Ju","Ja"};
  int cyr_count = sizeof(cyr) / sizeof(char*);

  dest[0] = '\0';

  while (*src)
  {
    int replaced = 0;

    if (((unsigned char)src[0] == 0xD0 || (unsigned char)src[0] == 0xD1) && src[1] != '\0')
    {
      char utf8_char[3] = {src[0], src[1], '\0'};

      for (int i = 0; i < cyr_count; i++) {
          if (strcmp(utf8_char, cyr[i]) == 0) {
              strcat(dest, lat[i]);
              replaced = 1;
              break;
          }
      }

      src += 2;
      if (replaced) continue;

      strcat(dest, "_");
      continue;
    }

    int len = strlen(dest);
    dest[len] = *src;
    dest[len + 1] = '\0';
    src++;
  }
}

const char *translate_token(const char *token, char *buffer, int current_mode)
{
  for (int i = 0; i < sizeof(keywords)/sizeof(Translate); i++)
  {
    if (strcmp(token, keywords[i].ru) == 0) {
      if ((keywords[i].mode & current_mode) == 0)
      {
        fprintf(stderr, "Rusm Error: Instruction '%s' is not supported in %d-bit mode\n", token, current_mode == 1 ? 32 : 64);
        exit(1);
      }
      return keywords[i].nasm;
    }
  }

  if (contains_cyrillic (token))
  {
    transliterate(token, buffer);
    return buffer;
  }

  return token;
}

void clean_token(char *token)
{
  int len = strlen(token);
  while (len > 0 && (token[len - 1] == ',' || token[len - 1] == ' ' || token[len - 1] == '\n' || token[len - 1] == ' ' || token[len - 1] == '\t'))
  {
    token[len - 1] = '\0';
    len--;
  }
  int start = 0;
  while (token[start] == ' ' || token[start] == '\t' || token[start] == ',')
  {
      start++;
  }
  if (start > 0) {
      memmove(token, token + start, strlen(token) - start + 1);
  }
}

int contains_cyrillic(const char *str)
{
  while(*str)
    {
      if ((unsigned char)*str > 127)
      {
        return 1;
      }
      str++;
    }
  return 0;
}
