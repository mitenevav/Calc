// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma warning(disable:4996)
#include "mathfunc.h"
#include "linehanding.h"
#include <math.h>

double Add(double x, double y)
{
  return x + y;
}
double Sub(double x, double y)
{
  return x - y;
}
double Mul(double x, double y)
{
  return x*y;
}
double Div(double x, double y)
{
  if (y != 0.0)
    return x / y;
  return 0.0;
}
double Pow(double x, double y)
{
  return pow(x, y);
}
double Cot(double x)
{
  return cos(x) / sin(x);
}

double Floor(double x)
{
  return floor(x);
}

double Ceil(double x)
{
  return ceil(x);
}

void CheckArgFunc(lex_t const * func, lex_t const * x, error_t* error)
{
  if ((func->body.Function == asin || func->body.Function == acos) && (x->body.number > 1 || x->body.number < -1))
    LogError(error, "(CheckArgFunc):", "impossible argument to arcsin(arccos)!");

  if (func->body.Function == sqrt && x->body.number < 0)
    LogError(error, "(CheckArgFunc):", "impossible argument to sqrt!");

  if (func->body.Function == log && x->body.number <= 0)
    LogError(error, "(CheckArgFunc):", "impossible argument to ln!");

  if (func->body.Function == tan && cos(x->body.number) == 0.0)
    LogError(error, "(CheckArgFunc):", "impossible argument to tg!");

  if (func->body.Function == Cot && sin(x->body.number) == 0.0)
    LogError(error, "(CheckArgFunc):", "impossible argument to ctg!");

}

void CheckArgOper(lex_t const * oper, lex_t const * x, lex_t const * y, error_t* error)
{
  if (oper->body.Operation == Pow && x->body.number < 0 && y->body.number != floor(y->body.number))
    LogError(error, "(CheckArgOper):", "impossible argument to \"^\"!");

  if (oper->body.Operation == Div && y->body.number == 0)
    LogError(error, "(CheckArgOper):", "to not divide by zero!");

  if (oper->body.Operation == Log && (x->body.number <= 0 || log(x->body.number) == 0 || y->body.number <= 0))
    LogError(error, "(CheckArgOper):", "impossible argument to log!");
}

double Log(double a, double x)
{
  return log(x) / log(a);
}