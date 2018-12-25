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
    mj_hand_print_line(this->hand[i]);
  }
  for (i = 0; i < this->event_size; i++) {
    mj_event_print_line(this->event[i]);
  }
  for (i = 0; i < this->term_size; i++) {
    mj_term_print_line(this->term[i]);
  }
}
