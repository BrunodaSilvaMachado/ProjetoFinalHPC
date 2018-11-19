#!/sbin/bash

##script run mainPll
./mainPll-03 >benchmark.txt 
gprof mainPll-03* gmon.out >profile.txt
