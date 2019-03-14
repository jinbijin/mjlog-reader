#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dstr.h"
#include "ferrorloc.h"

// Lexer errors

void error_unexpected_char (char c) {
  fprintf(stderr, "Unexpected character '%c' in file \"%s\" at location %d.\n",
    c, errfile, errloc);
  exit(EXIT_FAILURE);
}

void error_file_read () {
  fprintf(stderr, "Error occurred while reading file \"%s\".\n", errfile);
  exit(EXIT_FAILURE);
}

// Parser errors

void error_unexpected_eof (const dstr *tag) {
  char *content = dstr_content(tag);
  fprintf(stderr,
    "Reached end-of-file while inside element \"%s\" in file \"%s\".",
    content, errfile);
  exit(EXIT_FAILURE);
}

void error_unexpected_token_tag_open (const dstr *tag) {
  char *content = dstr_content(tag);
  fprintf(stderr,
    "Unexpected token \"%s\" while expecting tag-open in file \"%s\".",
    content, errfile);
  exit(EXIT_FAILURE);
}

void error_unexpected_token_name (const dstr *tag) {
  char *content = dstr_content(tag);
  fprintf(stderr,
    "Unexpected token \"%s\" while expecting tag name in file \"%s\".",
    content, errfile);
  exit(EXIT_FAILURE);
}

void error_unexpected_token_attr (const dstr *tag) {
  char *content = dstr_content(tag);
  fprintf(stderr,
    "Unexpected token \"%s\" while expecting attribute name or tag-end in file \"%s\".",
    content, errfile);
  exit(EXIT_FAILURE);
}

void error_unexpected_token_equal (const dstr *tag) {
  char *content = dstr_content(tag);
  fprintf(stderr,
    "Unexpected token \"%s\" while expecting \"=\" in file \"%s\".",
    content, errfile);
  exit(EXIT_FAILURE);
}

void error_unexpected_token_val (const dstr *tag) {
  char *content = dstr_content(tag);
  fprintf(stderr,
    "Unexpected token \"%s\" while expecting value or tag-end in file \"%s\".",
    content, errfile);
  exit(EXIT_FAILURE);
}

void error_unmatched_close_tag (const dstr *tag_open, const dstr *tag_close) {
  char *content_open = dstr_content(tag_open);
  char *content_close = dstr_content(tag_close);
  fprintf(stderr,
    "Tag opened with \"%s\" is closed with \"%s\" in file \"%s\".",
    content_open, content_close, errfile);
  exit(EXIT_FAILURE);
}

void error_invalid_tag () {
  fprintf(stderr, "Invalid tag </ ... /> in file \"%s\".", errfile);
  exit(EXIT_FAILURE);
}

void error_unexpected_attr (const dstr *attr) {
  char *content = dstr_content(attr);
  fprintf(stderr,
    "Unexpected attribute \"%s\" in a closing tag in file \"%s\".",
    content, errfile);
  exit(EXIT_FAILURE);
}
