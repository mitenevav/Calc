// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma warning (disable:4996)
#include "linehanding.h"
#include "box.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <crtdbg.h>
#include <locale.h>

#define TRUE    1
#define FALSE   0

int IsExpression(const char* str)
{
  int l, i, s = 0, space = 0, comment = 0;
  l = strlen(str);

  for (i = 0; i < l; i++)
  {
    if (isspace((unsigned char)str[i]))
      space++;
  }

  for (i = 0; i < l; i++)
  {
    if (isspace((unsigned char)str[i]))
      s++;

    if (str[i] == '/' && str[i + 1] == '/' && i == s)
      comment = -1;
  }

  if (s == l || comment == -1)
    return FALSE;

  return TRUE;
}

void Dispatcher(FILE* stream)
{
  int eoff = FALSE;
  char* str = NULL;
  box_t expression;
  error_t error = { FALSE };
  double result = 0.0;

  do
  {
    str = StrRead(stream, &eoff);

    if (str != NULL)
    {

      if (strlen(str) || !eoff)/*первый был не EOF?*/
      {
        if (IsExpression(str))
        {
          BoxInitialization(&expression);
          result = Calculate(str, &expression, &error);

          if (!error.exist)
            PrintExpression(str, result);
          else
          {
            PrintError(str, &error);
            error.exist = 0;
          }

          BoxDestroy(&expression);
        }
        else
          PrintNoExpression(str);
      }

      free(str);
    }
  } while (eoff != 1);
}
//FILE* WhatInStream(const int argc, char const* argv[], int* reaction) 

int main(int argc, char const * argv[])
{
  FILE* stream;

  setlocale(LC_CTYPE, "Russian");

  if (argc == 1)
  {
    Dispatcher(stdin);
  }

  else if (argc == 2)
  {
    stream = fopen(argv[1], "r");
    if (stream != 0)
    {
      Dispatcher(stream);
      fclose(stream);
    }

    else
    {
      PrintFatalError("ERROR: file not found");
      return 1;
    }
  }

  else
  {
    PrintFatalError("ERROR: incorrect number of arguments");
    return 1;
  }

  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
  _CrtDumpMemoryLeaks();

  return 0;
}