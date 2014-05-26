.PHONY: all

.PHONY: hlib

.PHONY: layout

.PHONY: listbox

.PHONY: sachy

.PHONY: grafy

.PHONY: miny

.PHOHY: evoluce

.PHOHY: neuro

.PHOHY: neuro

.PHONY: thread

.PHONY: prvni

.PHONY: exec

.PHONY: sr

.PHONY: sachylib

.PHONY: clean

.PHONY: distrib

all: hlib sachylib sachy layout listbox grafy miny prvni thread evoluce neuro argbview obdelniky exec sr

thread: hlib
	(cd thread; make)

sachylib:
	(cd sachylib; make)

sachy: hlib
	(cd sachy; make)
	
layout: hlib
	(cd layout; make)

listbox: hlib
	(cd listbox; make)

grafy: hlib
	(cd grafy; make)

prvni: hlib
	(cd prvni; make)

miny: hlib
	(cd miny; make)

evoluce: hlib
	(cd evoluce; make)

neuro: hlib
	(cd neuro; make)

argbview: hlib
	(cd argbview; make)

obdelniky: hlib
	(cd obdelniky; make)


exec: hlib
	(cd exec; make)

hlib:
	(cd hlib; make)

sr: hlib sachylib
	(cd sr; make)

clean:
	for x in *;\
	do if [ -d $$x ];\
	then (cd $$x; make clean);\
	fi;\
	done;\
	find . -name "*~" -o -name "*.plg" -o -name "*.opt"  | while read x; do rm -rf $$x; done;\
	find . -name "Debug" -o -name "Release" -exec rm -rf {} \;

distrib: clean
	cd ..;\
	tar -c hlib2 > hlib2.tar;\
	gzip hlib2.tar;\

