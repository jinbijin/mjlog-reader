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
#include "stack_elt.h"

void stack_elt_push (stack_elt **this, mj_elt **elt) {
  stack_elt *top = malloc(sizeof(*top));
  top->elt = elt;
  top->next = *this;
  *this = top;
}

// I shouldn't need the return value, but at this point it's classical to
// include it.
mj_elt ** stack_elt_pop (stack_elt **this) {
  assert(this != NULL);
  stack_elt *tmp = *this;
  mj_elt **out = (*this)->elt;
  *this = (*this)->next;
  free(tmp);
  return out;
}

mj_elt ** stack_elt_top (stack_elt *this) {
  assert(this != NULL);
  return (this->elt);
}
