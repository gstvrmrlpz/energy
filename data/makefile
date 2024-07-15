CSV = $(wildcard *.csv)
SVG = $(CSV:.csv=.svg) $(CSV:.csv=-com.svg) $(CSV:.csv=-dot.svg) $(CSV:.csv=-opt.svg) $(CSV:.csv=-siz.svg) $(CSV:.csv=-wrk.svg)

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
				 plot for [p in 'bitset bvector cvector string'] '<(grep '.p.' $<)' u 8:7 t p w points ps 0.75 pt 7"

%-com.svg: %.csv | makefile
	gnuplot <<< "set datafile separator ';'; \
				 set key bottom right; \
				 set terminal svg enhanced; \
				 set output '$@'; \
				 set xlabel 'time (s)'; \
				 set ylabel 'energy (J)'; \
				 plot for [i in 'clang++ g++'] '<(grep ^'.i.'\; $<)' u 8:7 t i w dots"

%-dot.svg: %.csv | makefile
	gnuplot <<< "set datafile separator ';'; \
				 set key bottom right; \
				 set terminal svg enhanced; \
				 set output '$@'; \
				 set xlabel 'time (s)'; \
				 set ylabel 'energy (J)'; \
				 plot for [p in 'bitset bvector cvector string'] '<(grep '.p.' $<)' u 8:7 t p w dots"

%-opt.svg: %.csv | makefile
	gnuplot <<< "set datafile separator ';'; \
				 set key bottom right; \
				 set terminal svg enhanced; \
				 set output '$@'; \
				 set xlabel 'time (s)'; \
				 set ylabel 'energy (J)'; \
				 plot for [i in 'O2 O3'] '<(grep '.i.' $<)' u 8:7 t i w dots"

%-siz.svg: %.csv | makefile
	gnuplot <<< "set datafile separator ';'; \
				 set key bottom right; \
				 set terminal svg enhanced; \
				 set output '$@'; \
				 set xlabel 'time (s)'; \
				 set ylabel 'energy (J)'; \
				 plot for [i in '512 1024 2048'] '<(grep \;'.i.'\; $<)' u 8:7 t i w dots"

%-wrk.svg: %.csv | makefile
	gnuplot <<< "set datafile separator ';'; \
				 set key bottom right; \
				 set terminal svg enhanced; \
				 set output '$@'; \
				 set xlabel 'time (s)'; \
				 set ylabel 'energy (J)'; \
				 plot for [i in 'g h i'] '<(grep \;'.i.'\; $<)' u 8:7 t i w dots"


.PHONY: all clean
