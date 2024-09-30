#-----------------------------------------------------------------------------
# global variables
#-----------------------------------------------------------------------------

files=

#-----------------------------------------------------------------------------
# global options
#-----------------------------------------------------------------------------

set encoding utf8
set key autotitle columnhead
set datafile columnheaders separator ','
set size ratio 2/(1+sqrt(5))
set style data lines
set terminal svg noenhanced size 1200,750 # 1600,1000
#set terminal epscairo enhanced
unset key

#-----------------------------------------------------------------------------
# main loop for data files
#-----------------------------------------------------------------------------

do for [file in files] {
    do for [quantity in 'pkg cpu'] {

        #---------------------------------------------------------------------
        # local options
        #---------------------------------------------------------------------
        set border 2
        set logscale y
        set xtics rotate by 45 right scale 0
        set ytics nomirror
        unset xlabel

        if (quantity eq 'pkg') { set ylabel 'pkg (J)' }
        else                   { set ylabel 'cpu (s)' }

        #---------------------------------------------------------------------
        # plot by column
        #---------------------------------------------------------------------
        set output file.'-'.quantity.'.svg'
        plot file u (1):(column(quantity)):(0.75):(column('engine')) w boxplot lc variable lw 1.33
    }
}
