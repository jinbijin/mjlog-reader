/* `hand.h`, by Jinbi Jin */

#ifndef __HAND_H_
#define __HAND_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The four "suits".
typedef enum {
  SUIT_MANZU = 0,
  SUIT_PINZU = 1,
  SUIT_SOUZU = 2,
  SUIT_JIHAI = 3
} mj_suit_t;

// A mahjong tile.
// @suit Kind of tile.
// @rank The number attached to the tile, minus 1.
// @red Whether this tile is a red five.
typedef struct {
  mj_suit_t suit;
  uint8_t rank;
  bool red;
} mj_tile;

// Contains data of a hand, for input into the (to be written) hand analyser.
// Each entry of `data` represents a suit (in order: manzu, pinzu, souzu, honors,
// and from least to most significant bits, it is divided into blocks of three
// bits containing the number of each of the successive kinds of tiles.
// The bit thereafter (the 28th) indicates whether the red 5 of that suit is in
// the hand or not.
typedef struct {
  uint32_t data[4];
} mj_hand;

/* `mj_tile` functions */

// Converts a tile id to a `mj_tile`.
mj_tile int_to_tile(long int);

void mj_tile_print(mj_tile);

void mj_tile_print_line(mj_tile);

/* `mj_hand` functions */

// Initialises with zero.
mj_hand mj_hand_init();

void mj_hand_print(mj_hand);

void mj_hand_print_line(mj_hand);

/* Writing functions */

int mj_hand_add_tile(mj_hand *, mj_tile);

int mj_hand_add_tile_by_id(mj_hand *, long int);

int mj_hand_remove_tile(mj_hand *, mj_tile);

int mj_hand_remove_tile_by_id(mj_hand *, long int);

#endif
