/* `error.h`, by Jinbi Jin */

#ifndef __ERROR_H_
#define __ERROR_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mj_error_alloc();

void mj_error_file(const char *);

void mj_error_null_pointer();

void mj_error_sub_append_empty();

void mj_error_tile_overflow(const char *);

void mj_error_tile_underflow(const char *);

void mj_error_unexpected_eof();

#endif
