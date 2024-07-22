all:
	-find -maxdepth 2 -mindepth 2 -name makefile -execdir make \;

clean:
	-rm -fv core.* *~
	-find -maxdepth 2 -mindepth 2 -name makefile -execdir make $@ \;
	-for i in $$(pgrep make); do
		if [ "$$i" != "$$$$" ]; then
				pstree -p $$i | grep -o '[0-9]\+' | xargs kill -KILL
		fi
	done

.PHONY: all clean
