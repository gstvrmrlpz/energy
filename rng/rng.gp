#-----------------------------------------------------------------------------
# global variables
#-----------------------------------------------------------------------------

engines='knuth_b minstd_rand0 minstd_rand mt19937 mt19937_64 mt11213b rand ranlux24_base ranlux48_base ranlux24 ranlux48 xoroshiro128+ xoshiro256+'
files=''

#-----------------------------------------------------------------------------
# data files loop
#-----------------------------------------------------------------------------
do for [file in files] {
    #---------------------------------------------------------------------
    # global options
    #---------------------------------------------------------------------
    reset
    set border 2
    set datafile columnheaders separator ';'
    set encoding utf8
    set key autotitle columnhead
    set logscale y
    set pointsize 0.66
    #set size ratio 2/(1+sqrt(5))
    set style boxplot medianlinewidth 2 pointtype 6
    set style fill solid 0.125
    set terminal svg noenhanced #size 1200,750 # 1600,1000 2400,1500 3200,2000
    #set xlabel 'pseudo random number generator engine'
    set xtics rotate by 33 right scale 0
    set ytics nomirror

    stem=file[:strlen(file)-4]

    #---------------------------------------------------------------------
    # stats
    #---------------------------------------------------------------------
    do for [col in 'pkg cpu sleep diff'] {
        stats file using (column(col)) name col nooutput
    }

    do for[e in engines] {
        print '##########################################################'
        print '# '.e.' stats in '.file
        print '##########################################################'
        stats '<(head -n 1 '.file.'; grep ^'.e.'\; '.file.')' using (column('diff'))
    }

    #---------------------------------------------------------------------
    # pkg plot
    #---------------------------------------------------------------------
    set ylabel 'power/energy-pkg (J)' offset 3
    #set yrange [diff_min:diff_max]
    set output stem.'-pkg.svg'
    plot file using (1):(column('diff')):(0.75):(column('engine')) with boxplot linecolor variable notitle, diff_mean w l lc 0 lt 1 lw 2 t 'mean', diff_median w l lc 0 lt 0 lw 2 t 'median'

    #---------------------------------------------------------------------
    # cpu plot
    #---------------------------------------------------------------------
    set ylabel 'cpu time (s)'
    set output stem.'-cpu.svg'
    plot file using (1):(column('cpu')):(0.75):(column('engine')) with boxplot linecolor variable notitle, cpu_mean w l lc 0 lt 1 lw 2 t 'mean', cpu_median w l lc 0 lt 0 lw 2 t 'median'

    #---------------------------------------------------------------------
    # pkg plot with zoom
    #---------------------------------------------------------------------
    if (0) {
    unset key
    set output stem.'-zoom.svg'
    set multiplot

    set origin 0,0
    set size 1,1
    set ylabel 'power/energy-pkg (J)' offset 3
    set yrange [diff_min:diff_max]
    plot file using (1):(column('diff')):(0.75):(column('engine')) with boxplot linecolor variable

    set border 0
    unset xtics
    unset ylabel
    unset yrange
    unset ytics
    set yrange [0.5:1]

    set origin 0.05,0.35
    set size 0.33,0.90
    set xlabel 'xoroshiro128+'
    plot '<(head -n1 '.file.'; grep xoroshiro128 '.file.')' using (1):(column('diff')):(0.75) with boxplot linecolor 4

    set origin 0.15,0.35
    set size 0.33,0.90
    set xlabel 'xoshiro256+'
    plot '<(head -n1 '.file.'; grep xoshiro256 '.file.')' using (1):(column('diff')):(0.75) with boxplot linecolor 5

    unset multiplot
    }

    #---------------------------------------------------------------------
    # pkg plot for every engine
    #---------------------------------------------------------------------
    reset
    set datafile columnheaders separator ';'
    set encoding utf8
    set key autotitle columnhead
    set grid lt 1 lc rgb "grey" lw 1
    set pointsize 0.5
    set style boxplot medianlinewidth 2 pointtype 6
    set ticscale 0
    set ytic offset 0.75,0
    unset xtics
    set output stem.'-all.svg'
    set multiplot layout 1,13 margin 0.075,0.99,0.14,0.99 spacing 0.05,0
    do for[e in engines] {
        set xlabel e rotate by 33 right
        if (e eq 'knuth_b') { set ylabel 'power/energy-pkg (J)' offset 3.25 }
        else                { unset ylabel }
        plot '<(head -n 1 '.file.'; grep ^'.e.'\; '.file.')' using (1):(column('diff')):(0.75) with boxplot notitle
    }
    unset multiplot

}
