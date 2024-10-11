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
    #set grid
    set key autotitle columnhead
    set logscale y
    set pointsize 0.75
    set size ratio 2/(1+sqrt(5))
    set style boxplot medianlinewidth 2 pointtype 6
    set style fill solid 0.125
    set terminal svg noenhanced size 1200,750 # 1600,1000 2400,1500 3200,2000
    set xlabel 'pseudo random number generator engine'
    set xtics rotate by 33 right scale 0
    set ytics nomirror
    unset key
    stem=file[:strlen(file)-4]

    #---------------------------------------------------------------------
    # stats
    #---------------------------------------------------------------------
    do for [col in 'pkg cpu sleep'] {
        stats file using (column(col)) name col nooutput
    }
    stats file using (column('pkg') - column('sleep')) name 'diff' nooutput

    do for[e in engines] {
        print '##########################################################'
        print '# '.e.' stats in '.file
        print '##########################################################'
        stats '<(head -n 1 '.file.'; grep ^'.e.'\; '.file.')' using (column('pkg') - column('sleep'))
    }

    #---------------------------------------------------------------------
    # pkg plot
    #---------------------------------------------------------------------
    set ylabel 'pkg (J)'
    #set yrange [diff_min:diff_max]
    set output stem.'-pkg.svg'
    plot file using (1):(column('pkg') - column('sleep')):(0.75):(column('engine')) with boxplot linecolor variable

    #---------------------------------------------------------------------
    # cpu plot
    #---------------------------------------------------------------------
    set ylabel 'cpu (s)'
    set output stem.'-cpu.svg'
    plot file using (1):(column('cpu')):(0.75):(column('engine')) with boxplot linecolor variable

    #---------------------------------------------------------------------
    # pkg plot with zoom
    #---------------------------------------------------------------------
    if (0) {
    unset key
    set output stem.'-zoom.svg'
    set multiplot

    set origin 0,0
    set size 1,1
    set ylabel 'pkg (J)'
    set yrange [diff_min:diff_max]
    plot file using (1):(column('pkg') - column('sleep')):(0.75):(column('engine')) with boxplot linecolor variable

    set border 0
    unset xtics
    unset ylabel
    unset yrange
    unset ytics
    set yrange [0.5:1]

    set origin 0.05,0.35
    set size 0.33,0.90
    set xlabel 'xoroshiro128+'
    plot '<(head -n1 '.file.'; grep xoroshiro128 '.file.')' using (1):(column('pkg') - column('sleep')):(0.75) with boxplot linecolor 4

    set origin 0.15,0.35
    set size 0.33,0.90
    set xlabel 'xoshiro256+'
    plot '<(head -n1 '.file.'; grep xoshiro256 '.file.')' using (1):(column('pkg') - column('sleep')):(0.75) with boxplot linecolor 5

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
    set style boxplot medianlinewidth 2 pointtype 6
    set style fill solid 0.125
    set ticscale 0
    set ytic offset 0.5,0
    unset key
    unset xtics
    set output stem.'-all.svg'
    set multiplot layout 1,13 margin 0.033,0.99,0.05,0.99 spacing 0.033,0
    do for[e in engines] {
        set xlabel e
        if (e eq 'knuth_b') { set ylabel 'power/energy-pkg (J)' offset 2.5}
        else                { unset ylabel }
        plot '<(head -n 1 '.file.'; grep ^'.e.'\; '.file.')' using (1):(column('pkg') - column('sleep')):(0.75) with boxplot
    }
    unset multiplot

}
