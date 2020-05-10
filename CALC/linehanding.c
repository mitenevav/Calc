// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma warning(disable:4996)
#include "linehanding.h"
#include "getlexeme.h"
#include "box.h"
#include "mathfunc.h"
#include <ctype.h>
#include <math.h>
#include <string.h>
#define TRUE  1
#define FALSE 0
const double g_PI = 3.1415926535897932384626433832795;

void s_procFinalSymbols(lex_t* stackLast, box_t* expression, error_t* error, box_t* stack);
void s_moveStackToExp(lex_t* stackLast, lex_t lex, box_t* stack, box_t* expression, error_t* error);

/*Функция вычисляет выражение из строки методом польской нотации и возвращает результат*/
double Calculate(const char * str, box_t* expression, error_t * error)
{
  double result;
  CreationPolNonation(str, expression, error);
  if (error->exist)
    return 0.0;
  result = Compute(expression, error);
  if (error->exist)
    return 0.0;
  return result;
}

/*Функция преобразует строку к виду обратной польской записи*/
void CreationPolNonation(char const * str, box_t* expression, error_t * error)
{
  box_t stack;
  lex_t lex;
  lex_t auxiliaryLex = { LT_NULL };
  lex_t stackLast;
  char const * startLex = NULL;
  int lenght;
  lex_t beLast = { LT_NULL };
  startLex = str;

  BoxInitialization(&stack);

  do
  {
    lex = GetLex(startLex, &lenght, error);
    if (error->exist)
      continue;
    /*проверка на правильную последовательность, а именно, что перед числом(переменной), "(", унарной и бинароной функиями находится число или ")"*/
    if ((lex.type == LT_NUMBER || lex.type == LT_VARIABLE || (lex.type == LT_SEPARARTORS &&
      lex.body.separators == '(') || lex.type == LT_UNFUNCTION || lex.type == LT_BIFUNCTION) && (beLast.type == LT_NUMBER ||
        beLast.type == LT_VARIABLE || (beLast.type == LT_SEPARARTORS && beLast.body.separators == ')')))/*проверка на правильность строки*/
    {
      LogError(error, "(CreationPolNonation):", "incorrect expression!");
      continue;
    }

    if (lex.type == LT_NUMBER || lex.type == LT_VARIABLE)
    {

      BoxAddBack(expression, &lex, error);
      if (error->exist)
        continue;
    }

    else if (lex.type != LT_NULL)
    {
      if (BoxEmpty(expression) && ((lex.type == LT_OPERATION && lex.body.Operation != Sub) ||
        (lex.type == LT_SEPARARTORS && lex.body.separators == ')')))  /*проверяем на первый элемент*/
      {
        LogError(error, "(CreationPolNonation):", "invalid first sybol!");
        continue;
      }
      else
      {
        /*унарный минус*/
        if (lex.body.Operation == Sub && (beLast.type == LT_OPERATION ||
          beLast.body.separators == '(' || BoxEmpty(expression) ||
          beLast.body.separators == '='))
        {
          lex.priority = 13;
          BoxAddBack(&stack, &lex, error);
          auxiliaryLex.type = LT_NUMBER;
          auxiliaryLex.priority = 10;
          auxiliaryLex.body.number = 0.0;
          BoxAddBack(expression, &auxiliaryLex, error);
          if (error->exist)
            continue;
        }

        else if (lex.type == LT_SEPARARTORS && (lex.body.separators == ')' || lex.body.separators == ','))/*закрвающаяся скобка или запятая(для логарифма)*/
        {
          stackLast = BoxSeeLast(&stack);

          s_procFinalSymbols(&stackLast, expression, error, &stack);

          if (error->exist)
            continue;

          if (stackLast.body.separators == '(' && lex.body.separators == ')')/*если не ")" то убиваем и "("*/
          {
            auxiliaryLex = BoxGetBack(&stack);
          }
        }
        else
        {
          stackLast = BoxSeeLast(&stack);
          if (stackLast.priority < lex.priority || (stackLast.priority == 14 &&
            lex.priority == 14) || (stackLast.priority == 2 &&
              lex.priority == 2) || lex.body.separators == '(') /*сравнение приоритетов и правая ассоц у степени и "="*/
          {
            BoxAddBack(&stack, &lex, error);
            if (error->exist)
              continue;
          }

          else
          {
            s_moveStackToExp(&stackLast, lex, &stack, expression, error);

            if (error->exist)
              continue;
          }
        }
      }
    }
    beLast = lex;
    startLex += lenght;
  } while (lex.type != LT_NULL && *startLex != '\0' && !error->exist);

  while (!BoxEmpty(&stack) && !error->exist)
  {
    auxiliaryLex = BoxGetBack(&stack);
    if (auxiliaryLex.type == LT_SEPARARTORS && auxiliaryLex.body.separators == '(')
      LogError(error, "(CreationPolNotation):", "inconsistency of the brackets!");
    BoxAddBack(expression, &auxiliaryLex, error);
  }

  BoxDestroy(&stack);
}

