#-----------------------------------------------------------------------------
# global variables
#-----------------------------------------------------------------------------

columns='engine'
engines='default_random_engine knuth_b minstd_rand minstd_rand0 mt11213b mt19937 mt19937_64 rand_t ranlux24 ranlux24_base ranlux48 ranlux48_base xoroshiro128p xoshiro256p'
files=
quantitys='pkg cpu'

#-----------------------------------------------------------------------------
# global options
#-----------------------------------------------------------------------------

set encoding utf8
set key autotitle columnhead
set datafile columnheaders separator ','
set size ratio 2/(1+sqrt(5))
set style data lines
set terminal svg enhanced size 1200,750 # 1600,1000
unset key

#-----------------------------------------------------------------------------
# main loop for data files
#-----------------------------------------------------------------------------

do for [file in files] {
    do for [quantity in quantitys] {
        #---------------------------------------------------------------------
        # local options
        #---------------------------------------------------------------------
        if (quantity eq 'pkg') { set ylabel 'pkg (J)' }
        else                   { set ylabel 'cpu (s)' }

        #---------------------------------------------------------------------
        # multiplots
        #---------------------------------------------------------------------
        set border 2
        set xtics scale 0
        set ytics nomirror
        set logscale y
        unset xlabel

        #---------------------------------------------------------------------
        # multiplot by columns
        #---------------------------------------------------------------------
        set output file.'-'.quantity.'.svg'
        set multiplot layout 1,1
        do for [col in columns] {
            plot file u (1):(column(quantity)):(0.75):(column(col)) w boxplot lc variable lw 1.33
        }
        unset multiplot
    }
}
