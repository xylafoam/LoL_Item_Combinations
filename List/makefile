
SOURCE = ../functions/

HEADERS = ../headers/


prime : primeGroups.c $(SOURCE)funcs.c $(HEADERS)funcs.h $(HEADERS)items.h $(HEADERS)primes.h
	gcc -Wall -I $(HEADERS) -o prime primeGroups.c $(SOURCE)funcs.c

stat : newStat.c $(SOURCE)funcs.c $(HEADERS)funcs.h $(HEADERS)items.h
	gcc -Wall -I $(HEADERS) -o stat newStat.c $(SOURCE)funcs.c

list : itemList.c $(SOURCE)funcs.c $(HEADERS)funcs.h $(HEADERS)items.h $(HEADERS)primes.h
	gcc -Wall -I $(HEADERS) -o list itemList.c $(SOURCE)funcs.c
