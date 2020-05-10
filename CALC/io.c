// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "io.h"
#include "linehanding.h"
#include <stdio.h>
#include <stdlib.h>
#define SIZEDM  256
#define TRUE    1
#define FALSE   0

char* StrRead(FILE* f, int*eoff)
{
  int i = 0;
  char *dopstr = NULL;
  char *str = NULL;
  int sym = 0;

  str = (char*)malloc(SIZEDM * sizeof(char));

  if (str == NULL)
  {
    while (((sym = fgetc(f)) != '\n') && sym != EOF);
    if (sym == EOF)
      (*eoff) = TRUE;
    else
      PrintFatalError("ERROR: memory is over");
    return NULL;
  }

  else
  {
    dopstr = str;
    do
    {
      str[i] = (char)fgetc(f);
      i++;
      if (i % SIZEDM == 0)
      {
        dopstr = (char*)realloc(str, i + SIZEDM);
        if (dopstr == NULL)
        {
          PrintFatalError("ERROR: memory is over");
          if (str[i - 1] != '\n')
            while (((sym = fgetc(f)) != '\n') && sym != EOF);
          if (str[i - 1] == (char)EOF || sym == EOF)
            (*eoff) = TRUE;
          free(str);
          return NULL;
        }

        else
        {
          str = dopstr;
        }
      }
    } while (str[i - 1] != '\n' && str[i - 1] != (char)EOF);

    if (str[i - 1] == (char)EOF)
      (*eoff) = TRUE;
    str[i - 1] = '\0';
  }

  return str;
}

void PrintNoExpression(char const * str)
{
  printf("%s\n", str);
}

void PrintExpression(char const * str, double result)
{
  printf("%s == %g\n", str, result);
}

void PrintError(char const * str, error_t* error)
{
  printf("%s == ERROR: %s %s\n", str, error->id, error->message);
}

void PrintFatalError(char const *str)
{
  printf("%s\n", str);
}