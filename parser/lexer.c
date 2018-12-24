/* `lexer.c`, by Jinbi Jin */

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

#include "token.h"
#include "tlist.h"
#include "lexer.h"

// The lexer mode.
typedef enum {
  LEX_IDLE,
  LEX_TAG_IDLE,
  LEX_TAG_IDLE_2,
  LEX_TAG_ALPHA,
  LEX_TAG_NUM,
  LEX_TAG_END_IDLE,
  LEX_FIELD_IDLE,
  LEX_FIELD_ALPHA,
  LEX_FIELD_NUM,
  LEX_VALUE_IDLE,
  LEX_VALUE,
  LEX_VALUE_END_IDLE
} lexer_m;

// Basic classes of characters.
typedef enum {
  CHAR_OPEN_ANGLE,
  CHAR_SLASH,
  CHAR_CLOSE_ANGLE,
  CHAR_SPACE,
  CHAR_EQUAL,
  CHAR_QUOTE,
  CHAR_COMMA,
  CHAR_ALPHA,
  CHAR_NUM,
  CHAR_OTHER
} char_t;

char_t classify_char(char h) {
  if (h >= 'a' && h <= 'z') {
    return CHAR_ALPHA;
  }
  else if (h >= 'A' && h <= 'Z') {
    return CHAR_ALPHA;
  }
  else if (h >= '0' && h <= '9') {
    return CHAR_NUM;
  }
  else if (h == '<') {
    return CHAR_OPEN_ANGLE;
  }
  else if (h == '/') {
    return CHAR_SLASH;
  }
  else if (h == '>') {
    return CHAR_CLOSE_ANGLE;
  }
  else if (h == ' ') {
    return CHAR_SPACE;
  }
  else if (h == '=') {
    return CHAR_EQUAL;
  }
  else if (h == '"') {
    return CHAR_QUOTE;
  }
  else if (h == ',') {
    return CHAR_COMMA;
  }
  else {
    return CHAR_OTHER;
  }
}

void lexer_error(lexer_m mode, char h) {
  switch (mode) {
    case LEX_IDLE:
      fprintf(stderr,
	  "Error: Expected '<'");
      break;
    case LEX_TAG_IDLE:
      fprintf(stderr,
	  "Error: Expected '/' or a letter");
      break;
    case LEX_TAG_IDLE_2:
      fprintf(stderr,
	  "Error: Expected a letter");
      break;
    case LEX_TAG_ALPHA:
      fprintf(stderr,
	  "Error: Expected an alphanumeric character, ' ', or end-of-tag");
      break;
    case LEX_TAG_NUM:
      fprintf(stderr,
	  "Error: Expected a numeric character, ' ', or end-of-tag");
      break;
    case LEX_TAG_END_IDLE:
      fprintf(stderr,
	  "Error: Expected '>'");
      break;
    case LEX_FIELD_IDLE:
      fprintf(stderr,
	  "Error: Expected a letter or end-of-tag");
      break;
    case LEX_FIELD_ALPHA:
      fprintf(stderr,
	  "Error: Expected an alphanumeric character or '='");
      break;
    case LEX_FIELD_NUM:
      fprintf(stderr,
	  "Error: Expected a numeric character or '='");
      break;
    case LEX_VALUE_IDLE:
      fprintf(stderr,
	  "Error: Expected '\"'");
      break;
    case LEX_VALUE:
      // This shouldn't be reached.
      break;
    case LEX_VALUE_END_IDLE:
      fprintf(stderr,
	  "Error: Expected ' ' or end-of-tag");
      break;
  }
  fprintf(stderr, ", got '%c'.\n",h);
  exit(EXIT_FAILURE);
}

