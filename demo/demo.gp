set terminal png size 640,480 font "Courier, 12"
set output 'demo.png'
set arrow 1 from 0,0 to sqrt(2),0 lw 2
set arrow 2 from 15,6 to 15,6+2 lw 2

set arrow 3 from 30,30 to 30+sqrt(2),30+sqrt(2) lw 2
set arrow 4 from 10,35 to 10-sqrt(2),35+sqrt(2) lw 2

set arrow 5 from 10,10 to 12,10 lw 2
set arrow 6 from 10,38 to 12,38 lw 2

set arrow 7 from 20,5 to 20,5-2 lw 2
set arrow 8 from 30,15 to 30-sqrt(2),15-sqrt(2) lw 2

plot [0:35][-1:40]"demo.dat" w l ti ""
unset output
reset