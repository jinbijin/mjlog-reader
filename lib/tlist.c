#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dstr.h"
#include "token.h"
#include "tlist.h"

#define TLIST_INITIAL_ALLOC 16

// Subroutines

size_t tlist_asize_get (const mj_tlist *this) {
  assert(this != NULL);
  return *((size_t *) this);
}

// Sets the allocated size of the contents of a list of tokens.
// Private as this needs to be equal to the actual allocated size.
void tlist_asize_set (mj_tlist *this, size_t n) {
  assert(this != NULL);
  size_t *field = (size_t *) this;
  *field = n;
}

size_t tlist_csize_get (const mj_tlist *this) {
  assert(this != NULL);
  return *((size_t *) this + 1);
}

// Sets the size of the contents of a list of tokens.
// Private as this needs to be equal to the actual size.
void tlist_csize_set (mj_tlist *this, size_t n) {
  assert(this != NULL);
  size_t *field = (size_t *) this + 1;
  *field = n;
}

// Returns the C string attached to a list of tokens.
// Private as this needs to satisfy multiple properties in terms of the other
// fields.
mj_token ** tlist_content (const mj_tlist *this) {
  assert(this != NULL);
  return (mj_token **)((size_t *) this + 2);
}

// Allocation to a new list of tokens, in number of entries.
void tlist_alloc (mj_tlist **this, size_t n) {
  assert(this != NULL);
  *this = malloc(n * sizeof(mj_token *) + 2 * sizeof(size_t));
  assert(*this != NULL);

  tlist_asize_set(*this, n);
}

// Reallocation to a list of tokens, in number of entries.
void tlist_realloc (mj_tlist **this, size_t n) {
  assert(this != NULL);
  *this = realloc(*this, n * sizeof(mj_token *) + 2 * sizeof(size_t));
  assert(*this != NULL);

  size_t csize = tlist_csize_get(*this);
  mj_token **content = tlist_content(*this);
  ptrdiff_t i;
  tlist_asize_set(*this, n);
  for (i = csize + 1; i < n; i++) {
    content[i] = NULL;
  }
}

// Basic functions

// A raw clear of a list of tokens.
// Private as references to (likely allocated) data are lost.
void tlist_clear (mj_tlist *this) {
  size_t asize = tlist_asize_get(this);
  mj_token **content = tlist_content(this);

  ptrdiff_t i;
  tlist_csize_set(this, 0);
  for (i = 0; i < asize; i++) {
    content[i] = NULL;
  }
}

void tlist_init (mj_tlist **this) {
  tlist_alloc(this, TLIST_INITIAL_ALLOC);
  tlist_clear(*this);
}

void tlist_delete (mj_tlist *this) {
  size_t csize = tlist_csize_get(this);
  mj_token **content = tlist_content(this);
  ptrdiff_t i;

  for (i = 0; i < csize; i++) {
    token_delete(content[i]);
  }
  free(this);
}

void tlist_print_lines (const mj_tlist *this) {
  size_t csize = tlist_csize_get(this);
  mj_token **content = tlist_content(this);
  ptrdiff_t i;

  for (i = 0; i < csize; i++) {
    token_print_line(content[i]);
  }
}

void tlist_append_new_token (mj_tlist **this) {
  size_t asize = tlist_asize_get(*this);
  size_t csize = tlist_csize_get(*this);

  if (csize + 1 >= asize) {
    tlist_realloc(this, 2 * asize);
  }

  mj_token **content = tlist_content(*this);

  token_init(content + csize);
  tlist_csize_set(*this, csize + 1);
}

void tlist_append_char (mj_tlist *this, char c) {
  size_t csize = tlist_csize_get(this);
  mj_token **content = tlist_content(this);
  token_append_char(content + (csize-1), c);
}

token_t tlist_current_type_get (const mj_tlist *this) {
  size_t csize = tlist_csize_get(this);
  mj_token **content = tlist_content(this);
  return token_type_get(content[csize-1]);
}

void tlist_current_type_set (mj_tlist *this, token_t t) {
  size_t csize = tlist_csize_get(this);
  mj_token **content = tlist_content(this);
  token_type_set(content[csize-1], t);
}

void tlist_current_delete_empty (mj_tlist *this) {
  size_t csize = tlist_csize_get(this);
  mj_token **content = tlist_content(this);
  if (token_equal_const(content[csize-1], TOK_EMPTY, "")) {
    token_delete(content[csize-1]);
    tlist_csize_set(this, csize-1);
  }
}
