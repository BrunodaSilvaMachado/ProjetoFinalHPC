#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define N 3000
#define T 1000

void imprime(int );

double matriz[N][N];
double mascara[N][N];

void init()
{
    int i,j;

    for(i = 0;i < N;i++)
    {
        for(j = 0;j < N;j++)
        {
            matriz[i][j] = 0;
            mascara[i][j] = 0;
        }
    }
}

void contorno()
{
    int i,j,lim,c;
    double r;
    c = N/2;
    lim = c/4;

    for(i = 0;i < N;i++)
	{
		for(j = 0;j < N;j++)
		{
			r = sqrt((c-i)*(c-i) + (c-j)*(c-j));
			
			if(r < lim*1.01 && r > lim*0.99)
			{
				mascara[i][j] = 1;
				matriz[i][j] = 1;
			}
		}
	}
}

double coef_term(double x,double y)
{
    return 1/6.0 * exp(-1.0/(pow(x,2)+pow(y,2)));
}

void relaxacao()
{
    int i,j;
	double s;

    for(i = 1;i < N - 1; i++)
    {
        for(j = 1; j < N - 1;j++)
        {
            if(mascara[i][j] == 0)
            {
				s = coef_term(i,j);
                matriz[i][j] = pow(s,2)*(matriz[i-1][j-1]+matriz[i-1][j+1]+matriz[i+1][j-1]+matriz[i+1][j+1])\
                 + s*(1-2*s)*(matriz[i][j-1] + matriz[i][j+1]+matriz[i-1][j]+matriz[i+1][j])\
				 +pow(1-2*s,2)*matriz[i][j];
            }
        }
    }
}

void periodo() //condicao periodica de contorno helicoidais
{
    int i;

    for(i = 0; i < N; i++)
    {
        matriz[i][N-1] = matriz[i+1][0];
    }
}

void imprime(int n)
{
    char nome[50];
	mkdir("data");
    sprintf(nome,"data//calor_%d.txt",n);
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

    for(i = 0; i < N;i++)
        sum += matriz[i][i];

    return sum;
}

int main()
{
    clock_t start_t= clock();
    int i = 0;
    double tr;

	init();
    contorno();

    do
    {
        tr = traco();
        relaxacao();
        periodo();
        if(i%50 == 0)
            imprime(i);
        i++;
    }while(fabs((traco()-tr)/tr) > 1e-5);

    printf("iter =%d\ndim =%d\ntime =%lf\n",i,N,(double)(clock() - start_t)/(double)CLOCKS_PER_SEC);
    return 0;
}

