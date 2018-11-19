#!/sbin/bash

CC=gcc
CFLAGS="-march=native -mtune=native"
DIRNOFLAGS="./bin/noflags"
DIRFLAGS="./bin/flags"
DIROPENMP="./bin/openMP"
DIRMAIN="../../"

echo "construindo executaveis..."

echo "noflags"
mkdir $DIRNOFLAGS

for i in 0 1 2 3
do
	$CC -g -pg -O$i $CFLAGS mainOtm.c -o $DIRNOFLAGS/mainOtm-0$i
done

echo "flags"
mkdir $DIRFLAGS

for i in 0 1 2 3
do
	$CC -g -pg -O$i $CFLAGS mainOtm.c -o $DIRFLAGS/mainOtm-0$i
done

echo "OpenMP"
mkdir $DIROPENMP
for i in 0 1 2 3
do
$CC -g -pg -fopenmp -O$i $CFLAGS mainPll.c -o $DIROPENMP/mainPll-0$i
done

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
