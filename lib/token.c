#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dstr.h"
#include "token.h"

#define TOK_INITIAL_ALLOC 16

// Subroutines
// These use the definition of `dstr`.

token_t token_type_get (const mj_token *this) {
  assert(this != NULL);
  return *((token_t *) this);
}

void token_type_set (mj_token *this, token_t t) {
  assert(this != NULL);
  token_t *type = (token_t *) this;
  *type = t;
}

size_t token_asize_get (const mj_token *this) {
  assert(this != NULL);
  return *((size_t *)((token_t *) this + 1));
}

// Sets the allocated size of the contents of a token.
// Private as this needs to be equal to the actual allocated size.
void token_asize_set (mj_token *this, size_t n) {
  assert(this != NULL);
  size_t *field = (size_t *)((token_t *) this + 1);
  *field = n;
}

size_t token_csize_get (const mj_token *this) {
  assert(this != NULL);
  return *((size_t *)((token_t *) this + 1) + 1);
}

// Sets the size of the contents of a token.
// Private as this needs to be equal to the actual content size.
void token_csize_set (mj_token *this, size_t n) {
  assert(this != NULL);
  size_t *field = (size_t *)((token_t *) this + 1) + 1;
  *field = n;
}

dstr * token_dynamic (const mj_token *this) {
  assert(this != NULL);
  return (dstr *)((token_t *) this + 1);
}

char * token_content (const mj_token *this) {
  assert(this != NULL);
  return (char *)((size_t *)((token_t *) this + 1) + 2);
}

// Allocates space for `n` characters to a token.
void token_alloc (mj_token **this, size_t n) {
  assert(this != NULL);
  *this = malloc(sizeof(token_t) + 2 * sizeof(size_t) + n * sizeof(char));
  assert(*this != NULL);

  token_asize_set(*this, n);
}

// Reallocates space for `n` characters to a token.
void token_realloc (mj_token **this, size_t n) {
  assert(this != NULL);
  *this = realloc(*this, sizeof(token_t) + 2 * sizeof(size_t) + n * sizeof(char));
  assert(*this != NULL);

  size_t csize = token_csize_get(*this);
  char *content = token_content(*this);
  ptrdiff_t i;
  token_asize_set(*this, n);
  for (i = csize + 1; i < n; i++) {
    content[i] = '\0';
  }
}

// Basic functions

void token_clear (mj_token *this) {
  dstr_clear(token_dynamic(this));
}

void token_init (mj_token **this) {
  token_alloc(this, TOK_INITIAL_ALLOC);
  token_type_set(*this, TOK_EMPTY);
  token_clear(*this);
}

void token_delete (mj_token *this) {
  free(this);
}

void token_print (const mj_token *this) {
  switch (token_type_get(this)) {
    case TOK_ID:
      printf("ID ");
      break;
    case TOK_SYM:
      printf("SYM ");
      break;
    case TOK_VAL:
      printf("VAL ");
      break;
    default:
      fprintf(stderr,"Invalid case.\n");
      exit(EXIT_FAILURE);
  }
  dstr_print(token_dynamic(this));
}

void token_print_line (const mj_token *this) {
  token_print(this);
  printf("\n");
}

void token_append_char (mj_token **this, char c) {
  size_t asize = token_asize_get(*this);
  size_t csize = token_csize_get(*this);
  char *content;

  if (c != '\0') {
    if (csize + 1 >= asize) {
      token_realloc(this, 2 * asize);
    }

    content = token_content(*this);

    content[csize] = c;
    token_csize_set(*this, csize + 1);
  }
}

void token_concat (mj_token **this, const dstr *x) {
  // `this` isn't accessed until the loop, and there the assertion
  // is checked for `this`.
  assert(x != NULL);

  size_t csize = token_csize_get(x);
  char *content = token_content(x);

  ptrdiff_t i;
  for (i = 0; i < csize; i++) {
    token_append_char(this, content[i]);
  }
}

bool token_equal (const mj_token *t, const mj_token *u) {
  return ((token_type_get(t) == token_type_get(u)) &&
    dstr_equal(token_dynamic(t),token_dynamic(u)));
}

bool token_equal_const (const mj_token *t, token_t type, const char *x) {
  return ((token_type_get(t) == type) && dstr_equal_const(token_dynamic(t),x));
}
