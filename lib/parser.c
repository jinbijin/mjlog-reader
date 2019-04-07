#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dstr.h"
#include "ferrorloc.h"
#include "token.h"
#include "tlist.h"
#include "lexer.h"
#include "attr.h"
#include "elt.h"
#include "parser.h"

// Stack of elements

// A stack of elements; per usual, the empty stack is represented by the `NULL`
// pointer.
struct _stack_elt {
  mj_elt **elt;
  struct _stack_elt *next;
};

typedef struct _stack_elt stack_elt;

void stack_elt_push (stack_elt **this, mj_elt **elt) {
  stack_elt *top = malloc(sizeof(*top));
  top->elt = elt;
  top->next = *this;
  *this = top;
}

// I shouldn't need the return value, but at this point it's classical to
// include it.
mj_elt ** stack_elt_pop (stack_elt **this) {
  assert(this != NULL);
  stack_elt *tmp = *this;
  mj_elt **out = (*this)->elt;
  *this = (*this)->next;
  free(tmp);
  return out;
}

mj_elt ** stack_elt_top (stack_elt *this) {
  assert(this != NULL);
  return (this->elt);
}

// Parser mode

typedef enum {
  PAR_IDLE,
  PAR_TAG_INIT,
  PAR_READ_NAME, // Past perfect tense
  PAR_READ_ATTR, // Past perfect tense
  PAR_VAL
} parser_m;

typedef enum {
  TAG_NONE,
  TAG_OPEN,
  TAG_CLOSE
} tag_t;

// Only entry point into this function is `parser`, which guarantees that
// `stack != NULL` and `*stack != NULL` are true; the latter is enforced
// whenever a pop is attempted.
int parser_loop (mj_elt **elt, stack_elt **stack, parser_m mode, tag_t tag,
  const mj_tlist *tlist, ptrdiff_t head) {
  parser_m mode_new = mode;
  tag_t tag_new = tag;
  ptrdiff_t head_new = head + 1;
  mj_elt **top = stack_elt_top(*stack);
  dstr *top_name = elt_name_get(*top);

  if (head >= tlist_csize_get(tlist)) {
    if ((*stack)->next != NULL) {
      error_unexpected_eof(top_name);
    }
    return EXIT_SUCCESS;
  }

  mj_token **content = tlist_content(tlist);
  mj_token *current = content[head];
  mj_elt **top_new;
  dstr *top_new_name;

  switch (mode) {
    case PAR_IDLE:
      assert(tag == TAG_NONE);
      if (token_equal_const(current, TOK_SYM, "<")) {
        top_new = elt_append_new_elt(top);
        stack_elt_push(stack, top_new);

        mode_new = PAR_TAG_INIT;
        tag_new = TAG_OPEN;
      }
      else if (token_equal_const(current, TOK_SYM, "</")) {
        top_new = elt_append_new_elt(top);
        stack_elt_push(stack, top_new);

        mode_new = PAR_TAG_INIT;
        tag_new = TAG_CLOSE;
      }
      else {
        error_unexpected_token_tag_open(token_dynamic(current));
      }
      break;
    case PAR_TAG_INIT:
      assert(tag != TAG_NONE);
      if (token_type_get(current) == TOK_ID) {
        elt_name_set(*top, token_dynamic(current));

        mode_new = PAR_READ_NAME;
      }
      else {
        error_unexpected_token_name(token_dynamic(current));
      }
      break;
    case PAR_READ_NAME:
      assert(tag != TAG_NONE);
      if (token_type_get(current) == TOK_ID) {
        if (tag == TAG_CLOSE) {
          error_unexpected_attr(token_dynamic(current));
        }
        elt_append_new_attr(top);
        elt_current_attr_name_set(*top, token_dynamic(current));

        mode_new = PAR_READ_ATTR;
      }
      else if (token_equal_const(current, TOK_SYM, ">")) {
        top_new = stack_elt_top((*stack)->next);
        top_new_name = elt_name_get(*top_new);
        if (tag == TAG_CLOSE) {
          if ((*stack)->next->next == NULL || !(dstr_equal(top_name, top_new_name))) {
            error_unmatched_close_tag(top_new_name, top_name);
          }
          stack_elt_pop(stack);
          elt_current_elt_delete(*top_new);
          stack_elt_pop(stack);
        }

        mode_new = PAR_IDLE;
        tag_new = TAG_NONE;
      }
      else if (token_equal_const(current, TOK_SYM, "/>")) {
        if (tag == TAG_CLOSE) {
          error_invalid_tag();
        }
        stack_elt_pop(stack);

        mode_new = PAR_IDLE;
        tag_new = TAG_NONE;
      }
      else {
        error_unexpected_token_attr(token_dynamic(current));
      }
      break;
    case PAR_READ_ATTR:
      assert(tag != TAG_NONE);
      if (token_equal_const(current, TOK_SYM, "=")) {
        mode_new = PAR_VAL;
      }
      else {
        error_unexpected_token_equal(token_dynamic(current));
      }
      break;
    case PAR_VAL:
      assert(tag == TAG_OPEN);
      if (token_type_get(current) == TOK_VAL) {
        elt_current_attr_append_dstr(*top, token_dynamic(current));
      }
      else if (token_type_get(current) == TOK_ID) {
        elt_append_new_attr(top);
        elt_current_attr_name_set(*top, token_dynamic(current));

        mode_new = PAR_READ_ATTR;
      }
      else if (token_equal_const(current, TOK_SYM, ">")) {
        // `(*stack)->next == NULL` is an internal error.
        top_new = stack_elt_top((*stack)->next);
        top_new_name = elt_name_get(*top_new);
        if (tag == TAG_CLOSE) {
          if ((*stack)->next->next == NULL || !(dstr_equal(top_name, top_new_name))) {
            error_unmatched_close_tag(top_new_name, top_name);
          }
          stack_elt_pop(stack);
          elt_current_elt_delete(*top_new);
          stack_elt_pop(stack);
        }

        mode_new = PAR_IDLE;
        tag_new = TAG_NONE;
      }
      else if (token_equal_const(current, TOK_SYM, "/>")) {
        // `(*stack)->next == NULL` is an internal error.
        if (tag == TAG_CLOSE) {
          error_invalid_tag();
        }
        stack_elt_pop(stack);

        mode_new = PAR_IDLE;
        tag_new = TAG_NONE;
      }
      else {
        error_unexpected_token_val(token_dynamic(current));
      }
      break;
    default:
      assert(false);
  }

  parser_loop(elt, stack, mode_new, tag_new, tlist, head_new);
}

void parser (mj_elt **elt, const char *filename) {
  mj_tlist *tlist;
  stack_elt *stack = NULL;
  lexer(&tlist, filename);
  elt_init(elt);
  elt_name_set_const(*elt, "root");
  stack_elt_push(&stack, elt);

  parser_loop(elt, &stack, PAR_IDLE, TAG_NONE, tlist, 0);

  tlist_delete(tlist);
}
