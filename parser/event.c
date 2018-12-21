/* `event.c`, by Jinbi Jin */

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "event.h"

/* `mj_event` functions */

// Placeholder print function for debugging.
// Eventually will print better information.
void mj_event_print(mj_event *this) {
  if (this != NULL) {
    switch (this->type) {
      case EVENT_DRAW:
	printf("DRAW");
	break;
      case EVENT_DISCARD:
	printf("DISCARD");
	break;
      case EVENT_CALL:
	printf("CALL");
	break;
      case EVENT_DORA:
	printf("DORA");
	break;
      case EVENT_RIICHI:
	printf("RIICHI");
	break;
    }
    printf(" %d %d", this->who, this->info);
  }
}

void mj_event_print_line(mj_event *this) {
  mj_event_print(this);
  printf("\n");
}
