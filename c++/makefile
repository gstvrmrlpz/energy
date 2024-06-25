###############################################################################
# makefile
###############################################################################

.ONESHELL:

###############################################################################

CAC = $(EXE:=.cac)
CAL = $(EXE:=.cal)
DAT = $(EXE:=.dat)
EXE = $(basename $(SRC))
FOL = $(EXE:=.fol)
SRC = $(wildcard *.cc)
SVG = $(EXE:=.svg)

###############################################################################

#CXXFLAGS = -fanalyzer -ftlo -g -Og -std=c++20 -Wall
CXXFLAGS = -fanalyzer -flto -g -O3 -std=c++20 -Wall

###############################################################################

all: test

clean:
	-rm -fv $(CAC) $(CAL) $(DAT) $(EXE) $(FOL) $(SVG) *~

exe: $(EXE)

test: $(SRC)
	@printf '%16s%16s%16s%16s\n' program length 'energy(J)' 'time(s)'
	printf '%64s\n' | tr ' ' '-'
	for e in $^; do
		for b in 256 512 1024 2048; do
			make -s c$$b $${e/.cc}
			sudo perf stat -a -e power/energy-pkg/ -- ./$${e/.cc} > /dev/null 2> /tmp/stat.txt
			energy=$$(sed -n 's/\([[:digit:].,]*\) Joules .*/\1/p' /tmp/stat.txt)
			cpu=$$(sed -n 's/\([[:digit:].,]*\) seconds .*/\1/p' /tmp/stat.txt)
			printf '%16s%16s%16s%16s\n' $${e/.cc} $$b $$energy $$cpu
		done
	done

test2: vector
	for (( i=0; i<6; ++i )); do
		sudo perf stat -a -e power/energy-pkg/ -r 10 -- ./$< $$i > /dev/null
	done

svg: $(SVG)

###############################################################################

c%: $(SRC)
	c=$@
	sed --follow-symlinks -i "s/CHROMOSOME_SIZE = [[:digit:]]*/CHROMOSOME_SIZE = $${c:1}/" $^

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

.PHONY: all clean exe test test2 svg
.PRECIOUS: $(DAT) $(FOL)

###############################################################################
