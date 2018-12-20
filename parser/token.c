/* `token.c`, by Jinbi Jin */

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>

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
int mj_token_init(mj_token *this, token_t type) {
  if (this == NULL) {
    printf("Attempt to modify a NULL pointer.\n");
    return EXIT_FAILURE;
  }
  this->type = type;
  this->alloc = INITIAL_ALLOC_TOKEN;
  this->size = 0;
  this->content = malloc(sizeof(*(this->content)) * this->alloc);
  if (this->content == NULL) {
    printf("Allocation failure.\n");
    return EXIT_FAILURE;
  }
  this->content[this->size] = '\0';
  return EXIT_SUCCESS;
}

// Frees memory from a token.
void mj_token_final(mj_token *this) {
  if (this != NULL) {
    free(this->content);
  }
}

// Prints a token to console.
void mj_token_print(mj_token *this) {
  if (this != NULL) {
    token_t_print(this->type);
    printf(" %s",this->content);
  }
}

// Prints a token and a '\n' to console.
void mj_token_print_line(mj_token *this) {
  if (this != NULL) {
    mj_token_print(this);
    printf("\n");
  }
}

/* Type checking functions */

// Checks whether a token has a certain flag and content.
bool mj_token_is(mj_token *this, token_t type, char *string) {
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
bool mj_token_is_discard(mj_token *this) {
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
bool mj_token_is_draw(mj_token *this) {
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
bool mj_token_is_null(mj_token *this) {
  if (this == NULL) {
    return true;
  }
  else {
    return false;
  }
}

// Checks whether a token represents an `INIT` tag.
bool mj_token_is_init(mj_token *this) {
  return mj_token_is(this, TOK_TAG, "INIT");
}

// Checks whether a token represents an event, i.e. one of
// * a discard;
// * a draw;
// * a dora reveal;
// * a call;
// * a riichi declaration.
bool mj_token_is_event(mj_token *this) {
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
bool mj_token_is_terminal(mj_token *this) {
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

/* Writing functions */

// Appends a `char` to the token.
int mj_token_append(mj_token *this, char c) {
  if (this == NULL) {
    printf("Attempt to modify a NULL pointer.\n");
    return EXIT_FAILURE;
  }
  if (c != '\0') {
    this->size++;
    // Reallocate if needed to satisfy `0 <= size < alloc`.
    if (this->size >= this->alloc) {
      this->alloc *= 2;
      this->content = 
	realloc(this->content, sizeof(*(this->content)) * this->alloc);
      if (this->content == NULL) {
	printf("Reallocation failure.\n");
	return EXIT_FAILURE;
      }
    }
    // Write
    this->content[this->size - 1] = c;
    this->content[this->size] = '\0';
  }
  return EXIT_SUCCESS;
}
