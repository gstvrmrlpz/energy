#-----------------------------------------------------------------------------
# global variables
#-----------------------------------------------------------------------------

files='amd-20240726-134906.csv pccito-20240726-143415.csv'
columns='compiler opt program work'

#-----------------------------------------------------------------------------
# global options
#-----------------------------------------------------------------------------

set key autotitle columnhead
set datafile columnheaders separator ';'
set size ratio 2/(1+sqrt(5))
set style data lines
set terminal svg enhanced
set xlabel 'time (s)'
set ylabel 'energy (J)'

#-----------------------------------------------------------------------------
# basic plot
#-----------------------------------------------------------------------------

do for [file in files] {
    set output file.'.svg'
    plot file u (column('time')):(column('energy')) notitle w p pt 6
}

#-----------------------------------------------------------------------------
# energy & time
#-----------------------------------------------------------------------------

set border 2
set xtics scale 0
set ytics nomirror
unset key
unset xlabel

do for [file in files] {
    do for [quantity in 'energy time'] {
        #---------------------------------------------------------------------
        # local options
        #---------------------------------------------------------------------
        if (quantity eq 'energy') { set ylabel 'energy (J)' }
        else                      { set ylabel 'time (s)'   }

        #---------------------------------------------------------------------
        # multiplots
        #---------------------------------------------------------------------
        set terminal svg enhanced size 1600,1000

        #---------------------------------------------------------------------
        # by columns
        #---------------------------------------------------------------------
        set output file.'-'.quantity.'.svg'
        set multiplot layout 2,2
        do for [col in columns] {
            plot file u (1):(column(quantity)):(0.75):(column(col)) w boxplot lc variable
        }
        unset multiplot

        #---------------------------------------------------------------------
        # by columns + no init
        #---------------------------------------------------------------------
        set output file.'-'.quantity.'-noinit.svg'
        set multiplot layout 2,2
        do for [col in columns] {
            plot '<(grep -v \;i\; '.file.')' u (1):(column(quantity)):(0.75):(column(col)) w boxplot lc variable
        }
        unset multiplot

        #---------------------------------------------------------------------
        # by columns and by work
        #---------------------------------------------------------------------
        do for [work in 'g h i'] {
            set output file.'-'.quantity.'-'.work.'.svg'
            set multiplot layout 2,2
            do for [col in columns] {
                plot '<(head -n1 '.file.'; grep \;'.work.'\; '.file.')' u (1):(column(quantity)):(0.75):(column(col)) w boxplot lc variable
            }
            unset multiplot
        }
    }
}

#-----------------------------------------------------------------------------
