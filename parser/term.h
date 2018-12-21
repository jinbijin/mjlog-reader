/* `term.h`, by Jinbi Jin 
 *
 * Defines the `mj_term` type.
 * It will occur for the purposes as a parser in a dynamic array, therefore
 * as pointer. */

#ifndef __TERM_H_
#define __TERM_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The end-of-round ("terminator") flag.
typedef enum {
  TERM_AGARI,
  TERM_RYUUKYOKU
} mj_term_t;

// The end of a round.
// @who The winning player, if any.
// @from The player who dealt in, if any. In case of tsumo, `from == who`.
// @pao The player liable through pao rule. In case of no pao, `pao == from`.
// @value The ID of the value (related to the 1-han-equivalent fu).
// @score_diff The differences in score incurred by *this event*.
typedef struct {
  mj_term_t type;
  uint8_t who : 2;
  uint8_t from : 2;
  uint8_t pao : 2;
  uint8_t value;
  int16_t score_diff[4];
} mj_term;

/* `mj_term` functions */

void mj_term_print(mj_term *);

void mj_term_print_line(mj_term *);

#endif
