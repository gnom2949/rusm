#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "translator.h"

Translate keywords[] = {
  {"присвоить", "mov"},
  {"положить", "push"},
  {"взять", "pop"},
  {"вычесть", "sub"},
  {"сложить", "add"},
  {"прыгнуть", "jmp"},
  {"сравнить", "cmp"},
  {"если_равно", "je"},
  {"стоп", "hlt"},
  {"вернуть", "ret"},
  {"запретить_прерывания", "cli"},
  {"разрешить_прерывания", "sti"},
  {"позвать", "call"},
  {"присвоить_если", "cmov"},
  {"положить_всё", "pusha"},
  {"взять_всё", "popa"},
  {"зайти", "enter"},
  {"покинуть", "leave"},
  {"адрес", "lea"},
  {"подгрузить_дс", "lds"},
  {"подгрузить_ес", "les"},
  {"подгрузить_фс", "lfs"},
  {"подгрузить_гс", "lgs"},
  {"подгрузить_сс", "lss"},
  {"сложить_с_переносом", "adc"},
  {"вычесть_с_переносом", "sbb"},
  {"умножить", "mul"},
  {"умножить_со_знаком", "imul"},
  {"делить", "div"},
  {"делить_со_знаком", "idiv"},
  {"инкремент", "inc"},
  {"и", "and"},
  {"или", "or"},
  {"не", "not"},
  {"проверить", "test"},
  {"сдвиг_влево", "shl"},
  {"сдвиг_логич_вправо", "shr"},
  {"уменьшить", "dec"},
  {"отрицание", "neg"},
  {"арифм_сдвиг_вправо", "sar"},
  {"цикл_влево", "rol"},
  {"цикл_вправо", "ror"},
  {"цикл_через_перенос_влево", "rcl"},
  {"цикл_через_перенос_вправо", "rcr"},
  {"двойной_сдвиг_влево", "shld"},
  {"двойной_сдвиг_вправо", "shrd"},
  {"обменять", "xchg"},
  {"байт_в_слово", "cbw"},
  {"слово_в_двойное", "cwd"},
  {"двойное_в_четверное", "cdq"},
  {"четверное_в_десятерное", "cqo"},
  {"преобразовать_в_64", "cdqe"},
  {"преобразовать_в_32", "cwde"},
  {"поменять_порядок_байт", "bswap"},
  {"расширить_нулями", "movzx"},
  {"расширить_знаком", "movsx"},
  {"если_выше", "ja"},
  {"если_выше_или_равно", "jae"},
  {"если_ниже", "jb"},
  {"если_ниже_или_равно", "jbe"},
  {"если_больше", "jg"},
  {"если_больше_или_равно", "jge"},
  {"если_меньше", "jl"},
  {"если_меньше_или_равно", "jle"},
  {"если_не_равно", "jne"},
  {"если_переполнение", "jo"},
  {"если_не_переполнение", "jno"},
  {"если_знак", "js"},
  {"если_не_знак", "jns"},
  {"если_перенос", "jc"},
  {"если_не_перенос", "jnc"},
  {"если_четность", "jp"},
  {"если_не_четность", "jnp"},
  {"цикл", "loop"},
  {"цикл_пока_равно", "loope"},
  {"цикл_пока_не_равно", "loopne"},
  {"прыжок_если_cx_ноль", "jcxz"},
  {"прыжок_если_ecx_ноль", "jecxz"},
  {"дальний_возврат", "retf"},
  {"установить_перенос", "stc"},
  {"сбросить_перенос", "clc"},
  {"инвертировать_перенос", "cmc"},
  {"установить_направление", "std"},
  {"сбросить_направление", "cld"},
  {"получить_флаги", "lahf"},
  {"установить_флаги", "sahf"},
  {"положить_флаги", "pushf"},
  {"взять_флаги", "popf"},
  {"скорректировать_сложение_bcd", "aaa"},
  {"скорректировать_вычитание_bcd", "aas"},
  {"скорректировать_умножение_bcd", "aam"},
  {"скорректировать_деление_bcd", "aad"},
  {"скорректировать_после_сложения", "daa"},
  {"скорректировать_после_вычитания", "das"},
  {"граница", "bound"},
  {"прервать", "int"},
  {"прервать_если_переполнение", "into"},
  {"точка_останова", "int3"},
  {"вернуться_из_прерывания", "iret"},
  {"вызвать_систему_64", "syscall"},
  {"вернуться_из_системы_64", "sysret"},
  {"вызвать_системное_прерывание", "int 0x80"},
  {"вызвать_систему_32", "sysenter"},
  {"вернуться_из_системы_32", "sysexit"},
  {"ожидание", "wait"},
  {"нет_операции", "nop"},
  {"заблокировать_шину", "lock"},
  {"пауза", "pause"},
  {"прочитать_счетчик_тактов", "rdtsc"},
  {"прочитать_маркер_времени", "rdtscp"},
  {"информация_о_процессоре", "cpuid"},
  {"очистить_кэш", "clflush"},
  {"предварительная_выборка", "prefetch"},
  {"инвалидировать_страницу", "invlpg"},
  {"записать_мзр", "wrmsr"},
  {"прочитать_мзр", "rdmsr"},
  {"сбросить_математический", "finit"},
  {"очистить_mmx", "emms"},
  {"загрузить_mxcsr", "ldmxcsr"},
  {"сохранить_mxcsr", "stmxcsr"},
  {"забор_памяти", "sfence"},
  {"забор_памяти_легкий", "lfence"},
  {"забор_памяти_полный", "mfence"},
  {"секция", "section"},
  {"глобальный", "global"},
  {"разрядность", "bits"},
  {"двойное_слово", "dd"},
  {"внешний", "extern"},
  {"слово", "dw"},
  {"байт", "db"}
};

