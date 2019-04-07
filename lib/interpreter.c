#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

#include "dstr.h"
#include "ferrorloc.h"
#include "attr.h"
#include "elt.h"
#include "state.h"
#include "parser.h"
#include "record.h"
#include "interpreter.h"

// TODO: Draw, discard, call
// TODO (constraints):  UN_1, BYE

// Procedure for checking whether an attribute has already been found yet,
// and whether it has enough values.
void check_attribute (const mj_attr *attr, const dstr *elt_name, bool *b, size_t n) {
  assert(elt_name != NULL);
  assert(b != NULL);
  dstr *attr_name = attr_name_get(attr);
  if (*b) {
    error_duplicate_attr(attr_name_get(attr), elt_name);
  }
  *b = true;
  if (attr_csize_get(attr) < n) {
    error_missing_value(attr_name_get(attr), elt_name);
  }
}

// Procedure for adding a dora indicator.
void add_dora_indicator (mj_state *state, mj_tile_id t) {
  ptrdiff_t i = 0;
  while (state->dora_indicator[i] != MJ_NULL && i < MJ_DORA_LIMIT) {
    i++;
  }
  if (i == MJ_DORA_LIMIT) {
    error_dora_limit_exceeded();
  }
  state->dora_indicator[i] = t;
  state->wall.tiles[t] = false;
}

// Read attributes from GO
void interpreter_loop_GO (mj_state *state, const mj_elt *elt) {
  bool b[2] = {false, false};
  char *a[2] = {"type", "lobby"};
  size_t n[2] = {1, 1};

  ptrdiff_t i, j;
  mj_attr **attr = elt_content_attr(elt);
  dstr *name = elt_name_get(elt);

  dstr **val;
  char *end_ptr;

  for (i = 0; i < elt_csize_get(elt); i++) {
    val = attr_content(attr[i]);

    for (j = 0; j < 2; j++) {
      if (dstr_equal_const(attr_name_get(attr[i]), a[j])) {
        check_attribute(attr[i], name, &(b[j]), n[j]);
        switch (j) {
          case 0: // "type"
            state->match_type = strtol(dstr_content(val[0]),&end_ptr,10);
            break;
          case 1: // "lobby"
            state->lobby_no = strtol(dstr_content(val[0]),&end_ptr,10);
            break;
          default:
            assert(false);
        }
      }
    }
  }

  for (j = 0; j < 2; j++) {
    if (!b[j]) {
      error_missing_attr(a[j], name);
    }
  }
}

// Read attributes from UN, first pass
void interpreter_loop_UN_0 (mj_state *state, const mj_elt *elt) {
  bool b[2] = {false, false};
  char *a[2] = {"dan", "rate"};
  size_t n[2] = {4, 4};

  ptrdiff_t i, j, k;
  mj_attr **attr = elt_content_attr(elt);
  dstr *name = elt_name_get(elt);

  dstr **val;
  char *end_ptr;

  for (i = 0; i < elt_csize_get(elt); i++) {
    val = attr_content(attr[i]);

    for (j = 0; j < 2; j++) {
      if (dstr_equal_const(attr_name_get(attr[i]), a[j])) {
        check_attribute(attr[i], name, &(b[j]), n[j]);
        switch (j) {
          case 0: // "dan"
            for (k = 0; k < MJ_PLAYER_NO; k++) {
              state->rank_id[k] = strtol(dstr_content(val[k]),&end_ptr,10);
            }
            break;
          case 1: // "rate"
            for (k = 0; k < MJ_PLAYER_NO; k++) {
              state->rating[k] = lround(100*strtod(dstr_content(val[k]),&end_ptr));
            }
            break;
          default:
            assert(false);
        }
      }
    }
  }

  for (j = 0; j < 2; j++) {
    if (!b[j]) {
      error_missing_attr(a[j], name);
    }
  }
}

// Read attributes from a reconnection UN
void interpreter_loop_UN_1 (mj_state *state, const mj_elt *elt) {
  mj_attr **attr = elt_content_attr(elt);
  dstr *name = elt_name_get(elt);

  if (elt_csize_get(elt) == 0) {
    error_missing_attr("n", name);
  }
  if (dstr_equal_const(attr_name_get(attr[0]), "n0")) {
    state->presence[0] = true;
  }
  else if (dstr_equal_const(attr_name_get(attr[0]), "n1")) {
    state->presence[1] = true;
  }
  else if (dstr_equal_const(attr_name_get(attr[0]), "n2")) {
    state->presence[2] = true;
  }
  else if (dstr_equal_const(attr_name_get(attr[0]), "n3")) {
    state->presence[3] = true;
  }
}

