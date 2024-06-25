all: perf
	-find -maxdepth 2 -mindepth 2 -name makefile -execdir make \;

clean:
	-rm -fv core.* *~
	-find -maxdepth 2 -mindepth 2 -name makefile -execdir make $@ \;

perf:
	sudo bash -c "echo -1 > /proc/sys/kernel/perf_event_paranoid"

.PHONY: all clean perf
