// Parser for version 2.3 mjlog files.
#ifndef _MJ_PARSER_H_
#define _MJ_PARSER_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "dstr.h"
#include "token.h"
#include "tlist.h"
#include "lexer.h"
#include "attr.h"
#include "elt.h"

// Nodes:
// * TAG [A-Z]+[0-9]* has a list of attributes (max 16 @ AGARI) as children and
//   a list of subtags as children.
// * ATTRIBUTE [a-z]+[0-9]* has a list of values (max 14 @ hai[0-3]) as children
// * VALUE .+ is terminal (dynamic string)
// Every tag and attribute name fits in 16 bytes (i.e. has size at most 15)
//
// Modes:
// * Normal (separator = ' ' or a change of character class)
// * Value (separator = ',' or '"')

#endif
