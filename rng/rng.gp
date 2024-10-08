#-----------------------------------------------------------------------------
# global variables
#-----------------------------------------------------------------------------

files=''

#-----------------------------------------------------------------------------
# global options
#-----------------------------------------------------------------------------

set encoding utf8
set key autotitle columnhead
set datafile columnheaders separator ';'
set pointsize 0.75
set size ratio 2/(1+sqrt(5))
set style boxplot medianlinewidth 2 pt 6 # nooutliers
set style fill solid 0.125
set terminal svg noenhanced size 1200,750 # 1600,1000
unset key

#-----------------------------------------------------------------------------
# main loop for data files
#-----------------------------------------------------------------------------

do for [file in files] {
    #---------------------------------------------------------------------
    # local options
    #---------------------------------------------------------------------
    set border 2
    set logscale y
    set xtics rotate by 45 right scale 0
    set ytics nomirror
    unset xlabel

    do for [quantity in 'pkg cpu'] {
        #---------------------------------------------------------------------
        # plot by column
        #---------------------------------------------------------------------
        if (quantity eq 'pkg') { set ylabel 'pkg (J)' }
        else                   { set ylabel 'cpu (s)' }
        #set output file.'-'.quantity.'.svg'
        #plot file u (1):(column(quantity)):(0.75):(column('engine')) w boxplot lc variable
   }

    #---------------------------------------------------------------------
    # plot by column corrected substracting mean consumption
    #---------------------------------------------------------------------
    set xlabel 'engine'
    set ylabel 'pkg (J)'

    #---------------------------------------------------------------------
    # plot by column corrected substracting mean generic consumption
    #---------------------------------------------------------------------
    if (strstrt(file, 'amd') != 0) { avg = 20.22 }
    else {
            if (strstrt(file, 'pccito') != 0) { avg = 15.72 }
            else { exit message 'no average consumption for '.file}
    }
    set output file.'-pkg-c.svg'
    plot file u (1):(column('pkg') - avg * column('cpu')):(0.75):(column('engine')) w boxplot lc variable

    #---------------------------------------------------------------------
    # plot by column corrected substracting mean measured consumption
    #---------------------------------------------------------------------
    set output file.'-pkg-a.svg'
    plot file u (1):(column('pkg') - column('avg')):(0.75):(column('engine')) w boxplot lc variable

}
