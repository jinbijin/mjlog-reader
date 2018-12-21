/* `event.h`, by Jinbi Jin */

#ifndef __EVENT_H_
#define __EVENT_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// An event flag.
typedef enum {
  EVENT_DRAW,
  EVENT_DISCARD,
  EVENT_CALL,
  EVENT_DORA,
  EVENT_RIICHI
} mj_event_t;

// An event.
// @info Additional information to the event if any, i.e. the "m" field for
// calls, the tile id for new dora indicators, or the step counter for riichi
// declarations.
typedef struct {
  mj_event_t type;
  uint8_t who;
  uint16_t info;
} mj_event;

/* `mj_event` functions */

// Placeholder print function for debugging.
void mj_event_print(mj_event *);

void mj_event_print_line(mj_event *);

#endif
