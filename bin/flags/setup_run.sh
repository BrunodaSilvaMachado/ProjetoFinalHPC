#!/sbin/bash

##script run mainOtm

./mainOtm-03 >benchmark.txt 
gprof mainOtm-03* gmon.out >profile.txt
