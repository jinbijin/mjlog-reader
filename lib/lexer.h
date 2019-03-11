// The lexer for the interpreter.
// It has two modes:
// * In normal mode, allowed characters are [a-zA-Z0-9], <, >, /, =, and
//   whitespace.
//   > acts as terminator, whitespace acts as separator, and " acts as
//   mode toggler.
// * In value mode, any character is allowed.
//   , acts as separator, and " acts as mode toggler.
#ifndef _MJ_LEXER_H_
#define _MJ_LEXER_H_

#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "ferrorloc.h"
#include "dstr.h"
#include "token.h"
#include "tlist.h"

// The lexer. Requires first argument to only be declared (so no init has run).
void lexer(mj_tlist **, const char *);

#endif
