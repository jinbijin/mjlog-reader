/* Tenhou mjlog parser, by Jinbi Jin */

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "parser/token.h"
#include "parser/tlist.h"
#include "parser/lexer.h"

#include "parser/hand.h"
#include "parser/event.h"
#include "parser/term.h"
#include "parser/round.h"
#include "parser/match.h"

#include "parser.h"

char mj_errfile[MJ_PATH_LIMIT];

typedef enum {
  AGARI_RON_OYA, 
  AGARI_RON_KO, 
  AGARI_TSUMO_OYA, 
  AGARI_TSUMO_KO
} mj_agari_t;

int mj_ten_to_value(mj_agari_t type, long int ten) {
  int table_ron_oya[24] = {
    1500, 2000, 2400, 2900, 3400, 
    3900, 4400, 4800, 5300, 5800, 
    6800, 7700, 8700, 9600, 10600, 11600, 
    12000, 18000, 24000, 36000, 48000, 
    96000, 144000, 192000
  };
  int table_ron_ko[24] = {
    1000, 1300, 1600, 2000, 2300, 
    2600, 2900, 3200, 3600, 3900, 
    4500, 5200, 5800, 6400, 7100, 7700,
    8000, 12000, 16000, 24000, 32000, 
    64000, 96000, 128000
  };
  int table_tsumo_oya[24] = {
    1500, 2100, 2400, 3000, 3600, 
    3900, 4500, 4800, 5400, 6000, 
    6900, 7800, 8700, 9600, 10800, 11700,
    12000, 18000, 24000, 36000, 48000, 
    96000, 144000, 192000
  };
  int table_tsumo_ko[24] = {
    1100, 1500, 1600, 2000, 2400, 
    2700, 3100, 3200, 3600, 4000, 
    4700, 5200, 5900, 6400, 7200, 7900,
    8000, 12000, 16000, 24000, 32000, 
    64000, 96000, 128000
  };
  int *table;
  ptrdiff_t i = 0;
  bool found_value = false;
  if (type == AGARI_RON_OYA) {
    table = table_ron_oya;
  }
  else if (type == AGARI_RON_KO) {
    table = table_ron_ko;
  }
  else if (type == AGARI_TSUMO_OYA) {
    table = table_tsumo_oya;
  }
  else if (type == AGARI_TSUMO_KO) {
    table = table_tsumo_ko;
  }
  while (!found_value && i < 24) {
    if (table[i] == ten) {
      found_value = true;
    }
    if (!found_value) {
      i++;
    }
  }
  if (i == 24) {
    mj_error_value_conversion(); // Exits.
  }
  return i;
}

void parser_event(mj_event *this, mj_tlist *tlist) {
  // The address of the following variable is the `char **` argument of
  // various string-to-numerical functions.
  char *not_used;
  // Head is on an event tag.
  if (mj_token_is_discard(tlist->content[tlist->head])) {
    this->type = EVENT_DISCARD;
    this->who = tlist->content[tlist->head]->content[0] - 'D';
    this->info =
      strtol(tlist->content[tlist->head]->content + 1, &not_used, 10);
  }
  else if (mj_token_is_draw(tlist->content[tlist->head])) {
    this->type = EVENT_DRAW;
    this->who = tlist->content[tlist->head]->content[0] - 'T';
    this->info =
      strtol(tlist->content[tlist->head]->content + 1, &not_used, 10);
  }
  else if (mj_token_is(tlist->content[tlist->head], TOK_TAG, "N")) {
    this->type = EVENT_CALL;
    mj_tlist_move(tlist, 2); // value of `who`
    this->who = 
      strtol(tlist->content[tlist->head]->content, &not_used, 10);
    mj_tlist_move(tlist, 2); // value of `m`
    this->info =
      strtol(tlist->content[tlist->head]->content, &not_used, 10);
  }
  else if (mj_token_is(tlist->content[tlist->head], TOK_TAG, "DORA")) {
    this->type = EVENT_DORA;
    this->who = 0;
    mj_tlist_move(tlist, 2); // value of `hai`
    this->info =
      strtol(tlist->content[tlist->head]->content, &not_used, 10);
  }
  else if (mj_token_is(tlist->content[tlist->head], TOK_TAG, "REACH")) {
    this->type = EVENT_RIICHI;
    mj_tlist_move(tlist, 2); // value of `who`
    this->who =
      strtol(tlist->content[tlist->head]->content, &not_used, 10);
    // Can't use `mj_tlist_scan_until` here, it would scan too far.
    mj_tlist_next(tlist); // next field; `step` or `ten`
    if (mj_token_is(tlist->content[tlist->head], TOK_FIELD, "ten")) {
      mj_tlist_move(tlist, 5); // `step`
    }
    mj_tlist_next(tlist); // value of `step`
    this->info =
      strtol(tlist->content[tlist->head]->content, &not_used, 10);
  }
  else { // Unreachable (if called correctly).
  }
  // Make sure head moves at least once.
  // This will move the head *onto* the next event tag at most.
  mj_tlist_next(tlist);
}

