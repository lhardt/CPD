#include <stdio.h>
#include <time.h>	 	// srand(time(null))
#include <stdlib.h> 	// rand for random partitioning

#define PART_MEDIANA   0
#define PART_ALEATORIO 1

#define PART_LOMUTO    0
#define PART_HOARE     1

#define MAX(a,b) ( (a) < (b) ? (b) : (a) )
#define MIN(a,b) ( (a) > (b) ? (b) : (a) )
#define SWP(a,b) do{int tmp = a; a = b; b = tmp; } while(0)

typedef struct Stats {
	int size, n_swaps, n_rec;
	double n_sec;
} Stats;

// swap e salva nos stats. 
#define SWP_ST(a,b) do{int tmp = a; a = b; b = tmp; stats->n_swaps++; } while(0)


void print_list(int * lista, int n){
	int i = 0;
	if(n == 0) return;

	while(i < n-1){
		printf("%d, ", lista[i]);
		++i;
	}
	printf("%d ", lista[i]);

}

int pivot_mediana(int * lista, int n_lista){
	if(n_lista < 2) return 0;

	int maior = MAX(lista[0], lista[n_lista-1]);
	int menor = MIN(lista[0], lista[n_lista-1]);

	int centro = lista[n_lista/2];

	int mediana = centro;

	if(centro > maior) mediana = maior;
	if(centro < menor) mediana = menor;

	// encontrei a mediana.
	// agora só redescobrir o índice.
	// até poderia ter feito de outra
	// forma mas ainda é bem rápido.

	return (mediana == lista[0]) ? 0
		 : (mediana == lista[n_lista-1]) ? n_lista-1
		 : (mediana == lista[n_lista/2]) ? n_lista/2
		 : -1;
}

int pivot_aleatorio(int * lista, int n_lista){
	if(n_lista < 2) return 0;

	return rand() % n_lista;
}

// Pressupondo lista[0] == pivot
int part_lomuto(int * lista, int n_lista, Stats * stats){
	if(n_lista < 2) return 0;

	int fim_menores = 0;
	int pivot = lista[0];


	// All the smaller ones go to the start.
	for(int i = 1; i < n_lista; ++i){
		if(lista[i] < pivot){
			SWP_ST(lista[fim_menores + 1], lista[i]);
			++fim_menores;
		}
	}

	// Versão Reversa:
	// for(int i = 1; i < inicio_maiores; ++i){
	//
	// 	if(lista[i] > pivot){
	// 		SWP_ST(lista[inicio_maiores - 1], lista[i]);
	// 		--i;
	// 		--inicio_maiores;
	// 	}
	// }

	SWP_ST(lista[0], lista[fim_menores]);

	return fim_menores;
}

// Pressupondo lista[0] == pivot
int part_hoare(int * lista, int n_lista, Stats * stats){
	if(n_lista < 2) return 0;

	int fim_menores = 0, inicio_maiores = n_lista;
	int pivot = lista[0];

	while(fim_menores < inicio_maiores-1){
		// Na primeira vez, não causa nenhum problema,
		// mas é útil para as outras iterações.
		SWP_ST(lista[fim_menores+1], lista[inicio_maiores-1]);

		while(fim_menores < inicio_maiores -1 && lista[fim_menores+1] <= pivot)
			++fim_menores;
		while(fim_menores < inicio_maiores -1 && lista[inicio_maiores-1] >= pivot)
			--inicio_maiores;
	}
	SWP_ST(lista[0], lista[inicio_maiores - 1]);

	return inicio_maiores-1;
}


void quicksort(int * lista, int n_lista, int t_pivot, int t_part, Stats * stats){
	if(n_lista <= 1) return;

	// O pivô inicial
	int i_pivot_ini = (t_pivot == PART_MEDIANA) ? pivot_mediana(lista, n_lista)
				: (t_pivot == PART_ALEATORIO)?pivot_aleatorio(lista, n_lista)
				: -1;

	// Coloca o pivô no lugar certo;
	SWP_ST(lista[i_pivot_ini], lista[0]);

	// Separa elementos e recebe a posição final do pivô.
	int i_pivot_fin = (t_part == PART_LOMUTO) ? part_lomuto(lista, n_lista, stats)
  			    : (t_part == PART_HOARE) ? part_hoare(lista, n_lista, stats)
				: -1;

	quicksort(lista, i_pivot_fin, t_pivot, t_part, stats);
	quicksort(lista + i_pivot_fin + 1, n_lista - i_pivot_fin - 1, t_pivot, t_part, stats);

	stats->n_rec++;

}

void print_stats(FILE * file, Stats* stats){
	fprintf(file, "TAMANHO ENTRADA %d\n", stats->size);
	fprintf(file, "SWAPS %d\n", stats->n_swaps);
	fprintf(file, "RECURSOES %d\n", stats->n_rec);
	fprintf(file, "TEMPO %lf\n", stats->n_sec);
}

// Tests and writes to the file stream.
void test_item(int * lista, int n_lista, int t_pivot, int t_part, FILE * file ){
	Stats stats = {.size=n_lista, .n_swaps=0, .n_rec=0, .n_sec=0.0};

	clock_t t_start = clock();
	quicksort(lista, n_lista, PART_MEDIANA, PART_HOARE, &stats);
	clock_t t_end = clock();
	stats.n_sec = (t_end - t_start + 0.0) / CLOCKS_PER_SEC;

	print_stats(file, &stats);

}

void copy_list(int * origem, int * dest, int n_lista){
	for(int i_lista = 0; i_lista<n_lista; ++i_lista){
		dest[i_lista] = origem[i_lista];
	}
}

int main(){
	srand(time(NULL));

	FILE * file_med_hoa = fopen("stats-mediana-hoare.txt", "w");
	FILE * file_med_lom = fopen("stats-mediana-lomuto.txt", "w");
	FILE * file_ran_hoa = fopen("stats-aleatorio-hoare.txt", "w");
	FILE * file_ran_lom = fopen("stats-aleatorio-lomuto.txt", "w");

	int n_lista = 0;
	int * lista = calloc(1000000, sizeof(int));
	int * lista_cp = calloc(1000000, sizeof(int));
	while(scanf("%d", &n_lista) != EOF){
		for(int i_lista = 0; i_lista<n_lista; ++i_lista){
			scanf("%d",&lista[i_lista]);
		}

		copy_list(lista, lista_cp, n_lista);
		test_item(lista, n_lista, PART_MEDIANA, PART_HOARE, file_med_hoa);
		copy_list(lista, lista_cp, n_lista);
		test_item(lista, n_lista, PART_MEDIANA, PART_LOMUTO, file_med_lom);
		copy_list(lista, lista_cp, n_lista);
		test_item(lista, n_lista, PART_ALEATORIO, PART_HOARE, file_ran_hoa);
		copy_list(lista, lista_cp, n_lista);
		test_item(lista, n_lista, PART_ALEATORIO, PART_LOMUTO, file_ran_lom);
	}
	return 0;
}
