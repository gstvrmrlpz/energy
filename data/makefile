CSV = $(wildcard *.csv)
SVG = $(CSV:.csv=.svg)

all: $(SVG)

clean:
	rm -fv $(SVG) *~

%.svg: %.csv | makefile
	gnuplot <<< "set datafile separator ';'; \
				 set terminal svg enhanced; \
				 set output '$@'; \
				 set xlabel 'time (s)'; \
				 set ylabel 'energy (J)'; \
				 plot for [p in 'bitset bvector cvector string'] '<(grep '.p.' $<)' u 6:5 t p w points ps 0.75 pt 7"

.PHONY: all clean
