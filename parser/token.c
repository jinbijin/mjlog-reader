/* `token.c`, by Jinbi Jin */

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

// This needs to be greater than `0`!
#define INITIAL_ALLOC_TOKEN 32

/* `token_t` functions */

// Prints a token flag to console.
void token_t_print(token_t type) {
  switch (type) {
    case TOK_TAG:
      printf("TAG");
      break;
    case TOK_TAG_END:
      printf("TAG_END");
      break;
    case TOK_FIELD:
      printf("FIELD");
      break;
    case TOK_VALUE:
      printf("VALUE");
      break;
  }
}

/* `mj_token` functions */

// Allocates memory for an empty token.
// Its flag should be supplied immediately.
void mj_token_init(mj_token *this, token_t type) {
  if (this == NULL) {
    mj_error_null_pointer(); // Exits.
  }
  this->type = type;
  this->alloc = INITIAL_ALLOC_TOKEN;
  this->size = 0;
  this->content = malloc(sizeof(*(this->content)) * this->alloc);
  if (this->content == NULL) {
    mj_error_alloc(); // Exits.
  }
  this->content[this->size] = '\0';
}

// Frees memory from a token.
void mj_token_final(mj_token *this) {
  if (this != NULL) {
    free(this->content);
  }
}

// Prints a token to console.
void mj_token_print(const mj_token *this) {
  if (this == NULL) {
    mj_error_null_pointer(); // Exits.
  }
  token_t_print(this->type);
  printf(" %s",this->content);
}

// Prints a token and a '\n' to console.
void mj_token_print_line(const mj_token *this) {
  mj_token_print(this);
  printf("\n");
}

/* Type checking functions */

// Checks whether a token has a certain flag and content.
bool mj_token_is(const mj_token *this, token_t type, char *string) {
  if (this == NULL) {
    return false;
  }
  else if (this->type != type) {
    return false;
  }
  else if (strcmp(this->content,string) == 0) {
    return true;
  }
  else {
    return false;
  }
}

// Checks whether a token represents a discard;
// i.e. matches `[D-G][0-9]*`.
bool mj_token_is_discard(const mj_token *this) {
  ptrdiff_t i;
  if (this == NULL) {
    return false;
  }
  else if (this->type != TOK_TAG) {
    return false;
  }
  else if (this->size < 2) { // This check is here to make the next check safe.
    return false;
  }
  else if (this->content[0] < 'D' || this->content[0] > 'G') {
    return false;
  }
  else {
    for (i = 1; i < this->size; i++) {
      if (this->content[i] < '0' || this->content[i] > '9') {
	return false;
      }
    }
    return true;
  }
}

// Checks whether a token represents a draw;
// i.e. matches `[T-W][0-9]*`
bool mj_token_is_draw(const mj_token *this) {
  ptrdiff_t i;
  if (this == NULL) {
    return false;
  }
  else if (this->type != TOK_TAG) {
    return false;
  }
  else if (this->size < 2) { // This check is here to make the next check safe.
    return false;
  }
  else if (this->content[0] < 'T' || this->content[0] > 'W') {
    return false;
  }
  else {
    for (i = 1; i < this->size; i++) {
      if (this->content[i] < '0' || this->content[i] > '9') {
	return false;
      }
    }
    return true;
  }
}

/* Specialised `(*bool)(mj_token *)` functions. */

// Checks whether a token pointer is `NULL`.
bool mj_token_is_null(const mj_token *this) {
  if (this == NULL) {
    return true;
  }
  else {
    return false;
  }
}

// Checks whether a token represents a `GO` tag.
bool mj_token_is_go(const mj_token *this) {
  return mj_token_is(this, TOK_TAG, "GO");
}

// Checks whether a token represents an `INIT` tag.
bool mj_token_is_init(const mj_token *this) {
  return mj_token_is(this, TOK_TAG, "INIT");
}

// Checks whether a token represents an event, i.e. one of
// * a discard;
// * a draw;
// * a dora reveal;
// * a call;
// * a riichi declaration.
bool mj_token_is_event(const mj_token *this) {
  if (this == NULL ) {
    return false;
  }
  else if (this->type != TOK_TAG) {
    return false;
  }
  else if (mj_token_is_discard(this)) {
    return true;
  }
  else if (mj_token_is_draw(this)) {
    return true;
  }
  else if (strcmp(this->content,"DORA") == 0) {
    return true;
  }
  else if (strcmp(this->content,"N") == 0) {
    return true;
  }
  else if (strcmp(this->content,"REACH") == 0) {
    return true;
  }
  else {
    return false;
  }
}

// Checks whether a token represents an end of a round.
bool mj_token_is_term(const mj_token *this) {
  if (this == NULL) {
    return false;
  }
  else if (this->type != TOK_TAG) {
    return false;
  }
  else if (strcmp(this->content,"AGARI") == 0) {
    return true;
  }
  else if (strcmp(this->content,"RYUUKYOKU") == 0) {
    return true;
  }
  else {
    return false;
  }
}

// Checks whether a token represents the `sc` attribute.
bool mj_token_is_sc(const mj_token *this) {
  return mj_token_is(this, TOK_FIELD, "sc");
}

// Checks whether a token represents the `step` attribute.
bool mj_token_is_step(const mj_token *this) {
  return mj_token_is(this, TOK_FIELD, "step");
}

// Checks whether a token represents the `ten` attribute.
bool mj_token_is_ten(const mj_token *this) {
  return mj_token_is(this, TOK_FIELD, "ten");
}

// Checks whether a token represents the `who` attribute.
bool mj_token_is_who(const mj_token *this) {
  return mj_token_is(this, TOK_FIELD, "who");
}

// Checks whether a token represents the `owari` attribute.
bool mj_token_is_owari(const mj_token *this) {
  return mj_token_is(this, TOK_FIELD, "owari");
}

// Checks whether a token represents an `INIT` tag or `owari` attribute.
bool mj_token_is_init_or_owari(const mj_token *this) {
  return (mj_token_is_init(this) || mj_token_is_owari(this));
}

// Checks whether a token represents an end of a terminator;
// i.e. another terminator tag, `INIT`, or `owari`.
bool mj_token_is_term_ender(const mj_token *this) {
  return (mj_token_is_term(this) || mj_token_is_init_or_owari(this));
}

/* Writing functions */

// Appends a `char` to the token.
void mj_token_append(mj_token *this, char c) {
  if (this == NULL) {
    mj_error_null_pointer(); // Exits.
  }
  if (c != '\0') {
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
    this->content[this->size - 1] = c;
    this->content[this->size] = '\0';
  }
}
