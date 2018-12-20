/* `lexer.c`, by Jinbi Jin */

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

void lexer_error_print(lexer_m mode, char h) {
  switch (mode) {
    case LEX_IDLE:
      printf("Expected '<'");
      break;
    case LEX_TAG_IDLE:
      printf("Expected '/' or a letter");
      break;
    case LEX_TAG_IDLE_2:
      printf("Expected a letter");
      break;
    case LEX_TAG_ALPHA:
      printf("Expected an alphanumeric character, ' ', or end-of-tag");
      break;
    case LEX_TAG_NUM:
      printf("Expected a numeric character, ' ', or end-of-tag");
      break;
    case LEX_TAG_END_IDLE:
      printf("Expected '>'");
      break;
    case LEX_FIELD_IDLE:
      printf("Expected a letter or end-of-tag");
      break;
    case LEX_FIELD_ALPHA:
      printf("Expected an alphanumeric character or '='");
      break;
    case LEX_FIELD_NUM:
      printf("Expected a numeric character or '='");
      break;
    case LEX_VALUE_IDLE:
      printf("Expected '\"'");
      break;
    case LEX_VALUE:
      // This shouldn't be reached.
      break;
    case LEX_VALUE_END_IDLE:
      printf("Expected ' ' or end-of-tag");
      break;
  }
  printf(", got '%c'.\n",h);
}

// The recursive part of the lexer.
// Contains a big bad ugly conditional (for now?).
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
	  lexer_error_print(mode, h);
	  return EXIT_FAILURE;
      }
      break;
    case LEX_TAG_IDLE:
      switch (classify_char(h)) {
	case CHAR_SLASH:
	  new_mode = LEX_TAG_IDLE_2;
	  break;
	case CHAR_ALPHA:
	  new_mode = LEX_TAG_ALPHA;
	  if (mj_tlist_append_new(this, TOK_TAG) == EXIT_FAILURE) {
	    return EXIT_FAILURE;
	  }
	  if (mj_tlist_append_char(this, h) == EXIT_FAILURE) {
	    return EXIT_FAILURE;
	  }
	  break;
	default:
	  lexer_error_print(mode, h);
	  return EXIT_FAILURE;
      }
      break;
    case LEX_TAG_IDLE_2:
      switch (classify_char(h)) {
	case CHAR_ALPHA:
	  new_mode = LEX_TAG_ALPHA;
	  if (mj_tlist_append_new(this, TOK_TAG_END) == EXIT_FAILURE) {
	    return EXIT_FAILURE;
	  }
	  if (mj_tlist_append_char(this, h) == EXIT_FAILURE) {
	    return EXIT_FAILURE;
	  }
	  break;
	default:
	  lexer_error_print(mode, h);
	  return EXIT_FAILURE;
      }
      break;
    case LEX_TAG_ALPHA:
      switch (classify_char(h)) {
	case CHAR_NUM:
	  new_mode = LEX_TAG_NUM; // Yes, no `break` here.
	case CHAR_ALPHA:
	  if (mj_tlist_append_char(this, h) == EXIT_FAILURE) {
	    return EXIT_FAILURE;
	  }
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
	  lexer_error_print(mode, h);
	  return EXIT_FAILURE;
      }
      break;
    case LEX_TAG_NUM:
      switch (classify_char(h)) {
	case CHAR_NUM:
	  if (mj_tlist_append_char(this, h) == EXIT_FAILURE) {
	    return EXIT_FAILURE;
	  }
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
	  lexer_error_print(mode, h);
	  return EXIT_FAILURE;
      }
      break;
    case LEX_TAG_END_IDLE:
      switch (classify_char(h)) {
	case CHAR_CLOSE_ANGLE:
	  new_mode = LEX_IDLE;
	  break;
	default:
	  lexer_error_print(mode, h);
	  return EXIT_FAILURE;
      }
      break;
    case LEX_FIELD_IDLE:
      switch (classify_char(h)) {
	case CHAR_ALPHA:
	  new_mode = LEX_FIELD_ALPHA;
	  if (mj_tlist_append_new(this, TOK_FIELD) == EXIT_FAILURE) {
	    return EXIT_FAILURE;
	  }
	  if (mj_tlist_append_char(this, h) == EXIT_FAILURE) {
	    return EXIT_FAILURE;
	  }
	  break;
	case CHAR_SLASH:
	  new_mode = LEX_TAG_END_IDLE;
	  break;
	case CHAR_CLOSE_ANGLE:
	  new_mode = LEX_IDLE;
	  break;
	default:
	  lexer_error_print(mode, h);
	  return EXIT_FAILURE;
      }
      break;
    case LEX_FIELD_ALPHA:
      switch (classify_char(h)) {
	case CHAR_NUM:
	  new_mode = LEX_FIELD_NUM; // Yes, no `break`.
	case CHAR_ALPHA:
	  if (mj_tlist_append_char(this, h) == EXIT_FAILURE) {
	    return EXIT_FAILURE;
	  }
	  break;
	case CHAR_EQUAL:
	  new_mode = LEX_VALUE_IDLE;
	  break;
	default:
	  lexer_error_print(mode, h);
	  return EXIT_FAILURE;
      }
      break;
    case LEX_FIELD_NUM:
      switch (classify_char(h)) {
	case CHAR_NUM:
	  if (mj_tlist_append_char(this, h) == EXIT_FAILURE) {
	    return EXIT_FAILURE;
	  }
	  break;
	case CHAR_EQUAL:
	  new_mode = LEX_VALUE_IDLE;
	  break;
	default:
	  lexer_error_print(mode, h);
	  return EXIT_FAILURE;
      }
      break;
    case LEX_VALUE_IDLE:
      switch (classify_char(h)) {
	case CHAR_QUOTE:
	  new_mode = LEX_VALUE;
	  if (mj_tlist_append_new(this, TOK_VALUE) == EXIT_FAILURE) {
	    return EXIT_FAILURE;
	  }
	  break;
	default:
	  lexer_error_print(mode, h);
	  return EXIT_FAILURE;
      }
      break;
    case LEX_VALUE:
      switch (classify_char(h)) {
	case CHAR_COMMA:
	  if (mj_tlist_append_new(this, TOK_VALUE) == EXIT_FAILURE) {
	    return EXIT_FAILURE;
	  }
	  break;
	case CHAR_QUOTE:
	  new_mode = LEX_VALUE_END_IDLE;
	  break;
	default:
	  if (mj_tlist_append_char(this, h) == EXIT_FAILURE) {
	    return EXIT_FAILURE;
	  }
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

int lexer(mj_tlist *this, char *filename) {
  int i;
  char h;
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Failed to open file %s.\n",filename);
    return EXIT_FAILURE;
  }
  if (mj_tlist_init(this) == EXIT_FAILURE) {
    return EXIT_FAILURE;
  }

  i = fgetc(file);
  if (i != EOF) {
    h = (char)i;
  }
  else {
    printf("Unexpected end-of-file.\n");
    return EXIT_FAILURE;
  }

  if (lexer_loop(this, LEX_IDLE, h, file) == EXIT_FAILURE) {
    return EXIT_FAILURE;
  }

  fclose(file);
  return EXIT_SUCCESS;
}
