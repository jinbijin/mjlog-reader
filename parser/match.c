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

/* `mj_match` functions */

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
