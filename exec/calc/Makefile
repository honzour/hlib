src=src
out=src
ladit=-O3

CXXFLAGS=${ladit} -I../../hlib/src

hlib=../../hlib/libhlib.a
output=exectest

make: buildprogram

.PHONY: buildprogram

buildprogram: ${output}

.PHONY: clean
clean:
	rm -rf ${output} ${src}/*.o Debug Release
	
objects=${out}/main.o

${objects}: ${hlib}

${output}: ${objects}
	g++ ${objects}  -L../../hlib -lhlib -L/usr/X11R6/lib -lX11 -lpthread ${ladit} -o ${output}
	

