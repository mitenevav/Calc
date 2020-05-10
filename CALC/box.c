// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma warning(disable:4996)
#include "box.h"
#include "linehanding.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define ADDELEMENTS 10
#define MINELEMENTS 3

const lex_t g_LEX_NULL = { LT_NULL };

void BoxInitialization(box_t * box)
{
  box->lenght = 0;
  box->numElement = 0;
  box->str = NULL;
}

void BoxDestroy(box_t* box)
{
  free(box->str);
  BoxInitialization(box);
}

void BoxAddBack(box_t* box, const lex_t* lex, error_t* error)
{
  lex_t* dopStr;
  if (box->lenght - box->numElement <= MINELEMENTS)
  {
    dopStr = (lex_t*)realloc(box->str, (box->lenght + ADDELEMENTS) * sizeof(lex_t));
    if (dopStr == NULL)
    {
      LogError(error, "(BoxAddBack):", "not enough memory!");
      return;
    }
    else
    {
      box->str = dopStr;
      box->lenght += ADDELEMENTS;
    }
  }
  box->str[box->numElement] = *lex;
  box->numElement++;
}

lex_t BoxGetBack(box_t* box)
{
  lex_t lex;
  lex_t* dopStr;

  if (box->lenght - box->numElement > ADDELEMENTS * 2)
  {
    dopStr = (lex_t*)realloc(box->str, (box->lenght - ADDELEMENTS) * sizeof(lex_t));
    if (dopStr != NULL)
    {
      box->str = dopStr;
      box->lenght -= ADDELEMENTS;
    }
  }

  if (box->numElement == 0)
  {
    lex.type = LT_NULL;
    return lex;
  } 
  lex = box->str[--box->numElement];
  return lex;
}

lex_t BoxSeeLast(box_t const * box)
{
  if (box->numElement <= 0)
  {
    return g_LEX_NULL;
  }
  return box->str[box->numElement - 1];
}

int BoxEmpty(const box_t* box)
{
  return !box->numElement;
}

lex_t BoxGet(const box_t * box, int index)
{
  if (index >= 0 && index < box->numElement)
    return box->str[index];
  return g_LEX_NULL;
}

lex_t * BoxAddress(box_t * box, int index)
{
  if (index >= 0 && index < box->numElement)
    return &box->str[index];
  return NULL;
}