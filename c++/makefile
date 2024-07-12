###############################################################################
# makefile
###############################################################################

.ONESHELL:

###############################################################################

ATT = $(EXE:=.att)
CAC = $(EXE:=.cac)
CAL = $(EXE:=.cal)
COM = clang++ g++
DAT = $(EXE:=.dat)
EXE = $(basename $(SRC))
FOL = $(EXE:=.fol)
LEV = -O2 -O3
OUT = csv
REP = 15
SIZ = 512 1024 2048
SRC = $(wildcard *.cc)
SVG = $(EXE:=.svg)

###############################################################################

CXXFLAGS = -flto -g -march=native -O3 -std=c++23 -Wall $(OPT)

###############################################################################

all: pre exe

att: $(ATT)

clean:
	-make -s $(firstword $(SIZ))z
	-rm -fv $(ATT) $(CAC) $(CAL) $(DAT) $(EXE) $(FOL) $(SVG) *~

exe: $(EXE)

perf:
	@sudo bash -c "echo -1 > /proc/sys/kernel/perf_event_paranoid"

pre: perf
	@for i in $(COM) perf; do
		hash $$i || echo "$$i not found!!!"
	done

svg: $(SVG)

###############################################################################

init: $(SRC) | pre
	@if [ "$(OUT)" == "csv" ]; then
		FORMAT='#%s;%s;%s;%s;%s;%s;%s\n'
		printf $$FORMAT program length compiler opt 'energy(J)' 'time(s)' run
		FORMAT='%s;%s;%s;%s;%s;%s;%s\n'
	else
		FORMAT='#%13s%14s%14s%14s%14s%14s%14s\n'
		printf '#%97s\n' | tr ' ' '-'
		printf $$FORMAT program length compiler opt 'energy(J)' 'time(s)' run
		printf '#%97s\n' | tr ' ' '-'
		FORMAT='%14s%14s%14s%14s%14s%14s%14s\n'
	fi
	for compiler in $(COM); do
		for level in $(LEV); do
			for src in $(SRC); do
				for size in $(SIZ); do
					make -Bs $${size}z $${src/.cc} CXX=$$compiler OPT=$$level
					for (( i=0; i<$(REP); ++i )); do
						-perf stat -a -e power/energy-pkg/ -- ./$${src/.cc} init 2> /tmp/stat.txt
						energy=$$(sed -n 's/\([[:digit:].,]*\) Joules .*/\1/p' /tmp/stat.txt)
						cpu=$$(grep seconds /tmp/stat.txt | tr -s ' ' | cut -d' ' -f2)
						printf $$FORMAT $${src/.cc} $$size $$compiler $$level $$energy $$cpu $$(( $$(date +%s%N) / 1000000 ))
					done
				done
			done
		done
	done

full: $(SRC) | pre
	@if [ "$(OUT)" == "csv" ]; then
		FORMAT='#%s;%s;%s;%s;%s;%s;%s\n'
		printf $$FORMAT program length compiler opt 'energy(J)' 'time(s)' run
		FORMAT='%s;%s;%s;%s;%s;%s;%s\n'
	else
		FORMAT='#%13s%14s%14s%14s%14s%14s%14s\n'
		printf '#%97s\n' | tr ' ' '-'
		printf $$FORMAT program length compiler opt 'energy(J)' 'time(s)' run
		printf '#%97s\n' | tr ' ' '-'
		FORMAT='%14s%14s%14s%14s%14s%14s%14s\n'
	fi
	for compiler in $(COM); do
		for level in $(LEV); do
			for src in $(SRC); do
				for size in $(SIZ); do
					make -Bs $${size}z $${src/.cc} CXX=$$compiler OPT=$$level
					for (( i=0; i<$(REP); ++i )); do
						-perf stat -a -e power/energy-pkg/ -- ./$${src/.cc} 2> /tmp/stat.txt
						energy=$$(sed -n 's/\([[:digit:].,]*\) Joules .*/\1/p' /tmp/stat.txt)
						cpu=$$(grep seconds /tmp/stat.txt | tr -s ' ' | cut -d' ' -f2)
						printf $$FORMAT $${src/.cc} $$size $$compiler $$level $$energy $$cpu $$(( $$(date +%s%N) / 1000000 ))
					done
				done
			done
		done
	done

datos: pre
	make init | tee $$(HOSTNAME%%.*)-init-$$(date "+%Y%m%d-%H%M%S").dat
	make full | tee $$(HOSTNAME%%.*)-full-$$(date "+%Y%m%d-%H%M%S").dat

###############################################################################
# pruebas
###############################################################################

xps: vector | pre
	@for (( i=0; i<6; ++i )); do
		-perf stat -a -e power/energy-cores/,power/energy-gpu/,power/energy-pkg/,power/energy-psys/,power/energy-ram/ -r 10 -- ./$< $$i > /dev/null
	done

###############################################################################

%z: common.h
	@c=$@
	sed --follow-symlinks -i "s/CHROMOSOME_SIZE = [[:digit:]]*/CHROMOSOME_SIZE = $${c:0:-1}/" $<

%.att: %
	objdump -Cd $< > $@

%.cga: %.cac
	cg_annotate --auto=yes $<

%.dat: %
#	-perf record --call-graph=dwarf -o $@ -- ./$<
#	-sudo perf record -a -e power/energy-pkg/ --freq=100 -o $@ -- ./$<
	-sudo perf record -a -e '{cycles,power/energy-pkg/}:S' --freq=100 -o $@ -- ./$<

%.cac: %
	-valgrind --cachegrind-out-file=$@ --cache-sim=yes --demangle=yes --tool=cachegrind ./$<

%.cal: %
	-valgrind --branch-sim=yes --cacheuse=yes --callgrind-out-file=$@ --cache-sim=yes --collect-jumps=yes --demangle=yes --dump-instr=yes --instr-atstart=yes -q --separate-threads=yes --tool=callgrind --trace-jump=yes ./$<

%.fg: %.svg
	eog $< &

%.fol: %.dat
	-perf script -i $< | stackcollapse-perf.pl | grep -v cpu_idle > $@

%.kcg: %.cal
	kcachegrind $<

%.leak: %
#	valgrind --leak-check=full --show-error-list=yes --show-leak-kinds=all --track-origins=yes --verbose ./$<
	valgrind --leak-check=summary --show-leak-kinds=all --track-origins=yes ./$<

%.ltrace: %
	@printf "%79s\n \t$*\n%79s\n" | tr ' ' '#'
	ltrace -cf ./$< |& c++filt | cut -c 1-$$(tput cols)

%.perf: %.dat
	-perf report -i $<

%.strace: %
	@printf "%79s\n \t$*\n%79s\n" | tr ' ' '#'
	strace -cf -qq ./$< |& c++filt | cut -c 1-$$(tput cols)

%.svg: %.fol
	flamegraph.pl $< > $@

%.trace: %
	@printf "%79s\n \t$*\n%79s\n" | tr ' ' '#'
	ltrace -cfS ./$< |& c++filt | cut -c 1-$$(tput cols)

###############################################################################

.PHONY: all att clean exe full init perf pre svg xps
.PRECIOUS: $(DAT) $(FOL)

###############################################################################
