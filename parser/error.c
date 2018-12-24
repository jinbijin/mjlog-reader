/* `error.c`, by Jinbi Jin */

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

void mj_error_alloc() {
  fprintf(stderr, "Error: Allocation failure.\n");
  exit(EXIT_FAILURE);
}

void mj_error_null_pointer() {
  fprintf(stderr, "Error: Attempt to access a null pointer.\n");
  exit(EXIT_FAILURE);
}

void mj_error_sub_append_empty() {
  fprintf(stderr, "Error: No item to append to.\n");
  exit(EXIT_FAILURE);
}
