src=src
out=src
ladit=-g

CXXFLAGS=${ladit} -I../hlib/src -I../sachylib

hlib=../hlib/libhlib.a
output=sachy

make: buildprogram

.PHONY: buildprogram

buildprogram: buildhlib buildsachylib ${output}

.PHONY: buildhlib
buildhlib:
	(cd ../hlib; make)

.PHONY: buildsachylib
buildsachylib:
	(cd ../sachylib; make)

.PHONY: clean
clean:
	rm -f ${output} ${src}/*.o ${src}/*~;
	
objects=${out}/main.o ${out}/stolkywindow.o ${out}/global.o ${out}/push.o ${out}/vlakna.o ${out}/zprava.o ${out}/onevent.o ${out}/loginwindow.o ${out}/struktury.o ${out}/stolkylistbox.o ${out}/sachwindow.o
headers=${src}/zpravy.h ${src}/zprava.h ${src}/stolkywindow.h ${src}/global.h ${src}/push.h ${src}/vlakna.h ${src}/onevent.h ${src}/loginwindow.h ${src}/struktury.h ${src}/stolkylistbox.h ${src}/sachwindow.h

${objects}: ${hlib}

${objects}: ${headers}

${output}: ${objects}
	g++ ${objects} -lm  -L../sachylib -lsachy -L../hlib -lhlib -L/usr/X11R6/lib -lX11 -lpthread ${ladit} -o ${output}
	

