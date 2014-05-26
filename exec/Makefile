.PHONY: all

.PHONY: calc

.PHONY: pipe

all: hlib calc pipe

hlib:
	(cd ../hlib; make)

pipe: hlib
	(cd pipe; make)

calc: hlib
	(cd calc; make)


