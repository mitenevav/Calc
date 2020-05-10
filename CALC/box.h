#ifndef BOX_H_INCLUDED__
#define BOX_H_INCLUDED__
#pragma once
#include "linehanding.h"

extern const lex_t g_LEX_NULL;

void BoxInitialization(box_t* box);
void BoxDestroy(box_t* box);
void BoxAddBack(box_t* box, const lex_t* lex, error_t* error);
lex_t BoxGetBack(box_t* box);
lex_t BoxSeeLast(box_t const* box);
int BoxEmpty(const box_t* box);
lex_t BoxGet(const box_t*box, int index);
lex_t* BoxAddress(box_t* box, int index);

#endif /* BOX_H_INCLUDED__ */