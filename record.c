#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "lib/dstr.h"
#include "lib/attr.h"
#include "lib/elt.h"
#include "lib/state.h"
#include "record.h"

// As a dummy implementation, a record currently is empty, i.e. all
// `mj_record *` will initialise as `NULL`

void record_init (mj_record **this) {
  *this = malloc(17 * sizeof(int64_t));
  ptrdiff_t i;
  for (i = 0; i < 17; i++) {
    ((int64_t *)(*this))[i] = 0;
  }
}

void record_delete (mj_record *this) {
  free(this);
}

void record_print_lines (const mj_record *this) {
  ptrdiff_t i, j;
  printf("%td\n", ((int64_t *)(this))[0]);
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      printf("%td", ((int64_t *)(this))[4*i + j + 1]);
      if (j < 3) {
        printf(", ");
      }
    }
    printf("\n");
  }
}

void record_update (mj_record **this, const mj_state *state, const mj_elt *elt, ptrdiff_t head) {
  ptrdiff_t i, j;
  // Trigger after updating using last element
  if (head >= elt_tsize_get(elt) - 1) {
    ((int64_t *)(*this))[0]++;
    for (i = 0; i < 4; i++) {
      for (j = 0; j < 4; j++) {
        ((int64_t *)(*this))[4*i+j+1] +=
          (state->score[i] - 250) * (state->score[j] - 250);
      }
    }
  }
}
