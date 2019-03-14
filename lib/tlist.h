// Implements list of tokens as a contiguous block consisting of two `size_t`s
// (representing allocated and actual length of the content) and a `NULL`-
// terminated array of pointers to `mj_token`s.
// All pointers past the end (and in the allocated range) are `NULL`.
#ifndef _MJ_TLIST_H_
#define _MJ_TLIST_H_

#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dstr.h"
#include "token.h"

typedef void mj_tlist;

// Gets the allocated size of the contents of a list of tokens.
size_t tlist_asize_get (const mj_tlist *);

// Gets the size of the contents of a list of tokens.
size_t tlist_csize_get (const mj_tlist *);

// Returns the underlying array attached to a list of tokens.
// Public as the pointer formula is available anyway.
mj_token ** tlist_content (const mj_tlist *);

// Makes a new list of tokens.
void tlist_init (mj_tlist **);

// Frees the memory allocated to a list of tokens and its children.
void tlist_delete (mj_tlist *);

// Prints a list of tokens.
void tlist_print_lines (const mj_tlist *);

// Creates a new token at the end of the list; automatically reallocates if needed.
void tlist_append_new_token (mj_tlist **);

// Appends a character to the newest token.
void tlist_append_char (mj_tlist *, char);

// Gets the type of the current token.
token_t tlist_current_type_get (const mj_tlist *);

// Sets the type of the current token.
void tlist_current_type_set (mj_tlist *, token_t);

// Deletes the last token if it is empty.
void tlist_current_delete_empty (mj_tlist *);

#endif
