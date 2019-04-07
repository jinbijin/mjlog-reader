#ifndef _MJ_INTERPRETER_H_
#define _MJ_INTERPRETER_H_

#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dstr.h"
#include "ferrorloc.h"
#include "state.h"
#include "record.h"

void interpreter (mj_record **, const char *);

#endif
