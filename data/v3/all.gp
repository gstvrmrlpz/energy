#-----------------------------------------------------------------------------
# global variables
#-----------------------------------------------------------------------------

files='amd-20240726-215542.csv pccito-20240726-215614.csv'
columns='compiler opt program work'

#-----------------------------------------------------------------------------
# global options
#-----------------------------------------------------------------------------

set key autotitle columnhead
set datafile columnheaders separator ';'
set size ratio 2/(1+sqrt(5))
set style data lines
set terminal svg enhanced
unset key

#-----------------------------------------------------------------------------
# energy & time
#-----------------------------------------------------------------------------

do for [file in files] {
    #-------------------------------------------------------------------------
    # global stats
    #-------------------------------------------------------------------------
    do for [quantity in 'energy time'] {
        stats file u (column(quantity)) name quantity nooutput
    }

    #-------------------------------------------------------------------------
    # time vs energy point plot
    #-------------------------------------------------------------------------
    set border 15
    set output file.'.svg'
    set palette defined (energy_min 'green', energy_median 'yellow',energy_max 'red')
    set xlabel 'time (s)'
    set ylabel 'energy (J)'
    plot file u (column('time')):(column('energy')):(column('energy')) notitle w p pt 7 palette

    do for [quantity in 'energy time'] {
        #---------------------------------------------------------------------
        # local options
        #---------------------------------------------------------------------

        if (quantity eq 'energy') { set ylabel 'energy (J)' }
        else                      { set ylabel 'time (s)'   }

        #---------------------------------------------------------------------
        # multiplots
        #---------------------------------------------------------------------
        set border 2
        #set terminal svg enhanced size 1600,1000
        set terminal svg enhanced size 1200,750
        set xtics scale 0
        set ytics nomirror
        unset xlabel

        #---------------------------------------------------------------------
        # multiplot by columns
        #---------------------------------------------------------------------
        set output file.'-'.quantity.'.svg'
        set multiplot layout 2,2
        do for [col in columns] {
            #set label 1 'mean' at time_mean,energy_mean
            plot file u (1):(column(quantity)):(0.75):(column(col)) w boxplot lc variable lw 1.33
        }
        #unset label
        unset multiplot

        #---------------------------------------------------------------------
        # multiplot by columns + no init
        #---------------------------------------------------------------------
        set output file.'-'.quantity.'-noinit.svg'
        set multiplot layout 2,2
        do for [col in columns] {
            plot '<(grep -v \;i\; '.file.')' u (1):(column(quantity)):(0.75):(column(col)) w boxplot lc variable
        }
        unset multiplot

        #---------------------------------------------------------------------
        # multiplot by columns and by work
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
