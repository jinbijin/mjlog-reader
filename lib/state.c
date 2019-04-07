#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dstr.h"
#include "state.h"

void discard_init (mj_discard *this) {
  assert(this != NULL);
  this->tile_id = MJ_NULL;
  this->tsumogiri = false;
  this->riichi = false;
  this->naki = false;
}

void tileset_init (mj_tileset *this) {
  assert(this != NULL);
  ptrdiff_t i;
  for (i = 0; i < MJ_TILES_NO; i++) {
    this->tiles[i] = false;
  }
  this->last = MJ_NULL;
  this->from_who = MJ_NULL;
  this->is_big = false;
}

void tileset_init_full (mj_tileset *this) {
  assert(this != NULL);
  ptrdiff_t i;
  for (i = 0; i < MJ_TILES_NO; i++) {
    this->tiles[i] = true;
  }
  this->last = MJ_NULL;
  this->from_who = MJ_NULL;
  this->is_big = false;
}

void tileset_print (const mj_tileset *this) {
  uint8_t i;
  for (i = 0; i < MJ_TILES_NO; i++) {
    if (this->tiles[i]) {
      printf("%d ", i);
    }
  }
}

void tileset_print_line (const mj_tileset *this) {
  tileset_print(this);
  printf("\n");
}

void state_init (mj_state *this) {
  assert(this != NULL);
  ptrdiff_t i, j;

  for (i = 0; i < MJ_PLAYER_NO; i++) {
    this->presence[i] = true;
  }
}

void state_reset_round (mj_state *this) {
  assert(this != NULL);
  ptrdiff_t i, j;
  this->remaining = 70;
  for (i = 0; i < MJ_PLAYER_NO; i++) {
    for (j = 0; j < MJ_DISCARD_LIMIT; j++) {
      discard_init(&(this->discard[i][j]));
    }
    this->riichi[i] = 0;
    tileset_init(&(this->hand[i]));
    for (j = 0; j < MJ_CALL_LIMIT; j++) {
      tileset_init(&(this->call[i][j]));
      this->pei[i][j] = MJ_NULL;
    }
  }
  for (i = 0; i < MJ_DORA_LIMIT; i++) {
    this->dora_indicator[i] = MJ_NULL;
  }
  tileset_init_full(&(this->wall));
}
