#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#define numeroThreads 16         
#define contadorFinal 56000055 

void *calcular(void *threadid){
   long longTid;
   longTid = (long)threadid;
   int tid = (int)longTid;  // obtener el id
   float *in_count = (float *)malloc(sizeof(float));
   *in_count=0;
   
   float iteracionesTot= contadorFinal/numeroThreads;   
   int contador=0;
   
   for(contador=0;contador<iteracionesTot;contador++){
        float x = rand()/ (float) RAND_MAX; 
        float y = rand()/ (float) RAND_MAX; 
        if(sqrt((x*x) + (y*y))<1){
            *in_count+=1;         //verificar si el valor generado esta dentro
        }
   }
   
   if(tid==0){
      float _r = contadorFinal%numeroThreads;
      for(contador=0;contador<_r;contador++){
            float x = rand()/ (float) RAND_MAX; //;randNumGen();
            float y = rand()/ (float) RAND_MAX; 
            if(sqrt((x*x) + (y*y)) <1){
                *in_count+=1;         //check if the generated value is inside a unit circle
        }
    }
   }
   pthread_exit((void *)in_count);     //return the in count
}

int main(int argc, char *argv[]){
    pthread_t threads[numeroThreads];
    int rc;
    long t;
    void *estado;
    float tot_in=0;
   
    for(t=0;t<numeroThreads;t++){
        rc = pthread_create(&threads[t], NULL, calcular, (void *)t);
    }
    for(t=0;t<numeroThreads;t++){        
        pthread_join(threads[t], &estado);
        tot_in+=*(float*)estado;          // guarda el total de lo contado 
     }
    printf("Value for Pi is %f \n",1, 4*(tot_in/contadorFinal));
    pthread_exit(NULL);
}