###############################################################################
# makefile
###############################################################################

.ONESHELL:

###############################################################################

CSV = $(EXE:=.csv)
EXE = $(basename $(SRC))
LOG = $(EXE:=.log)
SRC = $(wildcard *.cc)
ENG = default_random_engine knuth_b minstd_rand0 minstd_rand \
	mt19937 mt19937_64 mt11213b rand \
	ranlux24_base ranlux48_base ranlux24 ranlux48 xoroshiro128p xoshiro256p
REP = 15

###############################################################################

CXXFLAGS = -flto -march=native -O3 -Wall

###############################################################################

all: csv

clean:
	-rm -fv $(CSV) $(EXE) $(LOG) core* *~

csv: $(CSV)

exe: $(EXE)

###############################################################################

%.csv: %
	@LANG=C
	FORMAT='%s,%s,%s\n'
	printf $$FORMAT engine energy cpu | tee $@
	for eng in $(ENG); do
		for (( rep = 0; rep < $(REP); ++rep )); do
			-perf stat -a -e power/energy-pkg/ 2> $*.log -- ./$< -e $$eng
			energy=$$(sed -n 's/\([[:digit:].,]*\) Joules .*/\1/p' $*.log)
			cpu=$$(grep seconds $*.log | tr -s ' ' | cut -d' ' -f2)
			printf $$FORMAT $$eng $$energy $$cpu | tee -a $@
			rm -f $*.log
		done
	done

###############################################################################

.PHONY: all clean csv exe
.PRECIOUS: $(CSV)

###############################################################################
