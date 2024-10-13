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

kk: stats.txt
	IFS=$$'\n' read -r -d '' -a my_array < <( grep -E '.csv|Mean:|Std Dev:' $< | grep -v Err && printf '\0' )
	for i in $${my_array[@]}; do
		echo $$i
	done

all: $(PDF)

auto: $(PDF)
	latexmk -f -pdf -pvc -shell-escape -silent $< && killall -q latexmk &
	while ps $$! > /dev/null; do inotifywait -e modify -qr .; make -s $<; done

clean:
	-latexmk -C -f -shell-escape $(TEX)
	-rm -fv $(AUX) $(BBL) $(BLG) $(FDB) $(FLS) $(LOG) $(PDF) $(SPL) *~

stat: stats.txt
	@readarray -t a < <(grep -E '.csv|Mean:|Std Dev:' $< | grep -v Err)
	for ((i=0; i<$${#a[@]}; i+=6)) do
		title=($${a[i]})
		cpu_mean=($${a[i+1]})
		cpu_stddev=($${a[i+2]})
		diff_mean=($${a[i+4]})
		diff_stddev=($${a[i+5]})
		echo "$${title[1]} & $${diff_mean[1]}\$$\\pm\$$$${diff_stddev[2]} & $${cpu_mean[1]}\$$\\pm\$$$${cpu_stddev[2]} \\\\" | sed 's/_/\\_/g'
	done

%.pdf: %.tex
	latexmk -f -pdf -shell-escape $<

.PHONY: all auto clean
