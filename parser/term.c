/* `term.c`, by Jinbi Jin */

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "term.h"

/* `mj_term` functions */

// Placeholder print function for debugging.
void mj_term_print(const mj_term *this) {
  if (this == NULL) {
    mj_error_null_pointer();
  }
  switch (this->type) {
    case TERM_AGARI:
      printf("AGARI");
      break;
    case TERM_RYUUKYOKU:
      printf("RYUUKYOKU");
      break;
  }
  printf(" %d %d %d %d %d %d %d",
      this->who,
      this->from,
      this->pao,
      this->score_diff[0],
      this->score_diff[1],
      this->score_diff[2],
      this->score_diff[3]);
}

void mj_term_print_line(const mj_term *this) {
  mj_term_print(this);
  printf("\n");
}
