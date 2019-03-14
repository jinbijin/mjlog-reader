.PHONY = clean run

SRC_DIR = 2017

FILE = mjlog

IP_LIB_FILE = mjip
IP_LIB_C = lib/ferrorloc.c lib/parser.c lib/dstr.c lib/token.c lib/tlist.c \
lib/lexer.c lib/attr.c lib/elt.c
IP_LIB_H := $(IP_LIB_C:%.c=%.h)

all : lib$(IP_LIB_FILE) $(FILE)

lib$(IP_LIB_FILE) : $(IP_LIB_H) $(IP_LIB_C)
	gcc-7 -O3 -dynamiclib -o lib/lib$(IP_LIB_FILE).dylib $(IP_LIB_C)

$(FILE) : $(FILE).c lib/lib$(IP_LIB_FILE).dylib
	gcc-7 -O3 -o $(FILE).o $(FILE).c -Ilib -Llib -l$(IP_LIB_FILE)

run : $(FILE).o
	./$(FILE).o $(SRC_DIR) | gnomon
