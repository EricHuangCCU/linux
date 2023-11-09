#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/syscall.h> 
#include <assert.h>
#include <string.h>
#include <math.h>
#define loopCount 10000000000

int numCPU=-1;
char* exename;


volatile long partial_sum[100];
long double prec;

void thread(void *givenName) {

    int id = (intptr_t)givenName;
    long double sum = 0;
    long double i = 0;
    long double begin = (long double)1 / numCPU * id;
    long double end = (long double)1 / numCPU * (id+1);

    for(i=begin; i<=fmin(1-prec, end-prec); i+=prec){
        sum += sqrtl(1 - i*i);
    }

    partial_sum[id] = sum;

}   

int main(int argc, char **argv) {

    exename = argv[0];
    int precision;

    if(argc < 2){
        //printf("default precision : 5\n");
        precision = 5;
    }else{
        precision = atoi(argv[1]);
        printf("set precision to %d\n",precision);
    }

    prec = (long double)0.1;

    for(int i=0;i<=precision;i++){
        prec /= 10;
    }

    numCPU = sysconf( _SC_NPROCESSORS_ONLN );

    //numCPU = 3;

    printf("thread : %d\n", numCPU);

    pthread_t* tid = (pthread_t*)malloc(sizeof(pthread_t) * numCPU);

    for (long i=0; i< numCPU; i++)
        pthread_create(&tid[i], NULL, (void *) thread, (void*)i);

    for (int i=0; i< numCPU; i++)
	    pthread_join(tid[i], NULL);


    long double diff = (sqrtl((long double)1 - (1-prec) * (1-prec)));
    long double sum = 0;

    for(int i=0;i<numCPU;i++){
        sum += partial_sum[i];
    }

    long double lower = prec*(sum+diff-1)*4 ;
    long double upper = prec*sum*4 ;    

    printf("lower : %.32Lf\nupper : %.32Lf\n", lower, upper);
    printf("pi : %.32Lf\n", prec*4*(sum + (diff-1)/2));

}
