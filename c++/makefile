###############################################################################
# makefile
###############################################################################

.ONESHELL:

###############################################################################

COM = clang++ g++
CSV = $(wildcard *.csv)
EXE = $(basename $(SRC))
OPT = -O2 -O3
OUT = csv
REP = 15
SIZ = 512 1024 2048
SRC = $(wildcard *.cc)
STD = -std=c++17 -std=c++20 -std=c++23
WRK = g h i

###############################################################################

CXXFLAGS = -flto -march=native -O3 -Wall $(XTR) $(XT2) $(XT3)

###############################################################################

all: data

att: $(ATT)

clean:
	-rm -fv $(CSV) $(EXE) core* nohup.out *~

exe: $(EXE)

perf:
	@sudo bash -c "echo -1 > /proc/sys/kernel/perf_event_paranoid"

pre: perf
	@for i in $(COM) perf; do
		hash $$i || $$(echo "$$i not found!!!" && exit 1)
	done

###############################################################################

data:
	@make -s exp | tee $${HOSTNAME%%.*}-$$(date '+%Y%m%d-%H%M%S').$(OUT)

exp: pre
	@LANG=C
	case "$(OUT)" in
		csv)
			FORMAT='%s;%s;%s;%s;%s;%s;%s;%s\n'
			printf $$FORMAT compiler opt std program length work 'energy' 'time'
			;;
		txt)
			FORMAT='#%11s%12s%12s%12s%12s%12s%12s%12s\n'
			printf '#%108s\n' | tr ' ' '-'
			printf $$FORMAT compiler opt std program length work 'energy(J)' 'time(s)'
			printf '#%108s\n' | tr ' ' '-'
			FORMAT='%12s%12s%12s%12s%12s%12s%12s%12s\n'
			;;
		*)
			echo "Unknown format: '$(OUT)'"
			exit 1;
			;;
	esac
	for compiler in $(COM); do
		for opt in $(OPT); do
			for std in $(STD); do
				for exe in $(EXE); do
					make -Bs $$exe XTR=$$opt XT2=$$std
					for size in $(SIZ); do
						for work in $(WRK); do
							for run in $$(seq -w 0 $$(( $(REP) - 1 ))); do
								-perf stat -a -e power/energy-pkg/ 2> /tmp/stat.txt -- ./$$exe -s $$size -w $$work
								energy=$$(sed -n 's/\([[:digit:].,]*\) Joules .*/\1/p' /tmp/stat.txt)
								cpu=$$(grep seconds /tmp/stat.txt | tr -s ' ' | cut -d' ' -f2)
								printf $$FORMAT $$compiler $$opt $$std $$exe $$size $$work $$energy $$cpu
							done
						done
					done
				done
			done
		done
	done

quick:
	@make -s data OUT=txt REP=1 SIZ=1

###############################################################################
# pruebas
###############################################################################

xps: vector | pre
	@for (( i=0; i<6; ++i )); do
		-perf stat -a -e power/energy-cores/,power/energy-gpu/,power/energy-pkg/,power/energy-psys/,power/energy-ram/ -r 10 -- ./$< $$i > /dev/null
	done

###############################################################################

.PHONY: all att clean csv data exe exp perf pre quick xps

###############################################################################
