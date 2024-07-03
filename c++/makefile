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
LOG = $(foreach e,$(EXE),$(foreach o,0 1 2 3 g s z,$(foreach s,$(SIZ),$e-$s-O$o.log)))
SIZ = 256 512 1024 2048
SRC = $(wildcard *.cc)
SVG = $(EXE:=.svg)

###############################################################################

CXXFLAGS = -O3 -std=c++20 -Wall $(OPT)

ifeq ($(SIMD),no)
	CXXFLAGS += -mno-{avx,sse}
endif

###############################################################################

all: pre exe

att: $(ATT)

clean:
	-rm -fv $(ATT) $(CAC) $(CAL) $(DAT) $(EXE) $(FOL) $(SVG) *~

compilers: $(SRC) | c2048
	@printf '%48s\n' | tr ' ' '-'
	printf '%12s%12s%12s%12s\n' program compiler 'energy(J)' 'time(s)'
	printf '%48s\n' | tr ' ' '-'
	for src in $^; do
		for compiler in $(COM); do
			make -Bs $${e/.cc} CXX=$$compiler
			-perf stat -a -e power/energy-pkg/ -r 10 -- ./$${src/.cc} > /dev/null 2> /tmp/stat.txt
			energy=$$(sed -n 's/\([[:digit:].,]*\) Joules .*/\1/p' /tmp/stat.txt)
			cpu=$$(cat /tmp/stat.txt | grep seconds | tr -s ' ' | cut -d' ' -f2)
			printf '%12s%12s%12s%12s\n' $${src/.cc} $$compiler $$energy $$cpu
		done
	done

exe: $(EXE)

perf:
	sudo bash -c "echo -1 > /proc/sys/kernel/perf_event_paranoid"

pre: perf
	@for i in $(COM) perf; do
		hash $$i || echo "$$i not found!!!"
	done

test: $(SRC)
	@printf '%72s\n' | tr ' ' '-'
	printf '%12s%12s%12s%12s%12s%12s\n' program length opt SIMD 'energy(J)' 'time(s)'
	printf '%72s\n' | tr ' ' '-'
	for e in $^; do
		for s in $(SIZ); do
			for o in 0 1 2 3 g s z; do
				for simd in yes no; do
					make -Bs c$$s $${e/.cc} OPT=-O$$o SIMD=$$simd
					-perf stat -a -e power/energy-pkg/ -r 10 -- ./$${e/.cc} > /dev/null 2> /tmp/stat.txt
					energy=$$(sed -n 's/\([[:digit:].,]*\) Joules .*/\1/p' /tmp/stat.txt)
					cpu=$$(cat /tmp/stat.txt | grep secon@printf '%72s\n' | tr ' ' '-'
	printf '%12s%12s%12s%12s%12s%12s\n' program length opt SIMD 'energy(J)' 'time(s)'
	printf '%72s\n' | tr ' ' '-'
	done

test2: $(EXE) | c2048
	@for e in $^; do
		printf "%80s\n \t$$e\n%80s\n" | tr ' ' '#'
		for (( i=0; i<6; ++i )); do
			# -perf stat -a -e power/energy-pkg/ -j -r 10 -- ./$$e $$i > /dev/null
			-perf stat -a -e power/energy-pkg/ -r 10 -- ./$$e $$i > /dev/null
		done
	done

simd: $(SRC)
	@printf '%72s\n' | tr ' ' '-'
	printf '%12s%12s%12s%12s%12s%12s\n' program length opt SIMD 'energy(J)' 'time(s)'
	printf '%72s\n' | tr ' ' '-'
	for e in $^; do
		for s in $(SIZ); do
			for o in 0 1 2 3 g s z; do
				for simd in yes no; do
					make -Bs c$$s $${e/.cc} OPT=-O$$o SIMD=$$simd
					-perf stat -a -e power/energy-pkg/ -r 10 -- ./$${e/.cc} > /dev/null 2> /tmp/stat.txt
					energy=$$(sed -n 's/\([[:digit:].,]*\) Joules .*/\1/p' /tmp/stat.txt)
					cpu=$$(cat /tmp/stat.txt | grep seconds | tr -s ' ' | cut -d' ' -f2)
					printf '%12s%12s%12s%12s%12s%12s\n' $${e/.cc} $$s "-O$$o" $$simd $$energy $$cpu
				done
			done
		done
	done

svg: $(SVG)

xps: vector
	@for (( i=0; i<6; ++i )); do
		-perf stat -a -e power/energy-cores/,power/energy-gpu/,power/energy-pkg/,power/energy-psys/,power/energy-ram/ -r 10 -- ./$< $$i > /dev/null
	done

###############################################################################

c%: $(SRC)
	@c=$@
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

.PHONY: all att clean compilers exe perf pre test test2 simd svg xps
.PRECIOUS: $(DAT) $(FOL)

###############################################################################
