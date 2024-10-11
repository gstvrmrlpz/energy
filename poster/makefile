.ONESHELL:

AUX = $(TEX:.tex=.aux)
BBL = $(TEX:.tex=.bbl)
BLG = $(TEX:.tex=.blg)
FDB = $(TEX:.tex=.fdb_latexmk)
FLS = $(TEX:.tex=.fls)
LOG = $(TEX:.tex=.log)
PDF = $(TEX:.tex=.pdf)
SPL = $(TEX:.tex=.spl)
TEX = $(wildcard *.tex)

all: $(PDF)

auto: $(PDF)
	latexmk -f -pdf -pvc -shell-escape -silent $< && killall -q latexmk &
	while ps $$! > /dev/null; do inotifywait -e modify -qr .; make -s $<; done

clean:
	-latexmk -C -f -shell-escape $(TEX)
	-rm -fv $(AUX) $(BBL) $(BLG) $(FDB) $(FLS) $(LOG) $(PDF) $(SPL) *~

%.pdf: %.tex
	latexmk -f -pdf -shell-escape $<

.PHONY: all auto clean