/*получая указатель на начало лексемы, определяет ее и возвращает вместе с длинной*/
lex_t GetLex(const char * startLex, int* lenght, error_t * error)
{
  lex_t lex = { LT_NULL };
  const char* const beginnig = startLex;
  const char* endLex = NULL;

  for (; isspace((unsigned char)*startLex); startLex++);

  {
    endLex = startLex;
    /*если это число*/
    if (isdigit((unsigned char)*startLex) || *startLex == '.')
    {
      lex.body.number = GetNumber(startLex, &endLex, error);
      endLex--;
      if (!error->exist)
      {
        lex.type = LT_NUMBER;
        lex.priority = 10;
      }
      else
        return g_LEX_NULL;
    }
    /*если первая буква или"_"*/
    if (isalpha((unsigned char)*startLex) || *startLex == '_')
    {
      while (isalpha((unsigned char)*endLex) || *endLex == '_' || isdigit((unsigned char)*endLex))
        endLex++;
      endLex--;

      if (startLex == endLex && *startLex != 'e')/*проверям на переменную*/
      {
        lex.type = LT_VARIABLE;
        lex.priority = 10;
        lex.body.variable.name = *startLex;
      }

      else if (*(endLex + 1) == '(')/*проверка на функцию*/
      {
        lex.body.Function = GetUnFun(startLex, endLex);/*проверка на унарную функцию*/
        if (lex.body.Function != NULL)
        {
          lex.type = LT_UNFUNCTION;
          lex.priority = 15;
        }
        else
        {
          lex.body.Operation = GetBiFun(startLex, endLex);/*проверка на бинарную функцию*/
          if (lex.body.Operation != NULL)
          {
            lex.type = LT_BIFUNCTION;
            lex.priority = 15;
          }
        }
      }

      else
      {
        lex = WhatConst(startLex, endLex);
      }
    }

    if (IsCharacter(*startLex))
    {
      if (*startLex == ':' && *(startLex + 1) == '=')
        endLex++;

      lex.body.Operation = GetOperation(startLex, endLex);
      if (lex.body.Operation != NULL)
      {
        lex.type = LT_OPERATION;
        switch (*startLex)
        {
        case '+':
        case '-':
          lex.priority = 11;
          break;
        case '*':
        case '/':
          lex.priority = 12;
          break;
        case '^':
          lex.priority = 14;  //unary "-" have 13
          break;
        }
      }

      if (lex.type == LT_NULL)
      {
        lex.body.separators = GetSeparators(startLex, endLex);
        if (lex.body.separators != '\0')
        {
          lex.type = LT_SEPARARTORS;
          switch (*startLex)
          {
          case '(':
          case ')':
            lex.priority = 3; //todo
            break;
          case ';':
            lex.priority = 1;
            break;
          case ',':
            lex.priority = 4;//todo
            break;
          case '=':
            lex.priority = 2;
            break;
          }
        }
      }
    }
    if (lex.type == LT_NULL && !error->exist)
    {
      LogError(error, "(GetLex):", "invalid character!");
      return g_LEX_NULL;
    }
  }
  for (; isspace((unsigned char)*(endLex + 1)); endLex++);
  *lenght = endLex - beginnig + 1;


  return lex;
}