void parser_term(mj_term *this, mj_tlist *tlist, uint8_t dealer) {
  // The address of the following variable is the `char **` argument of
  // various string-to-numerical functions.
  char *not_used;
  ptrdiff_t i;
  long int ten = 0;
  // Head is on a terminal tag.
  if (mj_token_is(tlist->content[tlist->head], TOK_TAG, "AGARI")) {
    this->type = TERM_AGARI;
    mj_tlist_next(tlist);

    if (!mj_tlist_scan_until(tlist, &mj_token_is_ten, &mj_token_is_term_ender)) {
      mj_error_unexpected_eof_while("ten"); // Exits.
    }
    mj_tlist_move(tlist, 2); // second value of `ten`
    ten = 
      strtol(tlist->content[tlist->head]->content, &not_used, 10);

    if (!mj_tlist_scan_until(tlist, &mj_token_is_who, &mj_token_is_term_ender)) {
      mj_error_unexpected_eof_while("who"); // Exits.
    }
    mj_tlist_next(tlist); // value of `who`
    this->who =
      strtol(tlist->content[tlist->head]->content, &not_used, 10);
    mj_tlist_move(tlist, 2); // value of `fromWho`
    this->from =
      strtol(tlist->content[tlist->head]->content, &not_used, 10);
    mj_tlist_next(tlist); // next field; `paoWho` or `sc`
    
    if (mj_token_is(tlist->content[tlist->head], TOK_FIELD, "paoWho")) {
      mj_tlist_next(tlist); // value of `paoWho`
      this->pao =
	strtol(tlist->content[tlist->head]->content, &not_used, 10);
      mj_tlist_next(tlist); // `sc`
    }
    else {
      this->pao = this->from;
    }

    if (this->who == this->from) {
      if (this->who == dealer) {
	this->value = mj_ten_to_value(AGARI_TSUMO_OYA, ten);
      }
      else {
	this->value = mj_ten_to_value(AGARI_TSUMO_KO, ten);
      }
    }
    else {
      if (this->who == dealer) {
	this->value = mj_ten_to_value(AGARI_RON_OYA, ten);
      }
      else {
	this->value = mj_ten_to_value(AGARI_RON_KO, ten);
      }
    }
    
    for (i = 0; i < 4; i++) {
      mj_tlist_move(tlist, 2); // next value of `sc`
      this->score_diff[i] =
	strtol(tlist->content[tlist->head]->content, &not_used, 10);
    }
  }
  else if (mj_token_is(tlist->content[tlist->head], TOK_TAG, "RYUUKYOKU")) {
    this->type = TERM_RYUUKYOKU;
    this->who = 0;
    this->from = 0;
    this->pao = 0;
    this->value = 0;
    mj_tlist_next(tlist); // Moves away from tag
    if (mj_tlist_scan_until(tlist, &mj_token_is_sc, &mj_token_is_term_ender)) {
      for (i = 0; i < 4; i++) {
	mj_tlist_move(tlist, 2); // next value of `sc`
	this->score_diff[i] =
	  strtol(tlist->content[tlist->head]->content, &not_used, 10);
      }
    }
    else {
      for (i = 0; i < 4; i++) {
	this->score_diff[i] = 0; // no `sc` attribute, so no score change?
      }
    }
  }
  else { // Unreachable (if called correctly).
  }
}

