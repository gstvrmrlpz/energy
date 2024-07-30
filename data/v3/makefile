.ONESHELL:

CSV = $(wildcard *.csv)
GP  = $(wildcard *.gp)
SVG = $(wildcard *.svg)

all: plot

auto:
	while ps $$! > /dev/null; do
		inotifywait -e modify -qr .
		make plot
	done

in: $(GP)
	for i in $^; do
		sed -i "s/^files='.*'$$/files='$(CSV)'/" $$i
		git commit -m "new data files" $$i
		git push
	done

clean:
	-rm -fv $(SVG) *~

plot: $(GP) | in
	for i in $^; do
		gnuplot $$i
	done

.PHONY: all auto clean in plot