double Compute(box_t const * expression, error_t * error)
{
  box_t variable = { LT_NULL };
  box_t stack = { LT_NULL };
  lex_t lexExp;
  lex_t lex = g_LEX_NULL;
  lex_t auxiliaryLex;
  lex_t* lextmp;
  int i = 0;
  int j = 0;

  BoxInitialization(&stack);
  BoxInitialization(&variable);

  for (i = 0; i < expression->numElement && !error->exist; i++)
  {
    lexExp = BoxGet(expression, i);
    switch (lexExp.type)
    {

    case LT_NUMBER:
      BoxAddBack(&stack, &lexExp, error);
      if (error->exist)
        continue;
      break;
    case LT_VARIABLE:
      for (j = 0; j < variable.numElement; j++)
      {
        auxiliaryLex = BoxGet(&variable, j);
        if (auxiliaryLex.body.variable.name == lexExp.body.variable.name)
        {
          lexExp.type = LT_NUMBER;
          lexExp.priority = 10;
          lexExp.body.number = auxiliaryLex.body.variable.number;
          BoxAddBack(&stack, &lexExp, error);
          break;
        }
      }
      if (error->exist)
        continue;

      if (lexExp.type == LT_VARIABLE)
      {
        BoxAddBack(&variable, &lexExp, error);
        if (error->exist)
          continue;
        BoxAddBack(&stack, &lexExp, error);
        if (error->exist)
          continue;
      }
      break;
    case LT_UNFUNCTION:
      lex = BoxGetBack(&stack);
      if (lex.type != LT_NUMBER)                /*из стека взято число?*/
      {
        LogError(error, "(Compute):", "incorrect expression!");
        continue;
      }
      CheckArgFunc(&lexExp, &lex, error);
      if (error->exist)
        continue;
      lex.body.number = lexExp.body.Function(lex.body.number);
      BoxAddBack(&stack, &lex, error);
      if (error->exist)
        continue;
      break;
    case LT_BIFUNCTION:
      lex = BoxGetBack(&stack);
      if (lex.type != LT_NUMBER)                /*из стека взято число?*/
      {
        LogError(error, "(Compute):", "incorrect expression!");
        continue;
      }
      auxiliaryLex = BoxGetBack(&stack);
      if (auxiliaryLex.type != LT_NUMBER)      /*из стека взято число?*/
      {
        LogError(error, "(Compute):", "incorrect expression!");
        continue;
      }
      CheckArgOper(&lexExp, &auxiliaryLex, &lex, error);
      if (error->exist)
        continue;
      lex.body.number = lexExp.body.Operation(auxiliaryLex.body.number, lex.body.number);
      BoxAddBack(&stack, &lex, error);
      if (error->exist)
        continue;
      break;

    case LT_OPERATION:
      lex = BoxGetBack(&stack);
      if (lex.type != LT_NUMBER)   /*из стека взято число?*/
      {
        LogError(error, "(Compute):", "incorrect expression!");
        continue;
      }
      auxiliaryLex = BoxGetBack(&stack);
      if (auxiliaryLex.type != LT_NUMBER)      /*из стека взято число?*/
      {
        LogError(error, "(Compute):", "incorrect expression!");
        continue;
      }
      CheckArgOper(&lexExp, &auxiliaryLex, &lex, error);
      if (error->exist)
        continue;
      lex.body.number = lexExp.body.Operation(auxiliaryLex.body.number, lex.body.number);
      BoxAddBack(&stack, &lex, error);
      if (error->exist)
        continue;
      break;
    case LT_SEPARARTORS:/*либо "=" либо ";"*/
      switch (lexExp.body.separators)
      {
      case '=':
        lex = BoxGetBack(&stack);
        if (lex.type != LT_NUMBER)   /*из стека взято число?*/
        {
          LogError(error, "(Compute):", "incorrect expression!");
          continue;
        }
        auxiliaryLex = BoxGetBack(&stack);
        if (auxiliaryLex.type != LT_VARIABLE)      /*из стека взята переменная?*/
        {
          LogError(error, "(Compute):", "incorrect expression!");
          continue;
        }
        for (j = 0; j < variable.numElement; j++)/*ищем переменную в списке */
        {
          lextmp = BoxAddress(&variable, j);
          if (lextmp->body.variable.name == auxiliaryLex.body.variable.name)
          {
            lextmp->body.variable.number = lex.body.number;
            BoxAddBack(&stack, &lex, error);
            if (error->exist)
            {
              LogError(error, "(Compute):", "incorrect expression!");
              continue;
            }
            break;
          }
        }
        break;
      case ';':
        while (!BoxEmpty(&stack))
          BoxGetBack(&stack);
        break;


      }
      break;
    default:
      LogError(error, "(Compute):", "incorrect expression!");
      break;
    }
  }

  lex = BoxGetBack(&stack);
  if (BoxEmpty(&stack) && lex.type == LT_NUMBER)
  {
    BoxDestroy(&variable);
    BoxDestroy(&stack);
    return lex.body.number;
  }
  if (!error->exist)
    LogError(error, "(Compute):", "incorrect expression!");

  BoxDestroy(&variable);
  BoxDestroy(&stack);
  return 0.0;
}


/*функция заполняет сведения об ошибке*/
void LogError(error_t * error, const char * id, const char * message)
{
  error->exist = TRUE;
  if (strlen(id) < MAX_ERROR)
    strcpy(error->id, id);
  else
    strcpy(error->id, "unable to display id");
  if (strlen(message) < MAX_ERROR)
    strcpy(error->message, message);
  else
    strcpy(error->message, "unable to display error message!");
}

void s_procFinalSymbols(lex_t* stackLast, box_t* expression, error_t* error, box_t* stack)
{
  lex_t auxiliaryLex;

  while (!(stackLast->type == LT_SEPARARTORS && stackLast->body.separators == '(') && !error->exist)/*пока не скобка все достаем*/
  {
    auxiliaryLex = BoxGetBack(stack);
    BoxAddBack(expression, &auxiliaryLex, error);
    if (error->exist)
      continue;

    if (!BoxEmpty(stack))
      *stackLast = BoxSeeLast(stack);
    else
    {
      LogError(error, "(CreationPolNonation):", "inconsistency of the brackets!");
      continue;
    }
  }
}

void s_moveStackToExp(lex_t* stackLast, lex_t lex, box_t* stack, box_t* expression, error_t* error)
{
  lex_t auxiliaryLex;
  while (stackLast->priority >= lex.priority && !BoxEmpty(stack) && (stackLast->priority != 14 || lex.priority != 14))
  {
    auxiliaryLex = BoxGetBack(stack);
    BoxAddBack(expression, &auxiliaryLex, error);
    *stackLast = BoxSeeLast(stack);
  }
  if (lex.type == LT_SEPARARTORS && lex.body.separators == ';') /*";" после выталкивания идет в выражение, а не в стек*/
    BoxAddBack(expression, &lex, error);
  else
    BoxAddBack(stack, &lex, error);
}