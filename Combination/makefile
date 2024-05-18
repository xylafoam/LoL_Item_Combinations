
SOURCE = ../functions/

HEADERS = ../headers/



combos : itemCombos.c $(SOURCE)funcs.c $(SOURCE)exprlist.c $(SOURCE)vararray.c $(SOURCE)varstruct.c $(HEADERS)funcs.h $(HEADERS)items.h $(HEADERS)exprlist.h $(HEADERS)vararray.h $(HEADERS)varstruct.h $(HEADERS)primes.h
	gcc -Wall -I $(HEADERS) -o combos itemCombos.c $(SOURCE)funcs.c $(SOURCE)exprlist.c $(SOURCE)vararray.c $(SOURCE)varstruct.c

set : SetCalc.c $(SOURCE)funcs.c $(SOURCE)exprlist.c $(SOURCE)vararray.c $(SOURCE)varstruct.c $(HEADERS)funcs.h $(HEADERS)items.h $(HEADERS)exprlist.h $(HEADERS)vararray.h $(HEADERS)varstruct.h
	gcc -Wall -I $(HEADERS) -o set SetCalc.c $(SOURCE)funcs.c $(SOURCE)exprlist.c $(SOURCE)vararray.c $(SOURCE)varstruct.c
