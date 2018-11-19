#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>
#include <time.h>

#define N 150
#define T 1000

#define DEBUG 0

double matriz[N][N];
double mascara[N][N];

#if DEBUG
    void imprime(int );
#endif // DEBUG

void contorno()
{
    int i,j,lim,c;
    double r;
    c = N*0.5;
    lim = c*0.25;

	#pragma omp parallel shared(c,lim,matriz,mascara)
    {
        #pragma omp for schedule(dynamic,1000) private(i,j,r) nowait
		for(i = 0;i < N;i++)
        {
            for(j = 0;j < N;j++)
            {
                r = sqrt((c-i)*(c-i) + (c-j)*(c-j));

                mascara[i][j] = matriz[i][j] = (r < lim*1.1 && r > lim*0.9)?1:0;
            }
        }
	}
#if DEBUG
    imprime(0);
    exit(1);
#endif // DEBUG
}

double coef_term(double x, double y)
{
    return exp(-1.0/(x*x + y*y)) * 0.16667;
}

void relaxacao()
{
    int i,j;
    double s;

	#pragma omp parallel shared(i,s,matriz,mascara)
    {
		for(i = 1;i < N - 1; i++)
		{
		    #pragma omp for schedule(dynamic,1000) private(j) nowait
			for(j = 1; j < N/2-1;j++)
			{
				if(mascara[i][j] == 0)
				{
					s = coef_term(i,j);
					matriz[i][j] = s*s*(matriz[i-1][j-1]+matriz[i-1][j+1]+matriz[i+1][j-1]+matriz[i+1][j+1])\
					 + s*(1-2*s)*(matriz[i][j-1] + matriz[i][j+1]+matriz[i-1][j]+matriz[i+1][j])\
					 + (1-2*s)*(1-2*s)*matriz[i][j];
				}
			}

			#pragma omp for schedule(dynamic,1000) private(j) nowait
			for(j = N/2 - 1; j < N - 1;j++)
			{
				if(mascara[i][j] == 0)
				{
					s = coef_term(i,j);
					matriz[i][j] = s*s*(matriz[i-1][j-1]+matriz[i-1][j+1]+matriz[i+1][j-1]+matriz[i+1][j+1])\
					 + s*(1-2*s)*(matriz[i][j-1] + matriz[i][j+1]+matriz[i-1][j]+matriz[i+1][j])\
					 + (1-2*s)*(1-2*s)*matriz[i][j];
				}
			}
		}
	}
}

void periodo() //condicao periodica de contorno helicoidais
{
    int i;

	#pragma omp parallel private(i)
    {
        #pragma omp for schedule(dynamic,100) nowait
		for(i = 0; i < N - 1; i++)
		{
			matriz[i][N-1] = matriz[i+1][0];
		}
	}
}
void imprime(int n)
{
    char nome[30];

    mkdir("./data");
    sprintf(nome,"./data/calor_%d.txt",n);
    FILE *arq = fopen(nome,"w+");
    int i,j;

    for(i = 0;i < N; i++)
    {
        for(j = 0; j < N;j++)
        {
            fprintf(arq,"%lf\t",matriz[i][j]);
        }
        fprintf(arq,"\n");
    }

    fclose(arq);
}

double traco()
{
    int i;
    double sum = 0;

    #pragma omp parallel shared(sum,matriz)
    {
        #pragma omp for schedule(dynamic,100) private(i) reduction(+:sum)
        for(i = 0; i < N;i++)
            sum = sum + matriz[i][i];

    }

    return sum;
}

int main()
{
    clock_t start_t= clock();
    int i = 0;
    double tr;

	omp_set_num_threads(4);

    contorno();

	#pragma omp parallel shared(i)
    {
        //#pragma omp for schedule(dynamic,100)
		do
        {
            tr = traco();
            relaxacao();
            periodo();
            if(i%50 == 0)
                imprime(i);
            i++;
        }while(fabs((traco()-tr)/tr) > 1e-5);
	}
	printf("iter =%d\ndim =%d\ntime =%lf\n",i,N,(double)(clock() - start_t)/(double)CLOCKS_PER_SEC);

    return 0;
}

