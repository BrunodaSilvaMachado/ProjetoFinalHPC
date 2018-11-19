set terminal pngcairo
#init
N = 150
set xrange[0:N]
set yrange[0:N]

set map
#set dir
system("mkdir animation")

do for[i=0:1000:50]{
	set output sprintf("animation/calor_%d.png",i);plot sprintf("data/calor_%d.txt",i) matrix with image;unset output
}

unset terminal
reset
exit()
