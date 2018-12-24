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

#include "error.h"

#include "token.h"
#include "tlist.h"

void lexer(mj_tlist *, char *);

#endif
