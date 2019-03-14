// Implements an element as a contiguous block consisting of three `size_t`s
// (representing allocated size, number of attributes, number of elements),
// and a `NULL`-terminated array of pointers to attributes, resp. elements.
// All pointers past the end and in the allocated range are `NULL`.
#ifndef _MJ_ELT_H_
#define _MJ_ELT_H_

#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dstr.h"
#include "attr.h"

typedef void mj_elt;

// Gets the allocated size of the contents of an element.
size_t elt_asize_get (const mj_elt *);

// Gets the number of attributes of an element.
size_t elt_csize_get (const mj_elt *);

// Gets the number of subelements of an element.
size_t elt_tsize_get (const mj_elt *);

// Gets the name of an element.
dstr * elt_name_get (const mj_elt *);

// Sets the name of an element.
void elt_name_set (mj_elt *, const dstr *);

// Sets the name of an element.
void elt_name_set_const (mj_elt *, const char *);

// Returns the list of attributes attached to an element.
// Public as the pointer formula is available anyway.
mj_attr ** elt_content_attr (const mj_elt *);

// Returns the list of elements attached to an element.
// Public as the pointer formula is available anyway.
mj_elt ** elt_content_elt (const mj_elt *);

// Makes a new element.
void elt_init (mj_elt **);

// Frees the memory allocated to an element and its children.
void elt_delete (mj_elt *);

// Prints an element.
void elt_print_lines (const mj_elt *);

// Appends a new attribute to an element,
void elt_append_new_attr (mj_elt **);

// Appends a new subelement to an element, and returns a pointer to it.
mj_elt ** elt_append_new_elt (mj_elt **);

// Sets the name of the current attribute in an element.
void elt_current_attr_name_set (mj_elt *, const dstr *);

// Sets the name of the current attribute in an element.
void elt_current_attr_name_set_const (mj_elt *, const char *);

// Appends a value to the current attribute in an element.
void elt_current_attr_append_dstr (mj_elt *, const dstr *);

// Appends a value to the current attribute in an element.
void elt_current_attr_append_cstr (mj_elt *, const char *);

// Deletes the current subelement of an element.
void elt_current_elt_delete (mj_elt *);

#endif
