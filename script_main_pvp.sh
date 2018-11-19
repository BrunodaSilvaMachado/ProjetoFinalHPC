#!/sbin/bash

CC=gcc
FLAGS="-march=corei7-avx"
DIRFLAGS="./bin/flags"
DIROPENMP="./bin/openMP"
DIRMAIN="../../"

echo "construindo executaveis..."

echo "flags"
mkdir $DIRFLAGS
$CC -g -pg -O3 $FLAGS mainOtm.c -o $DIRFLAGS/mainOtm-03
echo "OpenMP"
mkdir $DIROPENMP
$CC -g -pg -fopenmp -O3 $FLAGS mainPll.c -o $DIROPENMP/mainPll-03

echo "gerando os setups"

echo -e "#!/sbin/bash\n\n\
##script run mainOtm\n\n\
./mainOtm-03 >benchmark.txt \n\
gprof mainOtm-03* gmon.out >profile.txt" >$DIRFLAGS/setup_run.sh

echo -e "#!/sbin/bash\n\n\
##script run mainPll\n\
./mainPll-03 >benchmark.txt \n\
gprof mainPll-03* gmon.out >profile.txt" >$DIROPENMP/setup_run.sh

echo "run all setup"

cd $DIRFLAGS
sh setup_run.sh 

sed 's/MAXIMO/'$(grep 'iter =' benchmark.txt | cut -d = -f 2)'/g' $DIRMAIN/animation.gp | sed 's/NUMERO/'$(grep 'dim =' benchmark.txt | cut -d = -f 2)'/g' > animation.gp

gnuplot -e "load 'animation.gp'"

cd $DIRMAIN
cd $DIROPENMP
sh setup_run.sh 

sed 's/MAXIMO/'$(grep 'iter =' benchmark.txt | cut -d = -f 2)'/g' $DIRMAIN/animation.gp | sed 's/NUMERO/'$(grep 'dim =' benchmark.txt | cut -d = -f 2)'/g' > animation.gp

gnuplot -e "load 'animation.gp'"
cd $DIRMAIN

echo "done"
