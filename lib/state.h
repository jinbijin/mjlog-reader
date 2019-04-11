#ifndef _MJ_STATE_H_
#define _MJ_STATE_H_

#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dstr.h"

#define MJ_PLAYER_NO 4
#define MJ_SUIT_NO 4
#define MJ_TILES_NO 136
#define MJ_CALL_LIMIT 4
#define MJ_DORA_LIMIT 5
#define MJ_DISCARD_LIMIT 24

#define MJ_NULL 255

// A player id.
// * `0-3` valid player id
// * `255` NULL value
typedef uint8_t mj_player_id;

// A tile id.
// * `0-135` valid tile id
// * `255` NULL value
typedef uint8_t mj_tile_id;

// Discard with flags
typedef struct {
  mj_tile_id tile_id;
  bool tsumogiri;
  bool riichi;
  bool naki;
} mj_discard;

// A set of tiles, in either hands or called sets
typedef struct {
  bool tiles[MJ_TILES_NO];
  mj_tile_id last;
  mj_player_id from_who;
  bool is_big; // True if daiminkan or ankan, only set for called tilesets.
} mj_tileset;

// Anonymous board state
typedef struct {
  // Match constants
  uint16_t match_type;
  uint32_t lobby_no;
  uint8_t rank_id[MJ_PLAYER_NO];
  int32_t rating[MJ_PLAYER_NO]; // Times 100 to make it an integer

  // Round constants
  uint8_t round;
  uint8_t repeats;
  mj_player_id dealer;

  // Round variables
  uint8_t deposits;
  uint8_t remaining; // Remaining draws (auxiliary)
  int16_t score[MJ_PLAYER_NO]; // Times 10 to make it an integer
  bool presence[MJ_PLAYER_NO];
  mj_tile_id dora_indicator[MJ_DORA_LIMIT];
  mj_discard discard[MJ_PLAYER_NO][MJ_DISCARD_LIMIT];
  // 0 = no riichi, 1 = riichi, no discard yet, 2 = successful riichi
  uint8_t riichi[MJ_PLAYER_NO];
  mj_tileset hand[MJ_PLAYER_NO];
  mj_tileset call[MJ_PLAYER_NO][MJ_CALL_LIMIT]; // Calls not made are empty
  mj_tile_id pei[MJ_PLAYER_NO][MJ_CALL_LIMIT]; // For compatibility with sanma
  mj_tileset wall; // (auxiliary)
} mj_state;

void discard_init (mj_discard *this);

void discard_print (const mj_discard *);

void discard_print_line (const mj_discard *);

void tileset_init (mj_tileset *this);

void tileset_init_full (mj_tileset *this);

void tileset_print (const mj_tileset *this);

void tileset_print_line (const mj_tileset *this);

bool tileset_is_null (const mj_tileset *this);

bool tileset_is_koutsu (const mj_tileset *this, uint8_t base);

void state_init (mj_state *this);

void state_reset_round (mj_state *this);

#endif
