src=src
out=src
ladit=-O3

CXXFLAGS=${ladit} -I../hlib/src

hlib=../hlib/libhlib.a
output=neuro

make: buildprogram

.PHONY: buildprogram

buildprogram: buildhlib ${output}

.PHONY: buildhlib
buildhlib:
	(cd ../hlib; make)

.PHONY: clean
clean:
	rm -f ${output} ${src}/*.o
	
objects=${out}/main.o ${out}/ns.o

${objects}: ${hlib}

${output}: ${objects}
	g++ ${objects}  -L../hlib -lhlib -L/usr/X11R6/lib -lX11 -lpthread ${ladit} -o ${output}
	

