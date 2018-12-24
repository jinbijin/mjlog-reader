/* `round.h`, by Jinbi Jin */

#ifndef __ROUND_H_
#define __ROUND_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

#include "hand.h"
#include "event.h"
#include "term.h"

// The history of a round.
typedef struct {
  uint8_t round;
  uint8_t repeat;
  uint8_t deposit;
  uint8_t indicator;
  uint8_t dealer;
  int16_t score[4]; // 10 times the score
  mj_hand hand[4];
  size_t event_alloc;
  size_t event_size;
  mj_event **event;
  size_t term_alloc;
  size_t term_size;
  mj_term **term;
} mj_round;

/* `mj_round` functions */

void mj_round_init(mj_round *);

void mj_round_final(mj_round *);

void mj_round_print_lines(const mj_round *);

/* Writing functions */

// Appends an event to this round. This event will be allocated, but
// uninitialised.
void mj_round_append_event(mj_round *);

// Appends an end to this round. This end will be allocated, but uninitialised.
void mj_round_append_term(mj_round *);

#endif
