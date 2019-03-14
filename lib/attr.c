#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dstr.h"
#include "attr.h"

#define ATTR_INITIAL_ALLOC 4

// Subroutines

size_t attr_asize_get (const mj_attr *this) {
  assert(this != NULL);
  return *((size_t *) this);
}

// Sets the allocated size of the contents of an attribute.
// Private as this needs to be equal to the actual allocated size.
void attr_asize_set (mj_attr *this, size_t n) {
  assert(this != NULL);
  size_t *field = (size_t *) this;
  *field = n;
}

size_t attr_csize_get (const mj_attr *this) {
  assert(this != NULL);
  return *((size_t *) this + 1);
}

// Sets the size of the contents of an attribute.
// Private as this needs to be equal to the actual size.
void attr_csize_set (mj_attr *this, size_t n) {
  assert(this != NULL);
  size_t *field = (size_t *) this + 1;
  *field = n;
}

dstr * attr_name_get (const mj_attr *this) {
  assert(this != NULL);
  return *(dstr **)((size_t *) this + 2);
}

void attr_name_set (mj_attr *this, const dstr *s) {
  assert(this != NULL);
  dstr **field = (dstr **)((size_t *) this + 2);
  dstr_clear(*field);
  dstr_concat(field, s);
}

void attr_name_set_const (mj_attr *this, const char *s) {
  assert(this != NULL);
  dstr **field = (dstr **)((size_t *) this + 2);
  dstr_clear(*field);
  dstr_concat_const(field, s);
}

dstr ** attr_content (const mj_attr *this) {
  assert(this != NULL);
  return (dstr **)((size_t *) this + 2) + 1;
}

// Allocation to a new attribute, in number of entries.
void attr_alloc (mj_attr **this, size_t n) {
  assert(this != NULL);
  *this = malloc((n+1) * sizeof(dstr *) + 2 * sizeof(size_t));
  assert(*this != NULL);

  dstr_init((dstr **)((size_t *) (*this) + 2));
  attr_asize_set(*this, n);
}

// Reallocation to an attribute, in number of entries.
void attr_realloc (mj_attr **this, size_t n) {
  assert(this != NULL);
  *this = realloc(*this, (n+1) * sizeof(dstr *) + 2 * sizeof(size_t));
  assert(*this != NULL);

  size_t csize = attr_csize_get(*this);
  dstr **content = attr_content(*this);
  ptrdiff_t i;
  attr_asize_set(*this, n);
  for (i = csize + 1; i < n; i++) {
    content[i] = NULL;
  }
}

// Basic functions

// A raw clear of an attribute.
// Private as references to (likely allocated) data are lost.
void attr_clear (mj_attr *this) {
  size_t asize = attr_asize_get(this);
  dstr *name = attr_name_get(this);
  dstr **content = attr_content(this);

  ptrdiff_t i;
  attr_csize_set(this, 0);
  dstr_clear(name);
  for (i = 0; i < asize; i++) {
    content[i] = NULL;
  }
}

void attr_init (mj_attr **this) {
  attr_alloc(this, ATTR_INITIAL_ALLOC);
  attr_clear(*this);
}

void attr_delete (mj_attr *this) {
  size_t csize = attr_csize_get(this);
  dstr **content = attr_content(this);
  ptrdiff_t i;

  dstr_delete(attr_name_get(this));
  for (i = 0; i < csize; i++) {
    dstr_delete(content[i]);
  }
  free(this);
}

void attr_print (const mj_attr *this) {
  size_t csize = attr_csize_get(this);
  dstr **content = attr_content(this);
  ptrdiff_t i;

  printf("ATTR ");
  dstr_print(attr_name_get(this));
  for (i = 0; i < csize; i++) {
    printf(" ");
    dstr_print(content[i]);
  }
}

void attr_print_line (const mj_attr *this) {
  attr_print(this);
  printf("\n");
}

void attr_append_dstr (mj_attr **this, const dstr *s) {
  size_t asize = attr_asize_get(*this);
  size_t csize = attr_csize_get(*this);
  dstr **content;

  if (s != NULL) {
    if (csize + 1 >= asize) {
      attr_realloc(this, 2 * asize);
    }

    content = attr_content(*this);

    dstr_init(content + csize);
    dstr_concat(content + csize, s);
    attr_csize_set(*this, csize + 1);
  }
}

void attr_append_cstr (mj_attr **this, const char *s) {
  size_t asize = attr_asize_get(*this);
  size_t csize = attr_csize_get(*this);
  dstr **content;

  if (s != NULL) {
    if (csize + 1 >= asize) {
      attr_realloc(this, 2 * asize);
    }

    content = attr_content(*this);

    dstr_init(content + csize);
    dstr_concat_const(content + csize, s);
    attr_csize_set(*this, csize + 1);
  }
}
