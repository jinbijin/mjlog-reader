#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dstr.h"
#include "attr.h"
#include "elt.h"

#define ELT_INITIAL_ALLOC 2

// Subroutines

size_t elt_asize_get (const mj_elt *this) {
  assert(this != NULL);
  return *((size_t *) this);
}

// Sets the allocated size of the contents of an element.
// Private as this needs to be equal to the actual allocated size.
void elt_asize_set (mj_elt *this, size_t n) {
  assert(this != NULL);
  size_t *field = (size_t *) this;
  *field = n;
}

size_t elt_csize_get (const mj_elt *this) {
  assert(this != NULL);
  return *((size_t *) this + 1);
}

// Sets the size of the contents of an element.
// Private as this needs to be equal to the actual size.
void elt_csize_set (mj_elt *this, size_t n) {
  assert(this != NULL);
  size_t *field = (size_t *) this + 1;
  *field = n;
}

size_t elt_tsize_get (const mj_elt *this) {
  assert(this != NULL);
  return *((size_t *) this + 2);
}

// Sets the size of the element part of the contents of an element.
// Private as this needs to be equal to the actual size.
void elt_tsize_set (mj_elt *this, size_t n) {
  assert(this != NULL);
  size_t *field = (size_t *) this + 2;
  *field = n;
}

dstr * elt_name_get (const mj_elt *this) {
  assert(this != NULL);
  return *(dstr **)((size_t *) this + 3);
}

void elt_name_set (mj_elt *this, const dstr *s) {
  assert(this != NULL);
  dstr **field = (dstr **)((size_t *) this + 3);
  dstr_clear(*field);
  dstr_concat(field, s);
}

void elt_name_set_const (mj_elt *this, const char *s) {
  assert(this != NULL);
  dstr **field = (dstr **)((size_t *) this + 3);
  dstr_clear(*field);
  dstr_concat_const(field, s);
}

mj_attr ** elt_content_attr (const mj_elt *this) {
  assert(this != NULL);
  return (mj_attr **)((size_t *) this + 3) + 1;
}

mj_elt ** elt_content_elt (const mj_elt *this) {
  assert(this != NULL);
  size_t csize = elt_csize_get(this);
  return (mj_elt **)((size_t *) this + 3) + 1 + csize;
}

// Allocation to a new element, in number of entries.
void elt_alloc (mj_elt **this, size_t n) {
  assert(this != NULL);
  *this = malloc((n+1) * sizeof(void *) + 3 * sizeof(size_t));
  assert(*this != NULL);

  dstr_init((dstr **)((size_t *) (*this) + 3));
  elt_asize_set(*this, n);
}

// Reallocation to an element, in number of entries.
void elt_realloc (mj_elt **this, size_t n) {
  assert(this != NULL);
  *this = realloc(*this, (n+1) * sizeof(void *) + 3 * sizeof(size_t));
  assert(*this != NULL);

  size_t size = elt_csize_get(*this) + elt_tsize_get(*this);
  mj_attr **content = elt_content_attr(*this);
  ptrdiff_t i;
  elt_asize_set(*this, n);
  for (i = size + 1; i < n; i++) {
    content[i] = NULL;
  }
}

// Basic functions

// A raw clear of an element.
// Private as references to (likely allocated) data are lost.
void elt_clear (mj_elt *this) {
  size_t asize = elt_asize_get(this);
  dstr *name = elt_name_get(this);
  mj_attr **content = elt_content_attr(this);

  ptrdiff_t i;
  elt_csize_set(this, 0);
  elt_tsize_set(this, 0);
  dstr_clear(name);
  for (i = 0; i < asize; i++) {
    content[i] = NULL;
  }
}

void elt_init (mj_elt **this) {
  elt_alloc(this, ELT_INITIAL_ALLOC);
  elt_clear(*this);
}

void elt_delete (mj_elt *this) {
  size_t csize = elt_csize_get(this);
  size_t tsize = elt_tsize_get(this);
  mj_attr **content_attr = elt_content_attr(this);
  mj_elt **content_elt = elt_content_elt(this);

  ptrdiff_t i;

  dstr_delete(elt_name_get(this));
  for (i = 0; i < csize; i++) {
    attr_delete(content_attr[i]);
  }
  for (i = 0; i < tsize; i++) {
    elt_delete(content_elt[i]);
  }
  free(this);
}

void elt_print_lines (const mj_elt *this) {
  size_t csize = elt_csize_get(this);
  size_t tsize = elt_tsize_get(this);
  mj_attr **content_attr = elt_content_attr(this);
  mj_elt **content_elt = elt_content_elt(this);
  ptrdiff_t i;

  printf("ELT ");
  dstr_print(elt_name_get(this));
  if (csize + tsize > 0) {
    printf(" (\n");
  }
  for (i = 0; i < csize; i++) {
    attr_print_line(content_attr[i]);
  }
  for (i = 0; i < tsize; i++) {
    elt_print_lines(content_elt[i]);
  }
  if (csize + tsize > 0) {
    printf(")\n");
  }
  else {
    printf("\n");
  }
}

// Write functions

void elt_append_new_attr (mj_elt **this) {
  size_t asize = elt_asize_get(*this);
  size_t csize = elt_csize_get(*this);
  size_t tsize = elt_tsize_get(*this);
  assert(tsize == 0);

  if (csize + 1 >= asize) {
    elt_realloc(this, 2 * asize);
  }

  mj_attr **content = elt_content_attr(*this);
  attr_init(content + csize);
  elt_csize_set(*this, csize + 1);
}

mj_elt ** elt_append_new_elt (mj_elt **this) {
  size_t asize = elt_asize_get(*this);
  size_t csize = elt_csize_get(*this);
  size_t tsize = elt_tsize_get(*this);

  if (csize + tsize + 1 >= asize) {
    elt_realloc(this, 2 * asize);
  }

  mj_elt **content = elt_content_elt(*this);
  elt_init(content + tsize);
  elt_tsize_set(*this, tsize + 1);

  return content + tsize;
}

void elt_current_attr_name_set (mj_elt *this, const dstr *s) {
  size_t asize = elt_asize_get(this);
  size_t csize = elt_csize_get(this);
  size_t tsize = elt_tsize_get(this);
  assert(tsize == 0);

  mj_attr **content = elt_content_attr(this);
  attr_name_set(content[csize-1], s);
}

void elt_current_attr_name_set_const (mj_elt *this, const char *s) {
  size_t asize = elt_asize_get(this);
  size_t csize = elt_csize_get(this);
  size_t tsize = elt_tsize_get(this);
  assert(tsize == 0);

  mj_attr **content = elt_content_attr(this);
  attr_name_set_const(content[csize-1], s);
}

void elt_current_attr_append_dstr (mj_elt *this, const dstr *s) {
  size_t asize = elt_asize_get(this);
  size_t csize = elt_csize_get(this);
  size_t tsize = elt_tsize_get(this);
  assert(tsize == 0);

  mj_attr **content = elt_content_attr(this);
  attr_append_dstr(content + (csize-1), s);
}

void elt_current_attr_append_cstr (mj_elt *this, const char *s) {
  size_t asize = elt_asize_get(this);
  size_t csize = elt_csize_get(this);
  size_t tsize = elt_tsize_get(this);
  assert(tsize == 0);

  mj_attr **content = elt_content_attr(this);
  attr_append_cstr(content + (csize-1), s);
}

void elt_current_elt_delete (mj_elt *this) {
  size_t tsize = elt_tsize_get(this);

  mj_elt **content = elt_content_elt(this);
  elt_delete(content[tsize-1]);
  elt_tsize_set(this, tsize-1);
}
