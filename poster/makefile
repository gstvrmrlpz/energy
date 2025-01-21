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

stat: stats.txt
	@LANG=C
	readarray -t a < <(grep -E '.csv|Mean:|Std Dev:' $< | grep -v Err)
	for ((i=0; i<$${#a[@]}; i+=6)) do
		title=($${a[i]})
		cpu_mean=($${a[i+1]})
		cm=$$(printf "%0.4f" $${cpu_mean[1]})
		cpu_stddev=($${a[i+2]})
		cs=$$(printf "%0.4f" $${cpu_stddev[2]})
		diff_mean=($${a[i+4]})
		dm=$$(printf "%0.4f" $${diff_mean[1]})
		diff_stddev=($${a[i+5]})
		ds=$$(printf "%0.4f" $${diff_stddev[2]})
		#echo "$${title[1]} & $${dm}\$$\\pm\$$$${ds} & $${cm}\$$\\pm\$$$${cs} & $$(bc <<< "scale=2; ($${diff_mean[1]}*10.0)/1") & $$(bc <<< "scale=2; ($${cpu_mean[1]}*10.0)/1") \\\\" | sed 's/_/\\_/g'
		echo "$${title[1]} & $${dm}\$$\\pm\$$$${ds} & $${cm}\$$\\pm\$$$${cs} \\\\" | sed 's/_/\\_/g'
	done

%.pdf: %.tex
	latexmk -f -pdf -shell-escape $<

.PHONY: all auto clean
