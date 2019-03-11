// Implements dynamically allocated strings as a contiguous block starting with
// two `size_t`s (representing allocated size and content size, resp.) and a
// `char` array which any allocated byte not part of the content is `\0`.
// It is enforced that at least one '\0' occurs at the end of the string, so
// `dstr_content` returns a C string.
#ifndef __DSTR_H__
#define __DSTR_H__

#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

typedef void dstr;

// Gets the allocated size of the contents of a dynamic string.
size_t dstr_asize_get (const dstr *);

// Gets the size of the contents of a dynamic string.
size_t dstr_csize_get (const dstr *);

// Makes a new dynamic string.
void dstr_init (dstr **);

// Clears the dynamic string while keeping its allocated memory.
void dstr_clear (dstr *);

// Frees the memory allocated to a dynamic string.
void dstr_delete (dstr *);

// Prints a dynamic string.
void dstr_print (const dstr *);

// Prints a dynamic string and ends the line.
void dstr_print_line (const dstr *);

// Appends a character to a dynamic string; automatically reallocates if needed.
void dstr_append_char (dstr **, char);

// Concatenates two strings; automatically reallocates if needed.
void dstr_concat (dstr **, const dstr *);

// Checks equality of the contents of dynamic strings.
bool dstr_equal (const dstr *, const dstr *);

// Checks whether a dynamic string has specified content.
bool dstr_equal_const (const dstr *, const char *);

#endif
