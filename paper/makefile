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
	latexmk -f -pdf -pvc -shell-escape $<

clean:
	-latexmk -C -f -shell-escape $(TEX)
	-rm -fv $(AUX) $(BBL) $(BLG) $(FDB) $(FLS) $(LOG) $(PDF) $(R) $(SPL) $(TEX) *~

%.pdf: %.tex
	latexmk -pdf -shell-escape $(TEX)

%.R: %.Rnw
	Rscript -e "require(knitr); knitr::knit('$(RNW)',output='$(R)',tangle=TRUE);"

%.tex: %.Rnw %.R
	Rscript -e "require(knitr); knitr::knit('$(RNW)',output='$(TEX)');"

.PHONY: all clean