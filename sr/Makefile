CXXFLAGS=-g -I../hlib/src/ -I../sachylib/
objects=sr.o struktury.o vlakno2.o rutiny.o global.o vlaknok.o vlakno3.o vlaknors.o vlakno4.o fronta.o zpracuj.o db.o

headers=struktury.h vlakno2.h rutiny.h global.h vlaknok.h vlakno3.h vlaknors.h vlakno4.h zpravy.h zprava.h fronta.h zpracuj.h db.h

.PHONY: all

all: buildprogram ovladac

.PHONY: buildprogram

buildprogram: buildhlib buildsachylib sr

.PHONY: buildsachylib
buildsachylib:
	(cd ../sachylib; make)

.PHONY: buildhlib
buildhlib:
	(cd ../hlib; make)

sr: ${objects}
	g++ ${objects} -o sr -g -lpthread -L../sachylib -lsachy -L../hlib -lhlib -L/usr/X11R6/lib -lX11

${objects}: ${headers}

ovladac: ovladac.cpp
	g++ ovladac.cpp -o ovladac

.PHONY: clean

clean:
	rm -rf *.o sr ovladac *~

test.o: test.cpp ${headers}
test: struktury.o test.o
	g++ struktury.o test.o -o test

