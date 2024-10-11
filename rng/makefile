###############################################################################
# makefile
###############################################################################

BASH := bash
.ONESHELL:

###############################################################################

CSV = $(wildcard *.csv)
EXE = $(basename $(SRC))
GP  = $(wildcard *.gp)
SRC = $(wildcard *.cc)
ENG = knuth_b minstd_rand0 minstd_rand mt19937 mt19937_64 mt11213b rand \
      ranlux24_base ranlux48_base ranlux24 ranlux48 xoroshiro128+ xoshiro256+
REP = 100
SVG = $(wildcard *.svg)

###############################################################################

CXXFLAGS = -flto -march=native -O3 -Wall

###############################################################################

all: svg

clean: stop
	-rm -fv $(EXE) $(LOG) $(SVG) core* *~

exe: $(EXE)

perf:
	if perf stat -a -e 'power/energy-pkg/' true |& grep 'not supported' &> /dev/null; then
		sudo sysctl kernel.perf_event_paranoid=-1
	fi

start:
    sudo cpupower frequency-set --governor performance
    echo 0 | sudo tee /sys/devices/system/cpu/cpufreq/boost

run: $(EXE) stop | perf start
	LANG=C
	file=$${HOSTNAME%%.*}-$$(date '+%s').csv
	FORMAT='%s;%s;%s;%s\n'
	printf $$FORMAT engine pkg cpu sleep | tee $$file
	for eng in $(ENG); do
		for (( rep = 0; rep < $(REP); ++rep )); do
			pkg=0.0
			sleep=0.0
			while [ $$(echo "$$pkg <= $$sleep" | bc) -eq 1 ]; do
				-perf stat -a -e power/energy-pkg/ 2> /tmp/$@ -- ./$< -e $$eng
				pkg=$$(sed -n 's/[[:space:]]*\([[:digit:].,]*\) Joules .*/\1/p' /tmp/$@)
				cpu=$$(grep seconds /tmp/$@ | tr -s ' ' | cut -d' ' -f2)
				-perf stat -a -e power/energy-pkg/ 2> /tmp/$@ -- sleep $$cpu
				sleep=$$(sed -n 's/[[:space:]]*\([[:digit:].,]*\) Joules .*/\1/p' /tmp/$@)
			done
			printf $$FORMAT $$eng $$pkg $$cpu $$sleep | tee -a $$file
		done
	done

schedutil:
    sudo cpupower frequency-set --governor schedutil
    echo 1 | sudo tee /sys/devices/system/cpu/cpufreq/boost

svg: $(GP) | $(CSV)
	gnuplot < <(sed "s/^files=.*/files='$(CSV)'/" $<)

###############################################################################

.PHONY: all clean exe perf run start stop svg
.PRECIOUS: $(CSV)

###############################################################################
