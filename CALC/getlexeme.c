// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma warning(disable:4996)
#include "getlexeme.h"
#include "mathfunc.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE  1
#define FALSE 0

typedef struct
{
  union type
  {
    functoin_t Func;
    operation_t Oper;
    double value;
    char separator;
  }type;
  char* name;
}info_t;

static const info_t s_UNFUNCTION[] =
{
    { {.Func = sin }, "sin" },
    { {.Func = cos }, "cos" },
    { {.Func = tan }, "tg" },
    { {.Func = Cot }, "ctg" },
    { {.Func = asin }, "arcsin" },
    { {.Func = acos }, "arccos" },
    { {.Func = atan }, "arctg" },
    { {.Func = sqrt }, "sqrt" },
    { {.Func = log }, "ln" },
    { {.Func = Floor }, "floor" },
    { {.Func = Ceil }, "ceil" },
    { {.Func = NULL }, NULL }
};

static const info_t s_BIFUNCTION[] =
{
  { {.Oper = Log }, "log" },
  { {.Oper = NULL }, NULL }
};

static const info_t s_CONST[] =
{
    { {.value = 3.1415926535897932384626433832795 }, "pi" },
    { {.value = 2.7182818284590452353602874713527 }, "e" },
    { {.value = 0.0 }, NULL }
};

static const info_t s_OPERATIONS[] =
{
    { {.Oper = Add }, "+" },
    { {.Oper = Sub }, "-" },
    { {.Oper = Mul }, "*" },
    { {.Oper = Div }, "/" },
    { {.Oper = Pow }, "^" },
    { {.Oper = NULL }, NULL }
};

static const info_t s_SEPARATORS[] =
{
    { {.separator = '(' }, "(" },
    { {.separator = ')' }, ")" },
    { {.separator = ',' }, "," },
    { {.separator = ';' }, ";" },
    { {.separator = '=' }, "=" },
    //{ {.Oper = NULL }, ":=" },
    { {.separator = '\0' }, NULL }
};

int IsCharacter(const char ch)
{
  if ((ch >= '(' && ch <= '-') || ch == '/' ||
    ch == '^' || ch == '=' || ch == ';' || ch == ':')
    return TRUE;

  return FALSE;
}

/*
*Returns pointer to function which name exist in string segment.
*if error occurs returns NULL
*/
functoin_t GetUnFun(const char * startLex, const char* endLex)
{
  int i;
  for (i = 0; s_UNFUNCTION[i].name != NULL; i++)
  {
    if (Compare(startLex, endLex, s_UNFUNCTION[i].name))
      return s_UNFUNCTION[i].type.Func;
  }

  return NULL;
}

operation_t GetBiFun(const char * startLex, const char* endLex)
{
  int i;
  for (i = 0; s_BIFUNCTION[i].name != NULL; i++)
  {
    if (Compare(startLex, endLex, s_BIFUNCTION[i].name))
      return s_BIFUNCTION[i].type.Oper;
  }

  return NULL;
}


int Compare(const char* start, const char * end, const char* someStr)
{
  while (start <= end && *someStr == *start)
  {
    someStr++;
    start++;
  }
  if (start > end && *someStr == '\0')
    return TRUE;

  return FALSE;

}

lex_t WhatConst(const char* start, const char* end)
{
  lex_t lex = { LT_NULL };
  int i;
  for (i = 0; s_CONST[i].name != NULL; i++)
  {
    if (Compare(start, end, s_CONST[i].name))
    {
      lex.body.number = s_CONST[i].type.value;
      lex.priority = 10;
      lex.type = LT_NUMBER;
      return lex;
    }
  }
  return lex;
}

operation_t GetOperation(const char * start, const char* end)
{
  int i;
  for (i = 0; s_OPERATIONS[i].name != NULL; i++)
  {
    if (Compare(start, end, s_OPERATIONS[i].name))
      return s_OPERATIONS[i].type.Oper;
  }
  return NULL;
}

char GetSeparators(const char* start, const char* end)
{
  int i;
  for (i = 0; s_SEPARATORS[i].name != NULL; i++)
  {
    if (Compare(start, end, s_SEPARATORS[i].name))
      return s_SEPARATORS[i].type.separator;
  }
  return '\0';
}

double GetNumber(const char * start, const char** end, error_t* error)
{
  double num = 0;
  if (*start == '.' && !isdigit((unsigned char)*(start + 1)))
    LogError(error, "(GetNumber):", "invalid expression!");

  else
  {
    num = strtod(start, (char **)end);

    if (*end == start)
      while (**end != '\0')
        (*end)++;
  }
  return num;
}