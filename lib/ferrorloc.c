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
    "Reached end-of-file while inside element \"%s\" in file \"%s\".\n",
    content, errfile);
  exit(EXIT_FAILURE);
}

void error_unexpected_token_tag_open (const dstr *tag) {
  char *content = dstr_content(tag);
  fprintf(stderr,
    "Unexpected token \"%s\" while expecting tag-open in file \"%s\".\n",
    content, errfile);
  exit(EXIT_FAILURE);
}

void error_unexpected_token_name (const dstr *tag) {
  char *content = dstr_content(tag);
  fprintf(stderr,
    "Unexpected token \"%s\" while expecting tag name in file \"%s\".\n",
    content, errfile);
  exit(EXIT_FAILURE);
}

void error_unexpected_token_attr (const dstr *tag) {
  char *content = dstr_content(tag);
  fprintf(stderr,
    "Unexpected token \"%s\" while expecting attribute name or tag-end in file \"%s\".\n",
    content, errfile);
  exit(EXIT_FAILURE);
}

void error_unexpected_token_equal (const dstr *tag) {
  char *content = dstr_content(tag);
  fprintf(stderr,
    "Unexpected token \"%s\" while expecting \"=\" in file \"%s\".\n",
    content, errfile);
  exit(EXIT_FAILURE);
}

void error_unexpected_token_val (const dstr *tag) {
  char *content = dstr_content(tag);
  fprintf(stderr,
    "Unexpected token \"%s\" while expecting value or tag-end in file \"%s\".\n",
    content, errfile);
  exit(EXIT_FAILURE);
}

void error_unmatched_close_tag (const dstr *tag_open, const dstr *tag_close) {
  char *content_open = dstr_content(tag_open);
  char *content_close = dstr_content(tag_close);
  fprintf(stderr,
    "Tag opened with \"%s\" is closed with \"%s\" in file \"%s\".\n",
    content_open, content_close, errfile);
  exit(EXIT_FAILURE);
}

void error_invalid_tag () {
  fprintf(stderr, "Invalid tag </ ... /> in file \"%s\".\n", errfile);
  exit(EXIT_FAILURE);
}

void error_unexpected_attr (const dstr *attr) {
  char *content = dstr_content(attr);
  fprintf(stderr,
    "Unexpected attribute \"%s\" in a closing tag in file \"%s\".\n",
    content, errfile);
  exit(EXIT_FAILURE);
}

// Interpreter errors

void error_root_tag () {
  fprintf(stderr,
    "Incorrect number of root elements in file \"%s\".\n",
    errfile);
  exit(EXIT_FAILURE);
}

void error_expected_tag (const char *exp, const dstr *name) {
  fprintf(stderr,
    "Expected element \"%s\", got element \"%s\" in file \"%s\".\n",
    exp, name, errfile);
  exit(EXIT_FAILURE);
}

void error_expected_attr (const char *exp, const dstr *name) {
  fprintf(stderr,
    "Expected attribute \"%s\", got attribute \"%s\" in file \"%s\".\n",
    exp, name, errfile);
  exit(EXIT_FAILURE);
}

void error_unexpected_end_of_game () {
  fprintf(stderr,
    "Unexpected end of game in file \"%s\".\n",
    errfile);
  exit(EXIT_FAILURE);
}

void error_missing_attr (const char *attr, const dstr *elt) {
  fprintf(stderr,
    "Missing attribute \"%s\" in element \"%s\" in file \"%s\".\n",
    attr, elt, errfile);
  exit(EXIT_FAILURE);
}

void error_duplicate_attr (const char *attr, const dstr *elt) {
  fprintf(stderr,
    "Duplicate attribute \"%s\" in element \"%s\" in file \"%s\".\n",
    attr, elt, errfile);
  exit(EXIT_FAILURE);
}

void error_missing_value (const dstr *attr, const dstr *elt) {
  fprintf(stderr,
    "Missing value for attribute \"%s\" of element \"%s\" in file \"%s\".\n",
    attr, elt, errfile);
  exit(EXIT_FAILURE);
}

void error_invalid_tile_id (const dstr *elt) {
  fprintf(stderr,
    "Invalid tile id in element \"%s\" in file \"%s\".\n",
    elt, errfile);
  exit(EXIT_FAILURE);
}

void error_invalid_player_id (const dstr *elt) {
  fprintf(stderr,
    "Invalid player id in element \"%s\" in file \"%s\".\n",
    elt, errfile);
  exit(EXIT_FAILURE);
}

void error_invalid_step (const dstr *elt) {
  fprintf(stderr,
    "Invalid step in element \"%s\" in file \"%s\".\n",
    elt, errfile);
  exit(EXIT_FAILURE);
}

void error_unavailable_tile (const dstr *elt) {
  char *content = dstr_content(elt);
  fprintf(stderr,
    "Unavailable tile in element \"%s\" in file \"%s\".\n",
    content, errfile);
  exit(EXIT_FAILURE);
}

void error_dora_limit_exceeded () {
  fprintf(stderr, "Dora limit exceeded in file \"%s\".\n", errfile);
  exit(EXIT_FAILURE);
}

void error_unknown_element (const dstr *elt) {
  fprintf(stderr, "Unknown element \"%s\" found in file \"%s\".\n", elt, errfile);
  exit(EXIT_FAILURE);
}
