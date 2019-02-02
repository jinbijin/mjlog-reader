FILE = mjlog
LIB_FILE = parser
LIB_C = parser.c parser/error.c parser/token.c parser/tlist.c parser/lexer.c parser/hand.c parser/event.c parser/term.c parser/round.c parser/match.c
LIB_H := $(LIB_C:%.c=%.h)
QUERY = mjquery
SRC_DIR = 2017

all : $(LIB_FILE) $(QUERY) $(FILE)

$(LIB_FILE) : $(LIB_H) $(LIB_C)
	gcc-7 -O3 -dynamiclib -o lib$(LIB_FILE).dylib $(LIB_C)

$(QUERY) : $(QUERY).h $(QUERY).c
	gcc-7 -O3 -dynamiclib -o lib$(QUERY).dylib $(QUERY).c

$(FILE) : $(FILE).c $(LIB_H) $(LIB_C) $(QUERY).h $(QUERY).c
	gcc-7 -O3 -o $(FILE).o $(FILE).c -L. -l$(LIB_FILE) -l$(QUERY)

run : $(FILE).o
	time ./$(FILE).o $(SRC_DIR)

clean :
	rm -f *.o *.dylib
