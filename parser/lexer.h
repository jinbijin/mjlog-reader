/* `lexer.h`, by Jinbi Jin 
 *
 * A lexer for `.mjlog` files. */

#ifndef __LEXER_H_
#define __LEXER_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>

#include "token.h"
#include "tlist.h"

int lexer(mj_tlist *, char *);

#endif
