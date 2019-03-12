#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dstr.h"

#define DSTR_INITIAL_ALLOC 16

// Subroutines

size_t dstr_asize_get (const dstr *this) {
  assert(this != NULL);
  return *((size_t *) this);
}

// Sets the allocated size of the contents of a dynamic string.
// Private as this needs to be equal to the actual allocated size.
void dstr_asize_set (dstr *this, size_t n) {
  assert(this != NULL);
  size_t *field = (size_t *) this;
  *field = n;
}

size_t dstr_csize_get (const dstr *this) {
  assert(this != NULL);
  return *((size_t *) this + 1);
}

// Sets the size of the contents of a dynamic string.
// Private as this needs to be equal to the actual size.
void dstr_csize_set (dstr *this, size_t n) {
  assert(this != NULL);
  size_t *field = (size_t *) this + 1;
  *field = n;
}

// Returns the C string attached to a dynamic string.
// Private as this needs to satisfy multiple properties in terms of the other
// fields.
char * dstr_content (const dstr *this) {
  assert(this != NULL);
  return (char *)((size_t *) this + 2);
}

// Allocation to a new dynamic string, in number of `char`s.
void dstr_alloc (dstr **this, size_t n) {
  assert(this != NULL);
  *this = malloc(n * sizeof(char) + 2 * sizeof(size_t));
  assert(*this != NULL);

  dstr_asize_set(*this, n);
}

// Reallocation to a dynamic string, in number of `char`s.
void dstr_realloc (dstr **this, size_t n) {
  assert(this != NULL);
  *this = realloc(*this, n * sizeof(char) + 2 * sizeof(size_t));
  assert(*this != NULL);

  size_t csize = dstr_csize_get(*this);
  char *content = dstr_content(*this);
  ptrdiff_t i;
  dstr_asize_set(*this, n);
  for (i = csize + 1; i < n; i++) {
    content[i] = '\0';
  }
}

// Basic functions

void dstr_clear (dstr *this) {
  assert(this != NULL);

  size_t asize = dstr_asize_get(this);
  char *content = dstr_content(this);

  ptrdiff_t i;
  dstr_csize_set(this, 0);
  for (i = 0; i < asize; i++) {
    content[i] = '\0';
  }
}

void dstr_init (dstr **this) {
  dstr_alloc(this, DSTR_INITIAL_ALLOC);
  dstr_clear(*this);
}

void dstr_delete (dstr *this) {
  free(this);
}

void dstr_print (const dstr *this) {
  printf("%s",dstr_content(this));
}

void dstr_print_line (const dstr *this) {
  dstr_print(this);
  printf("\n");
}

void dstr_append_char (dstr **this, char c) {
  size_t asize = dstr_asize_get(*this);
  size_t csize = dstr_csize_get(*this);
  char *content;

  if (c != '\0') {
    if (csize + 1 >= asize) {
      dstr_realloc(this, 2 * asize);
    }
    content = dstr_content(*this);

    content[csize] = c;
    dstr_csize_set(*this, csize + 1);
  }
}

// Derived functions

void dstr_concat (dstr **this, const dstr *x) {
  // `this` isn't accessed until the loop, and there the assertion
  // is checked for `this`.
  assert(x != NULL);

  size_t csize = dstr_csize_get(x);
  char *content = dstr_content(x);

  ptrdiff_t i;
  for (i = 0; i < csize; i++) {
    dstr_append_char(this, content[i]);
  }
}

bool dstr_equal (const dstr *x, const dstr *y) {
  return (0 == strcmp(dstr_content(x),dstr_content(y)));
}

bool dstr_equal_const (const dstr *x, const char *y) {
  assert(y != NULL);
  return (0 == strcmp(dstr_content(x), y));
}
