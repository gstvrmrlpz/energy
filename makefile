.ONESHELL:

all:
	-find -maxdepth 2 -mindepth 2 -name makefile -execdir make \;

clean:
	-rm -fv core.* *~
	-find -maxdepth 2 -mindepth 2 -name makefile -execdir make $@ \;

realclean: clean
	-pgrep make | xargs -n 1 -r pstree -p | grep -o '[[:digit:]]\+' | xargs kill

.PHONY: all clean realclean
