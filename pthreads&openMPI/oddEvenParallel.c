#include <iostream>
#include <stdlib.h>
#include <mpi.h>

using namespace std;

int Compare(const void*, const void*);
int CPartner(int, int, int);
void Merge_low(int[], int[], int);
void Merge_high(int[], int[], int);

int main(int argc, char* argv[]){
	srand(time(NULL));
	int n = atoi(argv[1]);

	int array[n], comm_sz = 0, my_rank = 0,local_n = 0, fase = 0, partner = 0;

	int* local_arr;
	int* local_brr;

	MPI_Init(nullptr, nullptr);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	local_arr = (int*) malloc(sizeof(int)*(n/comm_sz));
	local_brr = (int*) malloc(sizeof(int)*(n/comm_sz));

	if(my_rank == 0){
		cout<<"Array: ";
		for (int i = 0; i < n; ++i) {
			array[i] = rand() % 20;
			cout<<array[i]<<"-";
		}
		cout<<endl;
	}
	
    local_n = n/comm_sz;
    // copia los elementos a los buffers de todos
    // para compartir el array a todos
	MPI_Scatter(array, local_n, MPI_INT, local_arr, local_n, MPI_INT, 0, MPI_COMM_WORLD);

	qsort(local_arr, local_n, sizeof(int), Compare);

	for(fase = 0; fase < comm_sz; ++fase){
		partner = CPartner(fase, my_rank, comm_sz);
		if(partner >= 0){
			MPI_Sendrecv(local_arr, n/comm_sz, MPI_INT, partner, 0, local_brr, n/comm_sz, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			if (my_rank < partner){ // companero intercambiado adelante
				Merge_low(local_arr, local_brr, local_n);
			}
			else{   // companero intercambiado atras
				Merge_high(local_arr, local_brr, local_n);
			}
		}
	}

	cout<<my_rank<<": ";
	for (int i = 0; i < local_n; ++i) {
		cout<<local_arr[i]<<"-";
	}
	cout<<endl;

	MPI_Finalize();

	return 0;

}

int CPartner(int fase, int my_rank, int comm_sz){
	int partner = 0;
	if (fase % 2 == 0){
		if (my_rank % 2 != 0)
			partner = my_rank - 1;
		else
			partner = my_rank + 1;
	}
	else{
		if (my_rank % 2 != 0)
			partner = my_rank + 1;
		else
			partner = my_rank - 1;
		if (partner == - 1 || partner == comm_sz){
			partner = MPI_PROC_NULL;
		}
	}
	return partner;
}
void Merge_low(int my_keys[],int recv_keys[], int local_n){

	int m_i = 0,
		r_i = 0,
		t_i = 0;

	int temp_keys[local_n];

	while ( t_i < local_n ) {
		if ( my_keys[m_i] <= recv_keys[r_i] ) {
			temp_keys[t_i] = my_keys[m_i];
			++m_i;
			++t_i;
		}
		else{
			temp_keys[t_i] = recv_keys[r_i];
			++r_i;
			++t_i;
		}
	}
	for ( m_i = 0; m_i < local_n; ++m_i) {
		my_keys[m_i] = temp_keys[m_i];
	}
}

int Compare(const void* a_p, const void* b_p) {
	int a = *((int*)a_p);
	int b = *((int*)b_p);

	if (a < b)
		return -1;
	else if (a == b)
		return 0;
	else /* a > b */
		return 1;
}


void Merge_high(int my_keys[],int recv_keys[], int local_n){

	int m_i = local_n - 1,
		r_i = local_n - 1,
		t_i = local_n - 1;

	int temp_keys[local_n];

	while ( t_i >= 0 ) {
		if ( my_keys[m_i] >= recv_keys[r_i] ) {
			temp_keys[t_i] = my_keys[m_i];
			--m_i;
			--t_i;
		}
		else{
			temp_keys[t_i] = recv_keys[r_i];
			--r_i;
			--t_i;
		}
	}
	for ( m_i = 0; m_i < local_n; ++m_i) {
		my_keys[m_i] = temp_keys[m_i];
	}
}