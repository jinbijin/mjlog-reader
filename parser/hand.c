/* `hand.c`, by Jinbi Jin */

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hand.h"

/* `mj_tile` functions */

// Converts a tile id to a `mj_tile`.
// Argument is a `long int` as that is the return type of `strtol`.
mj_tile int_to_tile(long int id) {
  mj_tile tile;
  assert(id >= 0);
  assert(id < 4*34);
  tile.suit = id / 36;
  tile.rank = (id / 4) % 9;
  tile.red = (id % 36 == 16);
  return tile;
}

/* `mj_suit` functions */

void mj_suit_print(mj_suit suit) {
  switch (suit) {
    case SUIT_MANZU:
      printf("m");
      break;
    case SUIT_PINZU:
      printf("p");
      break;
    case SUIT_SOUZU:
      printf("s");
      break;
    case SUIT_JIHAI:
      printf("z");
      break;
  }
}

void mj_suit_print_line(mj_suit suit) {
  mj_suit_print(suit);
  printf("\n");
}

void mj_tile_print(mj_tile tile) {
  if (tile.red) {
    printf("0");
  }
  else {
    printf("%d",tile.rank);
  }
}

void mj_tile_print_line(mj_tile tile) {
  mj_tile_print(tile);
  printf("\n");
}

/* `mj_hand` functions */

// Initialises with zero.
mj_hand mj_hand_init() {
  mj_hand hand;
  int i;
  for (i = 0; i < 4; i++) {
    hand.data[i] = 0;
  }
}

bool mj_hand_has_red_of(mj_hand hand, mj_suit suit) {
  return (hand.data[suit] >> 27) % 2 == 1;
}

uint8_t mj_hand_number_of(mj_hand hand, mj_suit suit, uint8_t rank) {
  assert(rank >= 0);
  assert(rank < 9);
  return (hand.data[suit] >> 3*rank) % 8;
}

void mj_hand_print(mj_hand hand) {
  int i;
  uint8_t j, k;
  int count = 0;
  uint8_t red = 0;
  for (i = 0; i < 4; i++) {
    count = 0;
    for (j = 0; j < 9; j++) {
      if (j == 4 && mj_hand_has_red_of(hand, i)) {
	printf("0");
	red = 1;
	count++;
      }
      else {
	red = 0;
      }
      for (k = red; k < mj_hand_number_of(hand, i, j); k++) {
	printf("%d",j+1);
	count++;
      }
    }
    if (count > 0) {
      mj_suit_print(i);
    }
  }
}

void mj_hand_print_line(mj_hand hand) {
  mj_hand_print(hand);
  printf("\n");
}

/* Writing functions */

int mj_hand_add_tile(mj_hand *this, mj_tile tile) {
  if (tile.red) {
    if (mj_hand_has_red_of(*this, tile.suit)) {
      printf("Already contains this red five, cannot add.\n");
      return EXIT_FAILURE;
    }
    else {
      this->data[tile.suit] += (1 << 27);
    }
  }
  if (mj_hand_number_of(*this, tile.suit, tile.rank) == 4) {
    printf("Too many of ");
    mj_tile_print(tile);
    printf(", cannot add.\n");
    return EXIT_FAILURE;
  }
  else {
    this->data[tile.suit] += (1 << 3*(tile.rank));
  }
  return EXIT_SUCCESS;
}

int mj_hand_add_tile_by_id(mj_hand *this, long int id) {
  if (mj_hand_add_tile(this, int_to_tile(id)) == EXIT_FAILURE) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int mj_hand_remove_tile(mj_hand *this, mj_tile tile) {
  if (tile.red) {
    if (mj_hand_has_red_of(*this, tile.suit)) {
      this->data[tile.suit] -= (1 << 27);
    }
    else {
      printf("No red fives, cannot remove.\n");
      return EXIT_FAILURE;
    }
  }
  if (mj_hand_number_of(*this, tile.suit, tile.rank) == 0) {
    printf("No tile of ");
    mj_tile_print(tile);
    printf(", cannot remove.\n");
    return EXIT_FAILURE;
  }
  else {
    this->data[tile.suit] -= (1 << 3*(tile.rank));
  }
  return EXIT_SUCCESS;
}

int mj_hand_remove_tile_by_id(mj_hand *this, long int id) {
  if (mj_hand_remove_tile(this, int_to_tile(id)) == EXIT_FAILURE) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