// The recursive part of the lexer.
// Contains a big bad ugly conditional.
// I am unsure whether I can turn this into a void while still keeping
// tail call optimisation working.
int lexer_loop(mj_tlist *this, lexer_m mode, char h, FILE *file) {
  int i;
  lexer_m new_mode = mode;
  char new_h = h;

  switch (mode) {
    case LEX_IDLE:
      switch (classify_char(h)) {
	case CHAR_OPEN_ANGLE:
	  new_mode = LEX_TAG_IDLE;
	  break;
	default:
	  lexer_error(mode, h); // Exits.
      }
      break;
    case LEX_TAG_IDLE:
      switch (classify_char(h)) {
	case CHAR_SLASH:
	  new_mode = LEX_TAG_IDLE_2;
	  break;
	case CHAR_ALPHA:
	  new_mode = LEX_TAG_ALPHA;
	  mj_tlist_append_new(this, TOK_TAG);
	  mj_tlist_append_char(this, h);
	  break;
	default:
	  lexer_error(mode, h); // Exits.
      }
      break;
    case LEX_TAG_IDLE_2:
      switch (classify_char(h)) {
	case CHAR_ALPHA:
	  new_mode = LEX_TAG_ALPHA;
	  mj_tlist_append_new(this, TOK_TAG_END);
	  mj_tlist_append_char(this, h);
	  break;
	default:
	  lexer_error(mode, h); // Exits.
      }
      break;
    case LEX_TAG_ALPHA:
      switch (classify_char(h)) {
	case CHAR_NUM:
	  new_mode = LEX_TAG_NUM; // Yes, no `break` here.
	case CHAR_ALPHA:
	  mj_tlist_append_char(this, h);
	  break;
	case CHAR_SPACE:
	  new_mode = LEX_FIELD_IDLE;
	  break;
	case CHAR_SLASH:
	  new_mode = LEX_TAG_END_IDLE;
	  break;
	case CHAR_CLOSE_ANGLE:
	  new_mode = LEX_IDLE;
	  break;
	default:
	  lexer_error(mode, h); // Exits.
      }
      break;
    case LEX_TAG_NUM:
      switch (classify_char(h)) {
	case CHAR_NUM:
	  mj_tlist_append_char(this, h);
	  break;
	case CHAR_SPACE:
	  new_mode = LEX_FIELD_IDLE;
	  break;
	case CHAR_SLASH:
	  new_mode = LEX_TAG_END_IDLE;
	  break;
	case CHAR_CLOSE_ANGLE:
	  new_mode = LEX_IDLE;
	  break;
	default:
	  lexer_error(mode, h); // Exits.
      }
      break;
    case LEX_TAG_END_IDLE:
      switch (classify_char(h)) {
	case CHAR_CLOSE_ANGLE:
	  new_mode = LEX_IDLE;
	  break;
	default:
	  lexer_error(mode, h); // Exits.
      }
      break;
    case LEX_FIELD_IDLE:
      switch (classify_char(h)) {
	case CHAR_ALPHA:
	  new_mode = LEX_FIELD_ALPHA;
	  mj_tlist_append_new(this, TOK_FIELD);
	  mj_tlist_append_char(this, h);
	  break;
	case CHAR_SLASH:
	  new_mode = LEX_TAG_END_IDLE;
	  break;
	case CHAR_CLOSE_ANGLE:
	  new_mode = LEX_IDLE;
	  break;
	default:
	  lexer_error(mode, h); // Exits.
      }
      break;
    case LEX_FIELD_ALPHA:
      switch (classify_char(h)) {
	case CHAR_NUM:
	  new_mode = LEX_FIELD_NUM; // Yes, no `break`.
	case CHAR_ALPHA:
	  mj_tlist_append_char(this, h);
	  break;
	case CHAR_EQUAL:
	  new_mode = LEX_VALUE_IDLE;
	  break;
	default:
	  lexer_error(mode, h); // Exits.
      }
      break;
    case LEX_FIELD_NUM:
      switch (classify_char(h)) {
	case CHAR_NUM:
	  mj_tlist_append_char(this, h);
	  break;
	case CHAR_EQUAL:
	  new_mode = LEX_VALUE_IDLE;
	  break;
	default:
	  lexer_error(mode, h); // Exits.
      }
      break;
    case LEX_VALUE_IDLE:
      switch (classify_char(h)) {
	case CHAR_QUOTE:
	  new_mode = LEX_VALUE;
	  mj_tlist_append_new(this, TOK_VALUE);
	  break;
	default:
	  lexer_error(mode, h); // Exits.
      }
      break;
    case LEX_VALUE:
      switch (classify_char(h)) {
	case CHAR_COMMA:
	  mj_tlist_append_new(this, TOK_VALUE);
	  break;
	case CHAR_QUOTE:
	  new_mode = LEX_VALUE_END_IDLE;
	  break;
	default:
	  mj_tlist_append_char(this, h);
	  break;
      }
      break;
    case LEX_VALUE_END_IDLE:
      switch (classify_char(h)) {
	case CHAR_SPACE:
	  new_mode = LEX_FIELD_IDLE;
	  break;
	case CHAR_SLASH:
	  new_mode = LEX_TAG_END_IDLE;
	  break;
	case CHAR_CLOSE_ANGLE:
	  new_mode = LEX_IDLE;
	  break;
	default:
	  lexer_error(mode, h); // Exits.
      }
      break;
  }

  i = fgetc(file);
  if (i != EOF) {
    new_h = (char)i;
  }
  else {
    return EXIT_SUCCESS;
  }
  return lexer_loop(this, new_mode, new_h, file);
}

void lexer(mj_tlist *this, char *filename) {
  int i;
  char h;
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    mj_error_file(filename); // Exits.
  }
  mj_tlist_init(this);

  i = fgetc(file);
  if (i != EOF) {
    h = (char)i;
  }
  else {
    mj_error_unexpected_eof(); // Exits.
  }

  lexer_loop(this, LEX_IDLE, h, file); // Only `return` value is `EXIT_SUCCESS`.

  fclose(file);
}
