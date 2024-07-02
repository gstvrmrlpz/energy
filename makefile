all: perf
	-find -maxdepth 2 -mindepth 2 -name makefile -execdir make \;

clean:
	-rm -fv core.* *~
	-find -maxdepth 2 -mindepth 2 -name makefile -execdir make $@ \;

.PHONY: all clean