void clean_token(char *token);
const char *translate_token(const char *token, char *buffer);

void parse_line(const char *line, FILE *out) {
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
                    fprintf(out, is_first_token ? "    %s" : " %s", translate_token(token, trans_buf));
                }
            }
            fprintf(out, " %s", &line[i]);
            return;
        }

        if (c == ':' && !in_brackets) {
            token[t_idx] = '\0';
            clean_token(token);
            fprintf(out, "%s:\n", translate_token(token, trans_buf));
            return;
        }

        if (c == '[') in_brackets = 1;
        if (c == ']') in_brackets = 0;

        if ((c == ' ' || c == '\t' || c == ',') && !in_brackets) {
            if (t_idx > 0) {
                token[t_idx] = '\0';
                clean_token(token);

                if (strlen(token) > 0) {
                    const char *translated = translate_token(token, trans_buf);
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
            const char *translated = translate_token(token, trans_buf);
            fprintf(out, is_first_token ? "    %s\n" : " %s\n", translated);
        }
    } else {
        fprintf(out, "\n");
    }
}

static void transliterate(const char *src, char *dest)
{
  const char *cyr[] = {"а","б","в","г","д","е","ё","ж","з","и","й","к","л","м","н","о","п","р","с","т","у","ф","х","ц","ч","ш","щ","ъ","ы","ь","э","ю","я","_", "А","Б","В","Г","Д","Е","Ё","Ж","З","И","Й","К","Л","М","Н","О","П","Р","С","Т","У","Ф","Х","Ц","Ч","Ш","Щ","Ъ","Ы","Ь","Э","Ю","Я"};
  const char *lat[] = {"a","b","v","g","d","e","jo","zh","z","i","j","k","l","m","n","o","p","r","s","t","u","f","h","ts","ch","sh","shch","","y","","e","ju","ja","_", "A","B","V","G","D","E","Jo","Zh","Z","i","J","K","L","M","N","O","P","R","S","T","U","F","H","Ts","Ch","Sh","Shch","","Y","","E","Ju","Ja"};
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

const char *translate_token(const char *token, char *buffer)
{
  for (int i = 0; i < sizeof(keywords)/sizeof(Translate); i++)
  {
    if (strcmp(token, keywords[i].ru) == 0) return keywords[i].nasm;
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
