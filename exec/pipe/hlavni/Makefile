src=src
out=src
ladit=-g

CXXFLAGS=${ladit} -I../../../hlib/src

output=hlavni

make: buildprogram

.PHONY: buildprogram

buildprogram: ${output}

.PHONY: clean
clean:
	rm -rf ${output} ${src}/*.o Debug Release
	
objects=${out}/main.o

${objects}: ${hlib}

${output}: ${objects}
	g++ ${objects} -L/usr/X11R6/lib -lX11 -lpthread -L../../../hlib -lhlib ${ladit} -o ${output}
	

