#ifndef MATHFUNC_H_INCLEDED__
#define MATHFUNC_H_INCLEDED__
#pragma once
#include "linehanding.h"

double Add(double x, double y);
double Sub(double x, double y);
double Mul(double x, double y);
double Div(double x, double y);
double Pow(double x, double y);
double Cot(double x);
double Floor(double x);
double Ceil(double x);
void CheckArgFunc(lex_t const * func, lex_t const * x, error_t* error);
void CheckArgOper(lex_t const * oper, lex_t const * x, lex_t const * y, error_t* error);
double Log(double x, double y);


#endif /* MATHFUNC_H_INCLEDED__  */