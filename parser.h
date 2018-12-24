/* Tenhou mjlog parser, by Jinbi Jin */

#ifndef __PARSER_H_
#define __PARSER_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "parser/error.h"

#include "parser/hand.h"
#include "parser/event.h"
#include "parser/term.h"
#include "parser/round.h"
#include "parser/match.h"

void parser(mj_match *, const char *);

#endif
