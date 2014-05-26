src=src
out=src
ladit=-g

CXXFLAGS=${ladit}

make: libhlib.a

.PHONY: clean
clean:
	rm -rf ${out}/*.o *.a *~

objects=${out}/hoapp.o ${out}/hodc.o ${out}/hoedit.o ${out}/holabel.o\
${out}/holayout.o ${out}/hopushbutton.o ${out}/hoset.o ${out}/hostring.o\
${out}/hotree.o ${out}/howindow.o ${out}/hosocket.o ${out}/hoimage.o \
${out}/holistbox.o ${out}/hothread.o ${out}/holog.o ${out}/homsgbox.o \
${out}/hosemaphore.o ${out}/hoserversocket.o ${out}/hopipe.o ${out}/hoexec.o

headers=${src}/hoapp.h ${src}/hodc.h ${src}/hoedit.h ${src}/holabel.h\
${src}/holayout.h ${src}/hopushbutton.h ${src}/hoset.h ${src}/hostring.h\
${src}/hotree.h ${src}/howindow.h ${src}/hosocket.h ${src}/hoimage.h \
${src}/holistbox.h ${src}/hothread.h ${src}/holog.h ${src}/homsgbox.h \
${src}/hosemaphore.h ${src}/hoserversocket.h ${src}/hopipe.h ${src}/hoexec.h

${objects}: ${headers}

libhlib.a: ${objects}
	ar -cr libhlib.a ${objects}