// Read attributes from INIT
void interpreter_loop_INIT (mj_state *state, const mj_elt *elt) {
  bool b[7] = {false, false, false, false, false, false, false};
  char *a[7] = {"seed", "ten", "oya", "hai0", "hai1", "hai2", "hai3"};
  size_t n[7] = {6, 4, 1, 13, 13, 13, 13};

  ptrdiff_t i, j, k;
  mj_attr **attr = elt_content_attr(elt);
  dstr *name = elt_name_get(elt);

  dstr **val;
  char *end_ptr;
  mj_tile_id t;

  for (i = 0; i < elt_csize_get(elt); i++) {
    val = attr_content(attr[i]);

    for (j = 0; j < 7; j++) {
      if (dstr_equal_const(attr_name_get(attr[i]), a[j])) {
        check_attribute(attr[i], name, &(b[j]), n[j]);
        switch (j) {
          case 0: // "seed"
            state->round = strtol(dstr_content(val[0]),&end_ptr,10);
            state->repeats = strtol(dstr_content(val[1]),&end_ptr,10);
            state->deposits = strtol(dstr_content(val[2]),&end_ptr,10);
            state->dora_indicator[0] = strtol(dstr_content(val[5]),&end_ptr,10);
            break;
          case 1: // "ten"
            for (k = 0; k < MJ_PLAYER_NO; k++) {
              state->score[k] = strtol(dstr_content(val[k]),&end_ptr,10);
            }
            break;
          case 2: // "oya"
            state->dealer = strtol(dstr_content(val[0]),&end_ptr,10);
            break;
          case 3: // "hai0"
            for (k = 0; k < 13; k++) {
              t = strtol(dstr_content(val[k]),&end_ptr,10);
              if (t >= MJ_TILES_NO) {
                error_invalid_tile_id(elt_name_get(elt));
              }
              if (state->wall.tiles[t]) {
                state->hand[0].tiles[t] = true;
                state->wall.tiles[t] = false;
              }
              else {
                error_unavailable_tile(elt_name_get(elt));
              }
            }
            break;
          case 4: // "hai1"
            for (k = 0; k < 13; k++) {
              t = strtol(dstr_content(val[k]),&end_ptr,10);
              if (t >= MJ_TILES_NO) {
                error_invalid_tile_id(elt_name_get(elt));
              }
              if (state->wall.tiles[t]) {
                state->hand[1].tiles[t] = true;
                state->wall.tiles[t] = false;
              }
              else {
                error_unavailable_tile(elt_name_get(elt));
              }
            }
            break;
          case 5: // "hai2"
            for (k = 0; k < 13; k++) {
              t = strtol(dstr_content(val[k]),&end_ptr,10);
              if (t >= MJ_TILES_NO) {
                error_invalid_tile_id(elt_name_get(elt));
              }
              if (state->wall.tiles[t]) {
                state->hand[2].tiles[t] = true;
                state->wall.tiles[t] = false;
              }
              else {
                error_unavailable_tile(elt_name_get(elt));
              }
            }
            break;
          case 6: // "hai3"
            for (k = 0; k < 13; k++) {
              t = strtol(dstr_content(val[k]),&end_ptr,10);
              if (t >= MJ_TILES_NO) {
                error_invalid_tile_id(elt_name_get(elt));
              }
              if (state->wall.tiles[t]) {
                state->hand[3].tiles[t] = true;
                state->wall.tiles[t] = false;
              }
              else {
                error_unavailable_tile(elt_name_get(elt));
              }
            }
            break;
          default:
            assert(false);
        }
      }
    }
  }

  for (j = 0; j < 7; j++) {
    if (!b[j]) {
      error_missing_attr(a[j], name);
    }
  }
}

// Read attributes from REACH
void interpreter_loop_REACH (mj_state *state, const mj_elt *elt) {
  bool b[2] = {false, false};
  char *a[2] = {"who", "step"};
  size_t n[2] = {1, 1};

  mj_player_id who;
  uint8_t step;
  ptrdiff_t i, j;
  mj_attr **attr = elt_content_attr(elt);
  dstr *name = elt_name_get(elt);

  dstr **val;
  char *end_ptr;

  for (i = 0; i < elt_csize_get(elt); i++) {
    val = attr_content(attr[i]);

    for (j = 0; j < 2; j++) {
      if (dstr_equal_const(attr_name_get(attr[i]), a[j])) {
        check_attribute(attr[i], name, &(b[j]), n[j]);
        switch (j) {
          case 0: // "who"
            who = strtol(dstr_content(val[0]), &end_ptr, 10);
            break;
          case 1: // "step"
            step = strtol(dstr_content(val[0]), &end_ptr, 10);
            break;
          default:
            assert(false);
        }
      }
    }
  }

  for (j = 0; j < 2; j++) {
    if (!b[j]) {
      error_missing_attr(a[j], name);
    }
    if (who >= MJ_PLAYER_NO) {
      error_invalid_player_id(name);
    }
    if (step != 1 && step != 2) {
      error_invalid_step(name);
    }
    state->riichi[who] = step;
  }
}

