CSV = $(wildcard *.csv)
PS  = 0.75
PT  = 7
SVG = $(CSV:.csv=.svg) $(CSV:.csv=-co.svg) $(CSV:.csv=-com.svg) $(CSV:.csv=-dot.svg) $(CSV:.csv=-opt.svg) $(CSV:.csv=-ps.svg) $(CSV:.csv=-siz.svg) $(CSV:.csv=-wrk.svg)

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
				 plot for [p in 'bitset bvector cvector string'] '<(grep '.p.' $<)' u 8:7 t p w p ps $(PS) pt $(PT)"

%-co.svg: %.csv | makefile
	gnuplot <<< "set datafile separator ';'; \
				 set key bottom right; \
				 set terminal svg enhanced; \
				 set output '$@'; \
				 set xlabel 'time (s)'; \
				 set ylabel 'energy (J)'; \
				 plot for [c in 'clang++ g++'] for [o in 'O2 O3'] '<(grep ^'.c.'\; $< | grep '.o.')' u 8:7 t c.' -'.o w p ps $(PS) pt $(PT)"

%-com.svg: %.csv | makefile
	gnuplot <<< "set datafile separator ';'; \
				 set key bottom right; \
				 set terminal svg enhanced; \
				 set output '$@'; \
				 set xlabel 'time (s)'; \
				 set ylabel 'energy (J)'; \
				 plot for [i in 'clang++ g++'] '<(grep ^'.i.'\; $<)' u 8:7 t i w p ps $(PS) pt $(PT)"

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
				 plot for [i in 'O2 O3'] '<(grep '.i.' $<)' u 8:7 t '-'.i w p ps $(PS) pt $(PT)"

%-ps.svg: %.csv | makefile
	gnuplot <<< "set datafile separator ';'; \
				 set key bottom right; \
				 set terminal svg enhanced; \
				 set output '$@'; \
				 set xlabel 'time (s)'; \
				 set ylabel 'energy (J)'; \
				 plot for [p in 'bitset bvector cvector string'] for [s in '512 1024 2048'] '<(grep '.p.' $< | grep '.s.' | grep clang++ | grep O3)' u 8:7 t p.' -s'.s w p ps $(PS) pt $(PT)"

%-siz.svg: %.csv | makefile
	gnuplot <<< "set datafile separator ';'; \
				 set key bottom right; \
				 set terminal svg enhanced; \
				 set output '$@'; \
				 set xlabel 'time (s)'; \
				 set ylabel 'energy (J)'; \
				 plot for [s in '512 1024 2048'] '<(grep \;'.s.'\; $<)' u 8:7 t '-s'.s w p ps $(PS) pt $(PT)"

%-wrk.svg: %.csv | makefile
	gnuplot <<< "set datafile separator ';'; \
				 set key bottom right; \
				 set terminal svg enhanced; \
				 set output '$@'; \
				 set xlabel 'time (s)'; \
				 set ylabel 'energy (J)'; \
				 plot for [w in 'g h i'] '<(grep \;'.w.'\; $<)' u 8:7 t '-w'.w w p ps $(PS) pt $(PT)"

.PHONY: all clean