// `filename` is assumed to fit inside buffer of length
// `MJ_PATH_LIMIT`.
void parser(mj_match *this, const char *filename) {
  // The address of the following variable is the `char **` argument of
  // various string-to-numerical functions.
  char *not_used;
  ptrdiff_t i, j;

  strncpy(mj_errfile, filename, MJ_PATH_LIMIT);

  if (this == NULL) {
    mj_error_null_pointer(); // Exits.
  }
  mj_tlist *tlist = malloc(sizeof(*tlist));
  if (tlist == NULL) {
    mj_error_alloc(); // Exits.
  }
  lexer(tlist, filename);

  // Process token list.
  mj_match_init(this);

  if (!mj_tlist_scan_until(tlist, &mj_token_is_go, &mj_token_is_null)) {
    mj_error_unexpected_eof_while("GO");
  }
  mj_tlist_move(tlist, 2); // value of `type`
  this->match = 
    strtol(tlist->content[tlist->head]->content, &not_used, 10);
  mj_tlist_move(tlist, 2); // value of `lobby`
  this->lobby = 
    strtol(tlist->content[tlist->head]->content, &not_used, 10);
  mj_tlist_move(tlist, 10); // `dan` (in `UN`)
  for (i = 0; i < 4; i++) {
    mj_tlist_next(tlist); // next value
    this->rank[i] =
      strtol(tlist->content[tlist->head]->content, &not_used, 10);
  }
  mj_tlist_next(tlist); // `rate`
  for (i = 0; i < 4; i++) {
    mj_tlist_next(tlist); // next value
    this->rating[i] =
      lround(100 * strtod(tlist->content[tlist->head]->content, &not_used));
  }

  // Iterate over rounds, which start with `INIT`.
  while (mj_tlist_scan_until(tlist, &mj_token_is_init, &mj_token_is_owari)) {
    // Head of `tlist` is at an `INIT`.
    mj_match_append_round(this);
    mj_tlist_move(tlist, 2); // first value of `seed`
    this->round[this->size - 1]->round =
      strtol(tlist->content[tlist->head]->content, &not_used, 10);
    mj_tlist_next(tlist); // second value of `seed`
    this->round[this->size - 1]->repeat =
      strtol(tlist->content[tlist->head]->content, &not_used, 10);
    mj_tlist_next(tlist); // third value of `seed`
    this->round[this->size - 1]->deposit =
      strtol(tlist->content[tlist->head]->content, &not_used, 10);
    mj_tlist_move(tlist, 3); // sixth value of `seed`
    this->round[this->size - 1]->indicator =
      strtol(tlist->content[tlist->head]->content, &not_used, 10);
    mj_tlist_next(tlist); // `ten`
    for (i = 0; i < 4; i++) {
      mj_tlist_next(tlist); // next value
      this->round[this->size - 1]->score[i] =
	strtol(tlist->content[tlist->head]->content, &not_used, 10);
    }
    mj_tlist_move(tlist, 2); // value of `oya`
    this->round[this->size - 1]->dealer =
      strtol(tlist->content[tlist->head]->content, &not_used, 10);
    for (i = 0; i < 4; i++) {
      mj_tlist_next(tlist); // next `hai[0-3]`
      for (j = 0; j < 13; j++) {
	mj_tlist_next(tlist); // next value
	mj_hand_add_tile_by_id(
	    &(this->round[this->size - 1]->hand[i]),
	    strtol(tlist->content[tlist->head]->content, &not_used, 10)
	    );
      }
    }

    // Iterate over events.
    while (mj_tlist_scan_until(tlist, &mj_token_is_event, &mj_token_is_term)) {
      mj_match_append_event(this);
      parser_event(
	  this->round[this->size - 1]->event[
	  this->round[this->size - 1]->event_size - 1], tlist);
    }

    // Iterate over terminators.
    while (mj_tlist_scan_until(tlist, 
	  &mj_token_is_term, &mj_token_is_init_or_owari)) {
      mj_match_append_term(this);
      parser_term(
	  this->round[this->size - 1]->term[
	  this->round[this->size - 1]->term_size - 1], tlist,
	  this->round[this->size - 1]->dealer);
    }
  }

  // Head of `tlist` is at `owari`.
  for (i = 0; i < 4; i++) {
    mj_tlist_move(tlist, 2); // next value
    this->score[i] =
      lround(10 * strtod(tlist->content[tlist->head]->content, &not_used));
  }

  mj_tlist_final(tlist);
  free(tlist);
}
