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
#include "lexer.h"

// Loading of `ferrorloc`.
char errfile[MJ_PATH_LIMIT] = "";
ptrdiff_t errloc;

typedef enum {
  LEX_NM,
  LEX_VAL
} lexer_m;

lexer_m mode_toggle (lexer_m m) {
  switch (m) {
    case LEX_NM:
      return LEX_VAL;
      break;
    case LEX_VAL:
      return LEX_NM;
      break;
    default:
      assert(false);
  }
}

bool is_alphanumeric (char c) {
  return (('a' <= c) && (c <= 'z')) ||
    (('A' <= c) && (c <= 'Z')) ||
    (('0' <= c) && (c <= '9'));
}

bool is_special (char c) {
  return (c == '<' || c == '>' || c == '/' || c == '=');
}

bool is_whitespace (char c) {
  return (c == ' ' || c == '\t' || c == '\r' || c == '\n');
}

bool is_toggle (char c) {
  return (c == '"');
}

bool is_separator (lexer_m m, char c) {
  switch (m) {
    case LEX_NM:
      return is_whitespace(c);
      break;
    case LEX_VAL:
      return (c == ',');
      break;
    default:
      assert(false);
  }
}

int lexer_loop (mj_tlist **tlist, lexer_m mode, FILE *file) {
  int i = getc(file);
  char c;
  lexer_m new = mode;
  if (i == EOF) {
    if (feof(file)) {
      tlist_current_delete_empty(*tlist);
      return EXIT_SUCCESS;
    }
    else
    {
      error_file_read();
    }
  }
  c = (char) i;
  if (is_toggle(c)) {
    new = mode_toggle(new);
    switch (tlist_current_type_get(*tlist)) {
      case TOK_EMPTY:
      break;
      case TOK_ID:
      case TOK_SYM:
      case TOK_VAL:
      tlist_append_new_token(tlist);
      break;
      default:
      assert(false);
    }
  }
  else {
    switch (tlist_current_type_get(*tlist)) {
      case TOK_EMPTY:
        if (!is_separator(mode, c)) {
          if (mode == LEX_NM && is_alphanumeric(c)) {
            tlist_current_type_set(*tlist, TOK_ID);
            tlist_append_char(*tlist, c);
          }
          else if (mode == LEX_NM && is_special(c)) {
            tlist_current_type_set(*tlist, TOK_SYM);
            tlist_append_char(*tlist, c);
          }
          else if (mode == LEX_VAL) {
            tlist_current_type_set(*tlist, TOK_VAL);
            tlist_append_char(*tlist, c);
          }
          else {
            error_unexpected_char(c);
          }
        }
        break;
      case TOK_ID:
        assert(mode == LEX_NM);
        if (is_alphanumeric(c)) {
          tlist_append_char(*tlist, c);
        }
        else if (is_separator(mode, c)) {
          tlist_append_new_token(tlist);
        }
        else if (is_special(c)) {
          tlist_append_new_token(tlist);
          tlist_current_type_set(*tlist, TOK_SYM);
          tlist_append_char(*tlist, c);
        }
        else {
          error_unexpected_char(c);
        }
        break;
      case TOK_SYM:
        assert(mode == LEX_NM);
        if (c == '>') {
          tlist_append_char(*tlist, c);
          tlist_append_new_token(tlist);
        }
        else if (is_special(c)) {
          tlist_append_char(*tlist, c);
        }
        else if (is_separator(mode, c)) {
          tlist_append_new_token(tlist);
        }
        else if (is_alphanumeric(c)) {
          tlist_append_new_token(tlist);
          tlist_current_type_set(*tlist, TOK_ID);
          tlist_append_char(*tlist, c);
        }
        else {
          error_unexpected_char(c);
        }
        break;
      case TOK_VAL:
        assert(mode == LEX_VAL);
        if (is_separator(mode, c)) {
          tlist_append_new_token(tlist);
        }
        else {
          tlist_append_char(*tlist, c);
        }
        break;
      default:
        assert(false);
    }
  }
  errloc++;
  lexer_loop(tlist, new, file);
}

void lexer (mj_tlist **tlist, const char *filename) {
  strcpy(errfile, filename);
  errloc = 0;
  FILE *file = fopen(filename, "r");
  assert(file != NULL);
  tlist_init(tlist);
  tlist_append_new_token(tlist);
  lexer_loop (tlist, LEX_NM, file);
}
