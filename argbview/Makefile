src=src
out=src
ladit=-g

CXXFLAGS=${ladit} -I../hlib/src

hlib=../hlib/libhlib.a
output=argbview

make: buildprogram

.PHONY: buildprogram

buildprogram: buildhlib ${output}

.PHONY: buildhlib
buildhlib:
	(cd ../hlib; make)

.PHONY: clean
clean:
	rm -rf ${output} ${src}/*.o Debug Release
	
objects=${out}/main.o

${objects}: ${hlib}

${output}: ${objects}
	g++ ${objects} -L../hlib -lhlib -L/usr/X11R6/lib -lX11 -lpthread ${ladit} -o ${output}
	

