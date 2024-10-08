#-----------------------------------------------------------------------------
# global variables
#-----------------------------------------------------------------------------

files=''

#-----------------------------------------------------------------------------
# global options
#-----------------------------------------------------------------------------

set border 2
set datafile columnheaders separator ';'
set encoding utf8
set key autotitle columnhead
set logscale y
set pointsize 0.75
set size ratio 2/(1+sqrt(5))
set style boxplot medianlinewidth 2 pointtype 6 # nooutliers
set style fill solid 0.125
set terminal svg noenhanced size 1200,750 # 1600,1000
set xlabel 'pseudo random number generator engine'
set xtics rotate by 45 right scale 0
set ylabel 'pkg (J)'
set ytics nomirror
unset key

#-----------------------------------------------------------------------------
# main loop for data files
#-----------------------------------------------------------------------------

do for [file in files] {
    #---------------------------------------------------------------------
    # stats
    #---------------------------------------------------------------------
    do for [col in 'pkg cpu avg'] {
        stats file using (column(col)) name col nooutput
    }

    #---------------------------------------------------------------------
    # plot by column corrected substracting mean measured consumption
    #---------------------------------------------------------------------
    set output file.'.svg'
    #set yrange [0:pkg_max / 2]
    plot file using (1):(column('pkg') - column('avg') / 1.5):(0.75):(column('engine')) with boxplot linecolor variable
}
