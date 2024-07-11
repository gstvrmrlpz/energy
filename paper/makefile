PDF = $(RNW:.Rnw=.pdf)
R   = $(RNW:.Rnw=.R)
RNW = $(wildcard *.Rnw)
TEX = $(RNW:.Rnw=.tex)

all: $(PDF)

auto: all
	latexmk -f -pdf -pvc -shell-escape

clean:
	-latexmk -C -shell-escape $(TEX)
	-rm -fv $(R) $(TEX) *~

%.pdf: %.tex
	latexmk -pdf -shell-escape $(TEX)

%.R: %.Rnw
	Rscript -e "require(knitr); knitr::knit('$(RNW)',output='$(R)',tangle=TRUE);"

%.tex: %.Rnw %.R
	Rscript -e "require(knitr); knitr::knit('$(RNW)',output='$(TEX)');"

.PHONY: all clean
