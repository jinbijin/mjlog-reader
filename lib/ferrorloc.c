#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "ferrorloc.h"

void error_unexpected_char (char c) {
  fprintf(stderr, "Unexpected character '%c' in file \"%s\" at location %d.\n",
    c, errfile, errloc);
  exit(EXIT_FAILURE);
}

void error_file_read () {
  fprintf(stderr, "Error occurred while reading file \"%s\".\n", errfile);
  exit(EXIT_FAILURE);
}
