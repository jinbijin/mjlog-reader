#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "parser.h"

#include "mjquery.h"

void mj_result_init(mj_result *this) {
  ptrdiff_t i;
  for (i = 0; i < 4; i++) {
    this->total[i] = 0;
  }
  this->count = 0;
}

void mj_result_final(mj_result *this) {
}

void mj_result_update_by_match(mj_result *this, const mj_match *match) {
  int64_t score[4];
  ptrdiff_t i, j;
  int64_t temp;
  for (i = 0; i < 4; i++) {
    score[i] = (int64_t)(match->score[i]);
  }
  // Hard-code a sorting circuit for insertion sort, since size is 4
  for (i = 0; i < 3; i++) {
    for (j = i+1; j < 4; j++) {
      if (score[i] < score[j]) {
        temp = score[i];
        score[i] = score[j];
        score[j] = temp;
      }
    }
  }
  // Update results
  for (i = 0; i < 4; i++) {
    this->total[i] += score[i];
  }
  this->count++;
}

void mj_result_print_to_csv(const mj_result *this, const char *filename) {
  FILE *csv = fopen(filename,"w");
  ptrdiff_t i;
  if (csv == NULL) {
    fprintf(stderr,"Failed to open file.\n");
    exit(EXIT_FAILURE);
  }
  for (i = 0; i < 4; i++) {
    fprintf(csv, "%td\n", this->total[i]);
  }
  fprintf(csv, "%td\n", this->count);
}
