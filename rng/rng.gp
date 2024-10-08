#-----------------------------------------------------------------------------
# global variables
#-----------------------------------------------------------------------------

engines='knuth_b minstd_rand0 minstd_rand mt19937 mt19937_64 mt11213b rand ranlux24_base ranlux48_base ranlux24 ranlux48 xoroshiro128+ xoshiro256+'
files=''

#-----------------------------------------------------------------------------
# data files loop
#-----------------------------------------------------------------------------
do for [file in files] {
    #-----------------------------------------------------------------------------
    # global options
    #-----------------------------------------------------------------------------
    reset
    set border 2
    set datafile columnheaders separator ';'
    set encoding utf8
    set key autotitle columnhead
    set logscale y
    set pointsize 0.75
    set size ratio 2/(1+sqrt(5))
    set style boxplot medianlinewidth 2 pointtype 6
    set style fill solid 0.125
    set terminal svg noenhanced size 1200,750 # 1600,1000
    set xlabel 'pseudo random number generator engine'
    set xtics rotate by 33 right scale 0
    set ytics nomirror
    unset key

    if (strstrt(file, 'amd') != 0) { avg = 20.22 }
    else {
        if (strstrt(file, 'pccito') != 0) { avg = 15.72 }
        else { exit message 'no average consumption for '.file }
    }

    #---------------------------------------------------------------------
    # stats
    #---------------------------------------------------------------------
    do for [col in 'pkg cpu avg'] {
        stats file using (column(col)) name col nooutput
    }

    #---------------------------------------------------------------------
    # plot pkg column minus sleep consumption
    #---------------------------------------------------------------------
    set ylabel 'pkg (J)'
    set output file.'-pkg.svg'
    plot file using (1):(column('pkg') - column('avg')):(0.75):(column('engine')) with boxplot linecolor variable

    #---------------------------------------------------------------------
    # plot pkg column minus average sleep consumption
    #---------------------------------------------------------------------
    set output file.'-avg.svg'
    plot file using (1):(column('pkg') - avg * column('cpu')):(0.75):(column('engine')) with boxplot linecolor variable

    #---------------------------------------------------------------------
    # plot cpu column
    #---------------------------------------------------------------------
    set ylabel '(cpu (s))'
    set output file.'-cpu.svg'
    plot file using (1):(column('cpu')):(0.75):(column('engine')) with boxplot linecolor variable

    #---------------------------------------------------------------------
    # plot every engine
    #---------------------------------------------------------------------
    reset
    set datafile columnheaders separator ';'
    set encoding utf8
    set key autotitle columnhead
    set grid lt 1 lc rgb "grey" lw 1
    #set size ratio 20
    set style boxplot medianlinewidth 2 pointtype 6
    set style fill solid 0.125
    set ticscale 0
    set ytic offset 0.5,0
    unset key
    unset xtics
    unset ylabel
    set output file.'-all.svg'
    set multiplot layout 1,13 margin 0.033,0.99,0.05,0.99 spacing 0.033,0
    do for[e in engines] {
        set xlabel e
        plot '<(head -n 1 '.file.'; grep '.e.' '.file.')' using (1):(column('pkg') - avg * column('cpu')):(0.5) with boxplot
    }
    unset multiplot
}
