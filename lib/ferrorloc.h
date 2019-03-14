#ifndef _MJ_FERRORLOC_H_
#define _MJ_FERRORLOC_H_

#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dstr.h"

#define MJ_PATH_LIMIT 256

extern char errfile[MJ_PATH_LIMIT];
extern ptrdiff_t errloc;

// Lexer errors

// Unexpected character lexer error.
void error_unexpected_char (char);

// File read error.
void error_file_read ();

// Parser errors

// Unexpected end-of-file error.
void error_unexpected_eof (const dstr *);

// Unexpected token errors

void error_unexpected_token_tag_open (const dstr *);

void error_unexpected_token_name (const dstr *);

void error_unexpected_token_attr (const dstr *);

void error_unexpected_token_equal (const dstr *);

void error_unexpected_token_val (const dstr *);

void error_unmatched_close_tag (const dstr *, const dstr *);

void error_invalid_tag ();

void error_unexpected_attr (const dstr *);


#endif
