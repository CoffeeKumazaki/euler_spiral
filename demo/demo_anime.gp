set terminal gif animate delay 5 nooptimize size 640,480 font "Courier, 12"
set output 'demo.gif'
set arrow 1 from 0,0 to 2,2 lw 2
set arrow 2 from 15,8 to 15,6 lw 2
do for [i=1: 60] {
  plot [0:20][0:15]"../build/demo.dat" every :::::0 w l lc "red" ti "Biarc (initial state)",\
        "" every :i::i::i w l lc "blue" ti "estimating..."
}
do for [i=1: 60] {
  plot [0:20][0:15]"../build/demo.dat" every :::::0 w l lc "red" ti "Biarc (initial state)",\
        "" every :100::100::100 w l lc "blue" lw 2 ti "Estimation complete"
}
unset output
reset