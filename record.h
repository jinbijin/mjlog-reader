#ifndef _MJ_RECORD_H_
#define _MJ_RECORD_H_

#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "lib/dstr.h"
#include "lib/attr.h"
#include "lib/elt.h"
#include "lib/state.h"

// Typically `mj_record *` is a pointer into allocated memory.
typedef void mj_record;

void record_init (mj_record **);

void record_delete (mj_record *);

// Called after interpretation.
// Outputs to `stdout`, with intention to be redirected into file.
void record_print_lines (const mj_record *);

// May involve reallocations of the underlying data.
void record_update (mj_record **, const mj_state *, const mj_elt *, ptrdiff_t);

#endif
