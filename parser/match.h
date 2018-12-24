/* `match.h`, by Jinbi Jin */

#ifndef __MATCH_H_
#define __MATCH_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

#include "round.h"

// The history of a match, and the output type of the parser.
typedef struct {
  uint16_t match;
  uint32_t lobby;
  uint8_t rank[4];
  uint32_t rating[4]; // 100 times the Tenhou rating
  int16_t score[4]; // 10 times the final score
  size_t alloc;
  size_t size;
  mj_round **round;
} mj_match;

/* `mj_match` functions */

void mj_match_init(mj_match *);

void mj_match_final(mj_match *);

void mj_match_print_lines(const mj_match *);

void mj_match_append_round(mj_match *);

void mj_match_append_event(mj_match *);

void mj_match_append_term(mj_match *);

#endif
