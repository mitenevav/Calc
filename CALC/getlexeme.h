#ifndef GETLEXEME_H_UNCLUDED__
#define GETLEXEME_H_UNCLUDED__
#pragma once
#include "linehanding.h"

int IsCharacter(const char ch);
int Compare(const char* start, const char * end, const char* someStr);
functoin_t GetUnFun(const char * startLex, const char* endLex);
lex_t WhatConst(const char* start, const char* end);
operation_t GetOperation(const char * start, const char* end);
char GetSeparators(const char * start, const char * end);
double GetNumber(const char* str, const char **endLex, error_t* error);
operation_t GetBiFun(const char * startLex, const char* endLex);

#endif /* GETLEXEME_H_UNCLUDED__*/