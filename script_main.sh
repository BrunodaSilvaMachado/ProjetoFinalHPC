# !sbin/bash

echo "construindo executaveis..."
mkdir "bin"
echo "No flags"
mkdir "bin/noflags"
gcc -g -pg main.c -o ./bin/noflags/main-0
gcc -g -pg -O1 main.c -o ./bin/noflags/main-01
gcc -g -pg -O2 main.c -o ./bin/noflags/main-02
gcc -g -pg -O3 main.c -o ./bin/noflags/main-03
echo "flags"
mkdir "bin/flags"
gcc -g -pg -march=corei7-avx mainOtm.c -o ./bin/flags/mainOtm-0
gcc -g -pg -O1 -march=corei7-avx mainOtm.c -o ./bin/flags/mainOtm-01
gcc -g -pg -O2 -march=corei7-avx mainOtm.c -o ./bin/flags/mainOtm-02
gcc -g -pg -O3 -march=corei7-avx mainOtm.c -o ./bin/flags/mainOtm-03
echo "OpenMP"
mkdir "bin/openMP"
gcc -g -pg -fopenmp -march=corei7-avx mainPll.c -o ./bin/openMP/mainPll-0
gcc -g -pg -fopenmp -O1 -march=corei7-avx mainPll.c -o ./bin/openMP/mainPll-01
gcc -g -pg -fopenmp -O2 -march=corei7-avx mainPll.c -o ./bin/openMP/mainPll-02
gcc -g -pg -fopenmp -O3 -march=corei7-avx mainPll.c -o ./bin/openMP/mainPll-03

echo "done."
## mexer aqui
echo "run now"
echo "inicio:" $(date) >> "tempo.txt"
exec ./main > info_main.txt
echo "fim:" $(date) >> "tempo.txt"
shutdown now
