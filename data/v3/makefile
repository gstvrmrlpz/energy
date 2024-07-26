.ONESHELL:

CSV = $(wildcard *.csv)
GP  = $(wildcard *.gp)
SVG = $(wildcard *.svg)

all: plot

in: $(GP)
	for i in $^; do
		sed -i "s/^files='.*'$$/files='$(CSV)'/" $$i
	done

clean:
	-rm -fv $(SVG) *~

plot: $(GP) | in
	for i in $^; do
		gnuplot $$i
	done

.PHONY: all clean in plot
