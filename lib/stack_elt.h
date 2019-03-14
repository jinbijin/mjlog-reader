#ifndef _STACK_ELT_H_
#define _STACK_ELT_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dstr.h"
#include "token.h"
#include "tlist.h"
#include "lexer.h"
#include "attr.h"
#include "elt.h"

// A stack of elements; per usual, the empty stack is represented by the `NULL`
// pointer.
struct _stack_elt {
  mj_elt **elt;
  struct _stack_elt *next;
};

typedef struct _stack_elt stack_elt;

void stack_elt_push (stack_elt **, mj_elt **);

mj_elt ** stack_elt_pop (stack_elt **);

mj_elt ** stack_elt_top (stack_elt *);

#endif
