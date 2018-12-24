/* `round.c`, by Jinbi Jin */

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

#include "hand.h"
#include "event.h"
#include "term.h"
#include "round.h"

#define INITIAL_ALLOC_EVENT 32
#define INITIAL_ALLOC_TERM 4

void mj_round_init(mj_round *this) {
  int i;
  if (this == NULL) {
    mj_error_null_pointer(); // Exits.
  }
  this->round = 0;
  this->repeat = 0;
  this->deposit = 0;
  this->indicator = 0;
  this->dealer = 0;
  for (i = 0; i < 4; i++) {
    this->score[i] = 0;
  }
  for (i = 0; i < 4; i++) {
    this->hand[i] = mj_hand_init();
  }

  this->event_alloc = INITIAL_ALLOC_EVENT;
  this->event_size = 0;
  this->event = malloc((this->event_alloc) * sizeof(*(this->event)));
  if (this->event == NULL) {
    mj_error_alloc(); // Exits.
  }
  this->event[this->event_size] = NULL;

  this->term_alloc = INITIAL_ALLOC_TERM;
  this->term_size = 0;
  this->term = malloc((this->term_alloc) * sizeof(*(this->term)));
  if (this->term == NULL) {
    mj_error_alloc(); // Exits.
  }
  this->term[this->term_size] = NULL;
}

void mj_round_final(mj_round *this) {
  ptrdiff_t i;
  if (this != NULL) {
    for (i = 0; i < this->event_size; i++) {
      free(this->event[i]);
    }
    free(this->event);
    for (i = 0; i < this->term_size; i++) {
      free(this->term[i]);
    }
    free(this->term);
  }
}

void mj_round_print_lines(const mj_round *this) {
  ptrdiff_t i;
  if (this == NULL) {
    mj_error_null_pointer(); // Exits.
  }
  printf("--------------------\n");
  printf("Round: %d\n",this->round);
  printf("Repeat no.: %d\n",this->repeat);
  printf("Riichi deposits: %d\n",this->deposit);
  printf("Initial dora indicator: %d\n",this->indicator);
  printf("Dealer: Player %d\n",this->dealer);
  printf("Scores: %d, %d, %d, %d\n",
      this->score[0],
      this->score[1],
      this->score[2],
      this->score[3]);
  for (i = 0; i < 4; i++) {
    printf("Hand player %d: ",i);
    mj_hand_print(this->hand[i]);
  }
  for (i = 0; i < this->event_size; i++) {
    mj_event_print_line(this->event[i]);
  }
  for (i = 0; i < this->term_size; i++) {
    mj_term_print_line(this->term[i]);
  }
}

/* Writing functions */

// Appends an event to this round. This event will be allocated, but
// uninitialised.
void mj_round_append_event(mj_round *this) {
  if (this == NULL) {
    mj_error_null_pointer(); // Exits.
  }
  mj_event *event = malloc(sizeof(*event));
  if (event == NULL) {
    mj_error_alloc(); // Exits.
  }
  this->event_size++;
  // Reallocate if needed to satisfy `event_size < event_alloc`.
  if (this->event_size >= this->event_alloc) {
    this->event_alloc *= 2;
    this->event = 
      realloc(this->event, sizeof(*(this->event)) * (this->event_alloc));
    if (this->event == NULL) {
      mj_error_alloc(); // Exits.
    }
  }
  this->event[this->event_size - 1] = event;
  this->event[this->event_size] = NULL;
}

// Appends an end to this round. This end will be allocated, but uninitialised.
void mj_round_append_term(mj_round *this) {
  if (this == NULL) {
    mj_error_null_pointer(); // Exits.
  }
  mj_term *term = malloc(sizeof(*term));
  if (term == NULL) {
    mj_error_alloc(); // Exits.
  }
  this->term_size++;
  // Reallocate if needed to satisfy `term_size < term_alloc`.
  if (this->term_size >= this->term_alloc) {
    this->term_alloc *= 2;
    this->term = 
      realloc(this->term, sizeof(*(this->term)) * (this->term_alloc));
    if (this->term == NULL) {
      mj_error_alloc(); // Exits.
    }
  }
  this->term[this->term_size - 1] = term;
  this->term[this->term_size] = NULL;
}
