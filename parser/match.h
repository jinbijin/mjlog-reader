/* `match.h`, by Jinbi Jin */

#ifndef __MATCH_H_
#define __MATCH_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "round.h"

// The history of a match, and the output type of the parser.
typedef struct {
  uint16_t match;
  uint16_t lobby;
  uint8_t rank[4];
  uint32_t rating[4]; // 100 times the Tenhou rating
  int16_t score[4]; // 10 times the final score
  size_t alloc;
  size_t size;
  mj_round **round;
} mj_match;

#endif
