set terminal gif animate delay 10
#init
#N = 150
#M = 1000

N = 150
M = 5830
set xrange[0:N]
set yrange[0:N]

set map
#set dir
system("mkdir animation")

#make gif
set output 'animation/calor_ani.gif'

do for[i=0:M:50]{
	plot sprintf("data/calor_%d.txt",i) matrix with image
}

unset output
unset terminal
reset
exit()
