CSV = $(wildcard *.csv)
SVG = $(CSV:.csv=.svg) $(CSV:.csv=-dot.svg) $(CSV:.csv=-com.svg)

all: $(SVG)

clean:
	rm -fv $(SVG) *~

%.svg: %.csv | makefile
	gnuplot <<< "set datafile separator ';'; \
				 set key bottom right; \
				 set terminal svg enhanced; \
				 set output '$@'; \
				 set xlabel 'time (s)'; \
				 set ylabel 'energy (J)'; \
				 plot for [p in 'bitset bvector cvector string'] '<(grep '.p.' $<)' u 6:5 t p w points ps 0.75 pt 7"

%-com.svg: %.csv | makefile
	gnuplot <<< "set datafile separator ';'; \
				 set key bottom right; \
				 set terminal svg enhanced; \
				 set output '$@'; \
				 set xlabel 'time (s)'; \
				 set ylabel 'energy (J)'; \
				 plot for [c in 'clang++ g++'] '<(grep \;'.c.'\; $<)' u 6:5 t c w dots"

%-dot.svg: %.csv | makefile
	gnuplot <<< "set datafile separator ';'; \
				 set key bottom right; \
				 set terminal svg enhanced; \
				 set output '$@'; \
				 set xlabel 'time (s)'; \
				 set ylabel 'energy (J)'; \
				 plot for [p in 'bitset bvector cvector string'] '<(grep '.p.' $<)' u 6:5 t p w dots"

.PHONY: all clean
