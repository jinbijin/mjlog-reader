// Defines tokens, which are simply dynamic strings prefixed by a token type.
#ifndef _MJ_TOKEN_H_
#define _MJ_TOKEN_H_

#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dstr.h"

typedef enum {
  TOK_EMPTY,
  TOK_ID,
  TOK_SYM,
  TOK_VAL
} token_t;

typedef void mj_token;

// Gets the type of a token.
token_t token_type_get (const mj_token *);

// Sets the type of a token.
void token_type_set (mj_token *, token_t);

// Gets the allocated size of the contents of a token.
size_t token_asize_get (const mj_token *);

// Gets the size of the contents of a token.
size_t token_csize_get (const mj_token *);

// Gets the underlying C string of a token.
// Public as the pointer formula is available anyway.
char * token_content (const mj_token *);

// Gets the underlying dynamic string of a token.
dstr * token_dynamic (const mj_token *);

// Clears the content of a token.
void token_clear (mj_token *);

// Makes a new token with specified type.
void token_init (mj_token **);

// Frees the memory allocated to a token.
void token_delete (mj_token *);

// Prints a token.
void token_print (const mj_token *);

// Prints a token and ends the line.
void token_print_line (const mj_token *);

// Appends a character to a token, reallocating if needed.
void token_append_char (mj_token **, char);

// Concatenates a dynamic string to a token, reallocating if needed.
void token_concat (mj_token **, const dstr *);

// Checks equality of tokens, i.e. that of types and contents.
bool token_equal (const mj_token *, const mj_token *);

// Checks whether a token has specified type and content.
bool token_equal_const (const mj_token *, token_t, const char *);

#endif
