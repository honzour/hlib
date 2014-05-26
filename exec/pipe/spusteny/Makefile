src=src
out=src
ladit=-O3

CXXFLAGS=${ladit}

output=spusteny

make: buildprogram

.PHONY: buildprogram

buildprogram: ${output}

.PHONY: clean
clean:
	rm -rf ${output} ${src}/*.o Debug Release
	
objects=${out}/main.o

${objects}: ${hlib}

${output}: ${objects}
	g++ ${objects} ${ladit} -o ${output}
	

