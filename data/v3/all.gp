#-----------------------------------------------------------------------------
# global variables
#-----------------------------------------------------------------------------

files='amd-20240726-215542.csv amd-20240730-133022.csv pccito-20240726-215614.csv pccito-20240730-133053.csv'
columns='compiler opt std program length work'

#-----------------------------------------------------------------------------
# global options
#-----------------------------------------------------------------------------

set key autotitle columnhead
set datafile columnheaders separator ';'
set size ratio 2/(1+sqrt(5))
set style data lines
set terminal svg enhanced size 1200,750 # 1600,1000 # 1200,750
unset key

#-----------------------------------------------------------------------------
# energy & time
#-----------------------------------------------------------------------------

do for [file in files] {
    #-------------------------------------------------------------------------
    # global & by column stats
    #-------------------------------------------------------------------------
    do for [quantity in 'energy time'] {
        stats file u (column(quantity)) name quantity nooutput
        do for [com in 'clang g'] {
            stats '<(head -n1 '.file.'; grep ^'.com.'++\; '.file.')' using (column(quantity)) name quantity.'_'.com nooutput
        }
        do for [opt in 'O2 O3'] {
            stats '<(head -n1 '.file.'; grep \;-'.opt.'\; '.file.')' using (column(quantity)) name quantity.'_'.opt nooutput
        }
        do for [pro in 'bitset bvector cvector string'] {
            stats '<(head -n1 '.file.'; grep \;'.pro.'\; '.file.')' using (column(quantity)) name quantity.'_'.pro nooutput
        }
        do for [work in 'g h i'] {
            stats '<(head -n1 '.file.'; grep \;'.work.'\; '.file.')' using (column(quantity)) name quantity.'_'.work nooutput
        }
    }

    #-------------------------------------------------------------------------
    # time vs energy point plot
    #-------------------------------------------------------------------------
    set border 15
    set palette defined (energy_min 'green', energy_median 'yellow',energy_max 'red')
    set xlabel 'time (s)'
    set ylabel 'energy (J)'

    #-------------------------------------------------------------------------
    # with init
    #-------------------------------------------------------------------------
    set output file.'.svg'
    plot file u (column('time')):(column('energy')):(column('energy') / energy_mean):(column('energy')) w p ps var pt 6 palette

    #-------------------------------------------------------------------------
    # without init
    #-------------------------------------------------------------------------
    set output file.'-noinit.svg'
    plot '<(head -n1 '.file.'; grep -v \;i\; '.file.')' u (column('time')):(column('energy')):(column('energy') / energy_mean):(column('energy')) w p ps var pt 6 palette

    #-------------------------------------------------------------------------
    # alpha
    #-------------------------------------------------------------------------
    set output file.'-alpha.svg'
    plot file u (column('time')):(column('energy')):(column('energy') / energy_mean) w p ps var pt 7 lc rgb '#f80000ff', file u (column('time')):(column('energy')):(column('energy') / energy_max) w p ps var pt 6 lc rgb '#eeff0000'

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
        set xtics scale 0
        set ytics nomirror
        unset xlabel

        #---------------------------------------------------------------------
        # multiplot by columns
        #---------------------------------------------------------------------
        set output file.'-'.quantity.'.svg'
        set multiplot layout 2,3
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
        set multiplot layout 2,3
        do for [col in columns] {
            plot '<(grep -v \;i\; '.file.')' u (1):(column(quantity)):(0.75):(column(col)) w boxplot lc variable lw 1.33
        }
        unset multiplot

        #---------------------------------------------------------------------
        # multiplot by columns and by work
        #---------------------------------------------------------------------
        do for [work in 'g h i'] {
            set output file.'-'.quantity.'-'.work.'.svg'
            set multiplot layout 2,3
            do for [col in columns] {
                plot '<(head -n1 '.file.'; grep \;'.work.'\; '.file.')' u (1):(column(quantity)):(0.75):(column(col)) w boxplot lc variable lw 1.33
            }
            unset multiplot
        }
    }
}

#-----------------------------------------------------------------------------
