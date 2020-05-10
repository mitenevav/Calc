#ifndef LINEHANDING_H_INCLUDED__
#define LINEHANDING_H_INCLUDED__
#pragma once

#define MAX_ERROR 101

extern const double g_PI;

typedef double(*functoin_t)(double arg);
typedef double(*operation_t)(double firtsArg, double secondArg);

typedef struct
{
  char name;
  double number;
}variable_t;

typedef struct
{
  enum
  {
    LT_NULL, LT_NUMBER, LT_OPERATION,
    LT_UNFUNCTION, LT_BIFUNCTION, LT_VARIABLE, LT_SEPARARTORS
  }type;
  int priority;
  union body
  {
    functoin_t Function;
    operation_t Operation;
    double number;
    char separators;
    variable_t variable;
  }body;
}lex_t;


typedef struct
{
  int lenght;
  int numElement;
  lex_t* str;
}box_t;

typedef struct
{
  int exist;
  char id[MAX_ERROR];
  char message[MAX_ERROR];
}error_t;

double Calculate(const char* str, box_t* expression, error_t *error);
void CreationPolNonation(char const * str, box_t* expression, error_t * error);
lex_t GetLex(const char * startLex, int* lenght, error_t * error);
double Compute(box_t const* expression, error_t * error);
void LogError(error_t* error, const char* id, const char* message);

#endif /* LINEHANDING_H_INCLUDED__*/