#ifndef _MJ_FERRORLOC_H_
#define _MJ_FERRORLOC_H_

#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MJ_PATH_LIMIT 256

extern char errfile[MJ_PATH_LIMIT];
extern ptrdiff_t errloc;

// Unexpected character lexer error.
void error_unexpected_char (char);

// File read error.
void error_file_read ();

#endif
