// Implements an atrribute as a contiguous block consisting of two `size_t`s
// (representing allocated and actual length of the content), a pointer to
// a dynamic string, and a `NULL`-terminated array of pointers to dynamic strings.
// All pointers past the end (and in the allocated range) are `NULL`.
#ifndef _MJ_ATTR_H_
#define _MJ_ATTR_H_

#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dstr.h"

typedef void mj_attr;

// Gets the allocated size of the contents of an attribute.
size_t attr_asize_get (const mj_attr *);

// Gets the size of the contents of an attribute.
size_t attr_csize_get (const mj_attr *);

// Gets the name of an attribute.
dstr * attr_name_get (const mj_attr *);

// Sets the name of an attribute.
void attr_name_set (mj_attr *, const dstr *);

// Sets the name of an attribute.
void attr_name_set_const (mj_attr *, const char *);

// Makes a new attribute.
void attr_init (mj_attr **);

// Frees the memory allocated to an attribute and its children.
void attr_delete (mj_attr *);

// Prints an attribute.
void attr_print (const mj_attr *);

// Prints an attribute and ends the line.
void attr_print_line (const mj_attr *);

// Appends a dynamic string to the attribute; automatically reallocates if needed.
void attr_append_dstr (mj_attr **, const dstr *);

// Appends a C string to the attribute; automatically reallocates if needed.
void attr_append_cstr (mj_attr **, const char *);

#endif
