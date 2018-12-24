/* `token.h`, by Jinbi Jin
 *
 * Contains functions dealing with tokens. 
 * Cannot be accessed from `parser.h`. */

#ifndef __TOKEN_H_
#define __TOKEN_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

// A token flag.
typedef enum {TOK_TAG, TOK_TAG_END, TOK_FIELD, TOK_VALUE} token_t;

// A token.
// @type The type of token.
// @alloc The allocated size of `content` in objects.
// @size The actual size of `content`.
// @content A string.
typedef struct {
  token_t type;
  size_t alloc;
  size_t size;
  char *content;
} mj_token;
// Conditions:
// * `0 <= size < alloc`; the actual array size is one more than the string size
// * `content[size] == '\0'`
// * `content[i] != '\0'` for all `0 <= i < size`

// Allocates memory for an empty token.
// Its flag should be supplied immediately.
void mj_token_init(mj_token *, token_t);

// Frees memory from a token.
void mj_token_final(mj_token *);

// Prints a token to console.
void mj_token_print(const mj_token *);

// Prints a token and a '\n' to console.
void mj_token_print_line(const mj_token *);

/* Type checking functions */

// Checks whether a token has a certain flag and content.
bool mj_token_is(const mj_token *, token_t, char *);

// Checks whether a token represents a discard.
bool mj_token_is_discard(const mj_token *);

// Checks whether a token represents a draw.
bool mj_token_is_draw(const mj_token *);

/* Specialised `(*bool)(const mj_token *)` functions */

// Checks whether a token pointer is `NULL`.
bool mj_token_is_null(const mj_token *);

// Checks whether a token represents an `INIT` tag.
bool mj_token_is_init(const mj_token *);

// Checks whether a token represents an event.
bool mj_token_is_event(const mj_token *);

// Checks whether a token represents an end of a round.
bool mj_token_is_terminal(const mj_token *);

// Checks whether a token represents the "step" attribute.
bool mj_token_is_step(const mj_token *);

/* Writing functions */

// Appends a `char` to the token.
void mj_token_append(mj_token *, char);

#endif