void interpreter_loop_DORA (mj_state *state, const mj_elt *elt) {
  bool b[1] = {false};
  char *a[1] = {"hai"};
  size_t n[1] = {1};

  ptrdiff_t i, j;
  mj_attr **attr = elt_content_attr(elt);
  dstr *name = elt_name_get(elt);

  dstr **val;
  char *end_ptr;
  mj_tile_id t;

  for (i = 0; i < elt_csize_get(elt); i++) {
    val = attr_content(attr[i]);

    for (j = 0; j < 1; j++) {
      if (dstr_equal_const(attr_name_get(attr[i]), a[j])) {
        check_attribute(attr[i], name, &(b[j]), n[j]);
        switch (j) {
          case 0: // "hai"
            t = strtol(dstr_content(val[0]),&end_ptr,10);
            if (t >= MJ_TILES_NO) {
              error_invalid_tile_id(elt_name_get(elt));
            }
            if (!(state->wall.tiles[t])) {
              error_unavailable_tile(elt_name_get(elt));
            }
            add_dora_indicator(state, t);
            break;
          default:
            assert(false);
        }
      }
    }
  }

  for (j = 0; j < 1; j++) {
    if (!b[j]) {
      error_missing_attr(a[j], name);
    }
  }
}

// Reads atttributes of BYE
void interpreter_loop_BYE (mj_state *state, const mj_elt *elt) {
  bool b[1] = {false};
  char *a[1] = {"who"};
  size_t n[1] = {1};

  ptrdiff_t i, j;
  mj_attr **attr = elt_content_attr(elt);
  dstr *name = elt_name_get(elt);

  dstr **val;
  char *end_ptr;
  mj_player_id p;

  for (i = 0; i < elt_csize_get(elt); i++) {
    val = attr_content(attr[i]);

    for (j = 0; j < 1; j++) {
      if (dstr_equal_const(attr_name_get(attr[i]), a[j])) {
        check_attribute(attr[i], name, &(b[j]), n[j]);
        switch (j) {
          case 0: // "who"
            p = strtol(dstr_content(val[0]),&end_ptr,10);
            if (p >= MJ_PLAYER_NO) {
              error_invalid_player_id(elt_name_get(elt));
            }
            state->presence[p] = false;
            break;
          default:
            assert(false);
        }
      }
    }
  }

  for (j = 0; j < 1; j++) {
    if (!b[j]) {
      error_missing_attr(a[j], name);
    }
  }
}

// Reads atttributes of AGARI
void interpreter_loop_AGARI (mj_state *state, const mj_elt *elt) {
  bool b[1] = {false};
  char *a[1] = {"sc"};
  size_t n[1] = {8};

  ptrdiff_t i, j, k;
  mj_attr **attr = elt_content_attr(elt);
  dstr *name = elt_name_get(elt);

  dstr **val;
  char *end_ptr;
  int16_t score;

  for (i = 0; i < elt_csize_get(elt); i++) {
    val = attr_content(attr[i]);

    for (j = 0; j < 1; j++) {
      if (dstr_equal_const(attr_name_get(attr[i]), a[j])) {
        check_attribute(attr[i], name, &(b[j]), n[j]);
        switch (j) {
          case 0: // "sc"
            for (k = 0; k < MJ_PLAYER_NO; k++) {
              score = strtol(dstr_content(val[2*k]),&end_ptr,10);
              score += strtol(dstr_content(val[2*k+1]),&end_ptr,10);
              state->score[k] = score;
            }
            break;
          default:
            assert(false);
        }
      }
    }

    // Optional argument "owari"
    bool b_owari = false;
    if (dstr_equal_const(attr_name_get(attr[i]), "owari")) {
      check_attribute(attr[i], name, &b_owari, 8);
      for (k = 0; k < MJ_PLAYER_NO; k++) {
        score = strtol(dstr_content(val[2*k]),&end_ptr,10);
        state->score[k] = score;
      }
    }
  }

  for (j = 0; j < 1; j++) {
    if (!b[j]) {
      error_missing_attr(a[j], name);
    }
  }
}

