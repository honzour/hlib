src=src
out=src
ladit=-g

CFLAGS=${ladit}

make: libsachy.a

.PHONY: clean
clean:
	rm -rf *.o *.a *~

objects=cotyvar.o generato.o globruti.o hashtab.o lokruti.o partie.o sachy.o

headers=cotyvar.h generato.h globruti.h hashtab.h lokruti.h partie.h pole.h sachy.h volby.h

${objects}: ${headers}

libsachy.a: ${objects}
	ar -cr libsachy.a ${objects}

