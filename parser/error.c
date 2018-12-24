/* `error.c`, by Jinbi Jin */

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

void mj_error_preface() {
  fprintf(stderr, "In file %s:\n", mj_errfile);
}

void mj_error_alloc() {
  mj_error_preface();
  fprintf(stderr, "Error: Allocation failure.\n");
  exit(EXIT_FAILURE);
}

void mj_error_file(const char *filename) {
  mj_error_preface();
  fprintf(stderr, "Error: Cannot open file %s.\n", filename);
  exit(EXIT_FAILURE);
}

void mj_error_null_pointer() {
  mj_error_preface();
  fprintf(stderr, "Error: Attempt to access a null pointer.\n");
  exit(EXIT_FAILURE);
}

void mj_error_sub_append_empty() {
  mj_error_preface();
  fprintf(stderr, "Error: No item to append to.\n");
  exit(EXIT_FAILURE);
}

void mj_error_tile_overflow(const char *tilestring) {
  mj_error_preface();
  fprintf(stderr, "Error: Too many of tile %s.\n", tilestring);
  exit(EXIT_FAILURE);
}

void mj_error_tile_underflow(const char *tilestring) {
  mj_error_preface();
  fprintf(stderr, "Error: Too few of tile %s.\n", tilestring);
  exit(EXIT_FAILURE);
}

void mj_error_unexpected_eof() {
  mj_error_preface();
  fprintf(stderr, "Error: Unexpected EOF.\n");
  exit(EXIT_FAILURE);
}

void mj_error_unexpected_eof_while(const char* search) {
  mj_error_preface();
  fprintf(stderr, "Error: Unexpected EOF while looking for \"%s\".\n", search);
  exit(EXIT_FAILURE);
}

void mj_error_value_conversion() {
  mj_error_preface();
  fprintf(stderr, "Error: Failed to convert value to value id.\n");
  exit(EXIT_FAILURE);
}
