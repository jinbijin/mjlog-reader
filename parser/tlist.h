/* `tlist.h`, by Jinbi Jin 
 *
 * Contains functions dealing with token lists.
 * Cannot be accessed from `parser.h`. */

#ifndef __TLIST_H_
#define __TLIST_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

// A token list.
// @alloc The allocated size.
// @size The actual size.
// @content An array of `mj_token *`.
// @head The index of the token currently being *read*.
typedef struct {
  size_t alloc;
  size_t size;
  mj_token **content;
  ptrdiff_t head;
} mj_tlist;
// Conditions:
// * `0 <= head <= size < alloc`.
// * `content[size] == '\0'`
// * `content[i] != '\0'` for all `0 <= i < size`
// * No functions in this file move the head backward
//   (This is likely premature optimisation, but whatever.)

/* `mj_tlist` functions */

// Allocates memory for a `tlist`.
int mj_tlist_init(mj_tlist *);

// Frees memory from a `tlist`.
void mj_tlist_final(mj_tlist *);

// Prints a `tlist` to console.
void mj_tlist_print_lines(mj_tlist *);

/* Scanning functions */

// Moves to the next token in the list.
void mj_tlist_next(mj_tlist *);

// Moves a number of tokens forward in the list.
void mj_tlist_move(mj_tlist *, ptrdiff_t);

// Moves to the first token satisfying one of two conditions.
int mj_tlist_scan_until(mj_tlist *, bool (*target)(mj_token *),
    bool (*until)(mj_token *));

/* Writing functions */

// Appends a new token to the list.
int mj_tlist_append_new(mj_tlist *, token_t);

// Appends a character to the last token in the list.
int mj_tlist_append_char(mj_tlist *, char);

#endif
