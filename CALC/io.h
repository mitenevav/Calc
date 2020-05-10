#ifndef IO_H_UNCLUDED__
#define IO_H_UNCLUDED__
#pragma once
#include"linehanding.h"
#include <stdio.h>

char* StrRead(FILE* f, int*eoff);
void PrintNoExpression(char const * str);
void PrintExpression(char const * str, double result);
void PrintError(char const * str, error_t* error);
void PrintFatalError(char const *str);

#endif /* IO_H_UNCLUDED__*/