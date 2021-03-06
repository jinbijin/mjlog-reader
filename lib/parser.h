// Parser for version 2.3 mjlog files.
#ifndef _MJ_PARSER_H_
#define _MJ_PARSER_H_

#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dstr.h"
#include "ferrorloc.h"
#include "attr.h"
#include "elt.h"

void parser (mj_elt **elt, const char *);

#endif
