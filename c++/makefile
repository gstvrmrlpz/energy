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
	@make -s exp | tee $${HOSTNAME%%.*}-$$(date '+%Y%m%d-%H%M%S').$(OUT)

exp: pre
	@LANG=C
	case "$(OUT)" in
		csv)
			FORMAT='%s;%s;%s;%s;%s;%s;%s;%s\n'
			printf $$FORMAT compiler opt program run length work 'energy' 'time'
			;;
		txt)
			FORMAT='#%13s%14s%14s%14s%14s%14s%14s%14s\n'
			printf '#%111s\n' | tr ' ' '-'
			printf $$FORMAT compiler opt program run length work 'energy(J)' 'time(s)'
			printf '#%111s\n' | tr ' ' '-'
			FORMAT='%14s%14s%14s%14s%14s%14s%14s%14s\n'
			;;
		*)
			echo "Unknown file extension: '$(OUT)'"
			exit 1;
			;;
	esac
	for compiler in $(COM); do
		for level in $(LEV); do
			for exe in $(EXE); do
				make -Bs $$exe CXX=$$compiler OPT=$$level
				for size in $(SIZ); do
					for work in $(WRK); do
						for run in $$(seq -w 0 $$(( $(REP) - 1 ))); do
							-perf stat -a -e power/energy-pkg/ 2> /tmp/stat.txt -- ./$$exe -s $$size -w $$work
							energy=$$(sed -n 's/\([[:digit:].,]*\) Joules .*/\1/p' /tmp/stat.txt)
							cpu=$$(grep seconds /tmp/stat.txt | tr -s ' ' | cut -d' ' -f2)
							printf $$FORMAT $$compiler $$level $$exe $$run $$size $$work $$energy $$cpu
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