// Reads atttributes of RYUUKYOKU
void interpreter_loop_RYUUKYOKU (mj_state *state, const mj_elt *elt) {
  bool b[1] = {false};
  char *a[1] = {"sc"};
  size_t n[1] = {8};

  ptrdiff_t i, j, k;
  mj_attr **attr = elt_content_attr(elt);
  dstr *name = elt_name_get(elt);

  dstr **val;
  char *end_ptr;
  int16_t score;

  for (i = 0; i < elt_csize_get(elt); i++) {
    val = attr_content(attr[i]);

    for (j = 0; j < 1; j++) {
      if (dstr_equal_const(attr_name_get(attr[i]), a[j])) {
        check_attribute(attr[i], name, &(b[j]), n[j]);
        switch (j) {
          case 0: // "sc"
            for (k = 0; k < MJ_PLAYER_NO; k++) {
              score = strtol(dstr_content(val[2*k]),&end_ptr,10);
              score += strtol(dstr_content(val[2*k+1]),&end_ptr,10);
              state->score[k] = score;
            }
            break;
          default:
            assert(false);
        }
      }
    }

    // Optional argument "owari"
    bool b_owari = false;
    if (dstr_equal_const(attr_name_get(attr[i]), "owari")) {
      check_attribute(attr[i], name, &b_owari, 8);
      for (k = 0; k < MJ_PLAYER_NO; k++) {
        score = strtol(dstr_content(val[2*k]),&end_ptr,10);
        state->score[k] = score;
      }
    }
  }

  for (j = 0; j < 1; j++) {
    if (!b[j]) {
      error_missing_attr(a[j], name);
    }
  }
}

// Main loop of interpreter
int interpreter_loop (mj_state *state, mj_record **record,
    const mj_elt *elt, ptrdiff_t head) {
  mj_elt *current = elt_content_elt(elt)[head];

  // To protect against zero size content.
  if (head >= elt_tsize_get(elt)) {
    return EXIT_SUCCESS;
  }

  // Strict preamble checking; `INIT` is not processed yet.
  // This allows us to initially initialise fewer fields of `state`.
  if (head == 0) {
    if (!(dstr_equal_const(elt_name_get(current), "SHUFFLE"))) {
      error_expected_tag("SHUFFLE", elt_name_get(current));
    }
  }
  else if (head == 1) {
    if (!(dstr_equal_const(elt_name_get(current), "GO"))) {
      error_expected_tag("GO", elt_name_get(current));
    }
    interpreter_loop_GO(state, current);
  }
  else if (head == 2) {
    if (!(dstr_equal_const(elt_name_get(current), "UN"))) {
      error_expected_tag("UN", elt_name_get(current));
    }
    interpreter_loop_UN_0(state, current);
  }
  else if (head == 3) {
    if (!(dstr_equal_const(elt_name_get(current), "TAIKYOKU"))) {
      error_expected_tag("TAIKYOKU", elt_name_get(current));
    }
  }

  // Actual game
  if (head >= 3) {
    if (dstr_equal_const(elt_name_get(current), "INIT")) {
      state_reset_round(state);
      interpreter_loop_INIT(state, current);
    }
    else if (dstr_equal_const(elt_name_get(current), "REACH")) {
      interpreter_loop_REACH(state, current);
    }
    else if (dstr_equal_const(elt_name_get(current), "DORA")) {
      interpreter_loop_DORA(state, current);
    }
    else if (dstr_equal_const(elt_name_get(current), "UN")) {
      interpreter_loop_UN_1(state, current);
    }
    else if (dstr_equal_const(elt_name_get(current), "BYE")) {
      interpreter_loop_BYE(state, current);
    }
    else if (dstr_equal_const(elt_name_get(current), "AGARI")) {
      interpreter_loop_AGARI(state, current);
    }
    else if (dstr_equal_const(elt_name_get(current), "RYUUKYOKU")) {
      interpreter_loop_RYUUKYOKU(state, current);
    }
    // To be continued...
  }

  // Update record if applicable
  if (head >= 4) {
    record_update(record, state, elt, head);
  }

  // Checking whether `owari` attribute has been found
  if (head >= elt_tsize_get(elt) - 1) {
    if (head <= 4) {
      error_unexpected_end_of_game();
    }
    // Check for "owari" attribute here
    return EXIT_SUCCESS;
  }
  interpreter_loop (state, record, elt, head + 1);
}

void interpreter (mj_record **record, const char *filename) {
  mj_elt *elt;
  parser(&elt, filename);
  mj_state state;
  state_init(&state);

  // Basic structural checks
  assert(dstr_equal_const(elt_name_get(elt),"root"));
  if (elt_tsize_get(elt) != 1) {
    error_root_tag();
  }
  mj_elt *root = elt_content_elt(elt)[0];
  if (!(dstr_equal_const(elt_name_get(root),"mjloggm"))) {
    error_expected_tag("mjloggm", elt_name_get(root));
  }

  interpreter_loop(&state, record, root, 0);

  elt_delete(elt);
}
