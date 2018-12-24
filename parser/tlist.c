/* `tlist.c`, by Jinbi Jin */

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

#include "token.h"
#include "tlist.h"

// This needs to be greater than `0`!
#define INITIAL_ALLOC_TLIST 32

/* `mj_tlist` functions */

// Allocates memory for a `tlist`.
void mj_tlist_init(mj_tlist *this) {
  if (this == NULL) {
    mj_error_null_pointer(); // Exits.
  }
  this->alloc = INITIAL_ALLOC_TLIST;
  this->size = 0;
  this->content = malloc(sizeof(*(this->content)) * this->alloc);
  if (this->content == NULL) {
    mj_error_alloc(); // Exits.
  }
  this->content[this->size] = NULL;
  this->head = 0;
}

// Frees memory from a `tlist`.
void mj_tlist_final(mj_tlist *this) {
  ptrdiff_t i;
  if (this != NULL) {
    for (i = 0; i < this->size; i++) {
      mj_token_final(this->content[i]);
      free(this->content[i]);
    }
    free(this->content);
  }
}

// Prints a `tlist` to console.
void mj_tlist_print_lines(const mj_tlist *this) {
  ptrdiff_t i;
  if (this == NULL) {
    mj_error_null_pointer(); // Exits.
  }
  for (i = this->head; i < this->size; i++) {
    mj_token_print_line(this->content[i]);
  }
}

/* Scanning functions */

// Moves to the next token in the list.
void mj_tlist_next(mj_tlist *this) {
  if (this == NULL) {
    mj_error_null_pointer(); // Exits.
  }
  if (this->head < this->size) {
    this->head++;
  }
}

// Moves a number of tokens forward in the list.
void mj_tlist_move(mj_tlist *this, ptrdiff_t n) {
  ptrdiff_t i;
  for (i = 0; i < n; i++) {
    mj_tlist_next(this);
  }
}

// Moves to the first token satisfying one of two conditions, and returns
// `false` if the second is satisfied, and `true` if the first is.
// The latter is to be interpreted as the end of a "unit" in the list.
bool mj_tlist_scan_until(mj_tlist *this, bool (*target)(const mj_token *),
    bool (*until)(const mj_token *)) {
  if (this == NULL) {
    mj_error_null_pointer(); // Exits.
  }
  while (true) {
    if ((*until)(this->content[this->head])) {
      return false;
    }
    if (this->content[this->head] == NULL) {
      mj_error_unexpected_eof(); // Exits.
    }
    if ((*target)(this->content[this->head])) {
      return true;
    }
    mj_tlist_next(this);
  }
}

/* Writing functions */

// Appends a new token to the list.
void mj_tlist_append_new(mj_tlist *this, token_t type) {
  if (this == NULL) {
    mj_error_null_pointer(); // Exits.
  }
  mj_token *token = malloc(sizeof(*token));
  if (token == NULL) {
    mj_error_alloc(); // Exits.
  }
  mj_token_init(token, type);

  this->size++;
  // Reallocate if needed to satisfy `0 <= size < alloc`.
  if (this->size >= this->alloc) {
    this->alloc *= 2;
    this->content = 
      realloc(this->content, sizeof(*(this->content)) * this->alloc);
    if (this->content == NULL) {
      mj_error_alloc(); // Exits.
    }
  }
  // Write
  this->content[this->size - 1] = token;
  this->content[this->size] = NULL;
}

// Appends a character to the last token in the list.
void mj_tlist_append_char(mj_tlist *this, char c) {
  if (this == NULL) {
    mj_error_null_pointer(); // Exits.
  }
  if (this->size == 0) {
    mj_error_sub_append_empty(); // Exits.
  }
  mj_token_append(this->content[this->size - 1], c);
}
