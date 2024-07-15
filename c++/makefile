###############################################################################
# makefile
###############################################################################

.ONESHELL:

###############################################################################

COM = clang++ g++
EXE = $(basename $(SRC))
LEV = -O2 -O3
OUT = csv
REP = 100
SIZ = 512 1024 2048
SRC = $(wildcard *.cc)
WRK = g h i

###############################################################################

CXXFLAGS = -flto -march=native -O3 -std=c++20 -Wall $(OPT)

###############################################################################

all: exe

att: $(ATT)

clean:
	-rm -fv $(CAC) $(CAL) $(EXE) core* *~
	-for i in $$(pgrep make); do
		if [ "$$i" != "$$$$" ]; then
			for j in $$(pgrep --parent $$i sh); do
				kill $$j
			done
		fi
	done

exe: $(EXE)

perf:
	@sudo bash -c "echo -1 > /proc/sys/kernel/perf_event_paranoid"

pre: perf
	@for i in $(COM) perf; do
		hash $$i || $$(echo "$$i not found!!!" && exit 1)
	done

###############################################################################

data:
	@id="$$(date '+%Y%m%d-%H%M%S')"
	for w in $(WRK); do
		make -s exp WRK=$$w | tee $${HOSTNAME%%.*}-$$w-$$id.$(OUT)
	done

exp: pre
	@LANG=C
	if [ "$(OUT)" == "csv" ]; then
		FORMAT='#%s;%s;%s;%s;%s;%s;%s\n'
		printf $$FORMAT program length compiler opt 'energy(J)' 'time(s)' seed
		FORMAT='%s;%s;%s;%s;%s;%s;%s\n'
	else
		FORMAT='#%13s%14s%14s%14s%14s%14s%14s\n'
		printf '#%97s\n' | tr ' ' '-'
		printf $$FORMAT program length compiler opt 'energy(J)' 'time(s)' seed
		printf '#%97s\n' | tr ' ' '-'
		FORMAT='%14s%14s%14s%14s%14s%14s%14s\n'
	fi
	for compiler in $(COM); do
		for level in $(LEV); do
			for exe in $(EXE); do
				make -Bs $$exe CXX=$$compiler OPT=$$level
				for size in $(SIZ); do
					for work in $(WRK); do
						for seed in $$(seq -w 0 $$(( $(REP) - 1 ))); do
							-perf stat -a -e power/energy-pkg/ 2> /tmp/stat.txt -- ./$$exe -r $$seed -s $$size -w $$work
							energy=$$(sed -n 's/\([[:digit:].,]*\) Joules .*/\1/p' /tmp/stat.txt)
							cpu=$$(grep seconds /tmp/stat.txt | tr -s ' ' | cut -d' ' -f2)
							printf $$FORMAT $$exe $$size $$compiler $$level $$energy $$cpu $$seed
						done
					done
				done
			done
		done
	done

data2:
	@make -s exp2 | tee $${HOSTNAME%%.*}-$$(date '+%Y%m%d-%H%M%S').$(OUT)

exp2: pre
	@LANG=C
	if [ "$(OUT)" == "csv" ]; then
		FORMAT='#%s;%s;%s;%s;%s;%s;%s;%s\n'
		printf $$FORMAT compiler opt program seed length work 'energy(J)' 'time(s)'
		FORMAT='%s;%s;%s;%s;%s;%s;%s;%s\n'
	else
		FORMAT='#%13s%14s%14s%14s%14s%14s%14s%14s\n'
		printf '#%111s\n' | tr ' ' '-'
		printf $$FORMAT compiler opt program seed length work 'energy(J)' 'time(s)'
		printf '#%111s\n' | tr ' ' '-'
		FORMAT='%14s%14s%14s%14s%14s%14s%14s%14s\n'
	fi
	for compiler in $(COM); do
		for level in $(LEV); do
			for exe in $(EXE); do
				make -Bs $$exe CXX=$$compiler OPT=$$level
				for size in $(SIZ); do
					for work in $(WRK); do
						for seed in $$(seq -w 0 $$(( $(REP) - 1 ))); do
							energy=""
							while [[ -z $$energy || ! $$energy =~ ^[+-]?[0-9]+([.][0-9]+)?$$ ]]; do
								-perf stat -a -e power/energy-pkg/ 2> /tmp/stat.txt -- ./$$exe -r $$seed -s $$size -w $$work
								energy=$$(sed -n 's/\([[:digit:].,]*\) Joules .*/\1/p' /tmp/stat.txt)
								cpu=$$(grep seconds /tmp/stat.txt | tr -s ' ' | cut -d' ' -f2)
							done
							printf $$FORMAT $$compiler $$level $$exe  $$seed $$size $$work $$energy $$cpu
						done
					done
				done
			done
		done
	done

###############################################################################
# pruebas
###############################################################################

xps: vector | pre
	@for (( i=0; i<6; ++i )); do
		-perf stat -a -e power/energy-cores/,power/energy-gpu/,power/energy-pkg/,power/energy-psys/,power/energy-ram/ -r 10 -- ./$< $$i > /dev/null
	done

###############################################################################

.PHONY: all att clean csv exe exp exp2 perf pre svg xps

###############################################################################
