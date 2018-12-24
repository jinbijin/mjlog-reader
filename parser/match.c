/* `match.c`, by Jinbi Jin */

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

#include "round.h"
#include "match.h"

#define INITIAL_ALLOC_MATCH 16

/* `mj_match` functions */

void mj_match_init(mj_match *this) {
  ptrdiff_t i;
  if (this == NULL) {
    mj_error_null_pointer(); // Exits.
  }
  this->match = 0;
  this->lobby = 0;
  for (i = 0; i < 4; i++) {
    this->rank[i] = 0;
    this->rating[i] = 0;
    this->score[i] = 0;
  }
  this->alloc = INITIAL_ALLOC_MATCH;
  this->size = 0;
  this->round = malloc(sizeof(*(this->round)) * this->alloc);
  if (this->round == NULL) {
    mj_error_alloc(); // Exits.
  }
  this->round[this->size] = NULL;
}

void mj_match_final(mj_match *this) {
  ptrdiff_t i;
  if (this != NULL) {
    for (i = 0; i < this->size; i++) {
      mj_round_final(this->round[i]);
      free(this->round[i]);
    }
    free(this->round);
  }
}

void mj_match_print_lines(const mj_match *this) {
  ptrdiff_t i;
  if (this == NULL) {
    mj_error_null_pointer(); // Exits.
  }
  printf("====================\n");
  printf("Type: %d\n",this->match);
  printf("Lobby: %d\n",this->lobby);
  printf("Ranks: %d, %d, %d, %d\n",
      this->rank[0],
      this->rank[1],
      this->rank[2],
      this->rank[3]);
  printf("Ratings: %d, %d, %d, %d\n",
      this->rating[0],
      this->rating[1],
      this->rating[2],
      this->rating[3]);
  printf("Final scores: %d, %d, %d, %d\n",
      this->score[0],
      this->score[1],
      this->score[2],
      this->score[3]);
  printf("--------------------\n");
  for (i = 0; i < this->size; i++) {
    mj_round_print_lines(this->round[i]);
  }
  printf("====================\n");
}

/* Writing functions */

// Appends and initialises a round.
void mj_match_append_round(mj_match *this) {
  if (this == NULL) {
    mj_error_null_pointer(); // Exits.
  }
  mj_round *round = malloc(sizeof(*round));
  if (round == NULL) {
    mj_error_alloc(); // Exits.
  }
  mj_round_init(round);

  this->size++;
  // Reallocate if needed to satisfy `0 <= size < alloc`.
  if (this->size >= this->alloc) {
    this->alloc *= 2;
    this->round = 
      realloc(this->round, sizeof(*(this->round)) * this->alloc);
    if (this->round == NULL) {
      mj_error_alloc(); // Exits.
    }
  }
  // Write
  this->round[this->size - 1] = round;
  this->round[this->size] = NULL;
}

// Appends an event to the last initialised round.
void mj_match_append_event(mj_match *this) {
  if (this == NULL) {
    mj_error_null_pointer(); // Exits.
  }
  if (this->size == 0) {
    mj_error_sub_append_empty(); // Exits.
  }
  mj_round_append_event(this->round[this->size - 1]);
}

// Appends a terminator to the last initialised round.
void mj_match_append_term(mj_match *this) {
  if (this == NULL) {
    mj_error_null_pointer(); // Exits.
  }
  if (this->size == 0) {
    mj_error_sub_append_empty(); // Exits.
  }
  mj_round_append_term(this->round[this->size - 1]);
}
