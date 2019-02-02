#ifndef __MJQUERY_H_
#define __MJQUERY_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "parser.h"

// The type of the result of the query.
typedef struct {
  int64_t total[4];
  int64_t count;
} mj_result;

void mj_result_init(mj_result *);
void mj_result_final(mj_result *);

// Update the result given the history of a match.
void mj_result_update_by_match(mj_result *, const mj_match *);

// Print the output to a .csv file.
void mj_result_print_to_csv(const mj_result *, const char *);

#endif
