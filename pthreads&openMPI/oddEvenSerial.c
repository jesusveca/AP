#include <stdio.h>
#include <stdlib.h>
void oddEvenSort(int a[],int n){
	int phase,i,temp;
	for(phase=0;phase<n;phase++){
		if(phase%2 == 0){
			for(i=1;i<n;i+=2){
				if(a[i-1]>a[i]){
					temp = a[i];
					a[i] = a[i-1];
					a[i-1]=temp;
				}
			}
		}
		else{
			for(i=1;i<n-1;i+=2){
				if(a[i]>a[i+1]){
					temp=a[i];
					a[i]=a[i+1];
					a[i+1]=temp;
				}
			}
		}
	}
	for(int i=0;i<10;i++){
        printf("%d, ", a[i]);
	}
}


int main(){
	int lista[10]={5,2,3,0,9,1,4,6,7,8};
	oddEvenSort(lista,10);
	return 0;
}