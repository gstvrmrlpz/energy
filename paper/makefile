.ONESHELL:

AUX = $(RNW:.Rnw=.aux)
BBL = $(RNW:.Rnw=.bbl)
BLG = $(RNW:.Rnw=.blg)
FDB = $(RNW:.Rnw=.fdb_latexmk)
FLS = $(RNW:.Rnw=.fls)
LOG = $(RNW:.Rnw=.log)
PDF = $(RNW:.Rnw=.pdf)
R   = $(RNW:.Rnw=.R)
RNW = $(wildcard *.Rnw)
SPL = $(RNW:.Rnw=.spl)
TEX = $(RNW:.Rnw=.tex)

all: $(PDF)

auto: $(TEX)
	latexmk -f -pdf -pvc -shell-escape -silent $< && killall -q latexmk &
	while ps $$! > /dev/null; do inotifywait -e modify -qr .; make -s $<; done

clean:
	-latexmk -C -f -shell-escape $(TEX)
	-rm -fv $(AUX) $(BBL) $(BLG) $(FDB) $(FLS) $(LOG) $(PDF) $(R) $(SPL) $(TEX) *~

install:
	sudo dnf -y install R{,-{dplyr,ggplot2,knitr}} texlive-soul
	sudo R -e 'install.packages("kableExtra", repos = "https://cran.rediris.es/"); quit();' -q

%.pdf: %.tex
	latexmk -pdf -shell-escape $<

%.R: %.Rnw
	Rscript -e "require(knitr); knitr::knit('$<',output='$@',tangle=TRUE);"

%.tex: %.Rnw %.R
	Rscript -e "require(knitr); knitr::knit('$<',output='$@');"

.PHONY: all auto clean pre
.PRECIOUS: $(R)

