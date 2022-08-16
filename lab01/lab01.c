// COMPILADO COM 'gcc -Wall -g' sem nenhum aviso.
// Léo Hardt. 00333562. 12/jul/22.
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MIN(a,b) ( (a) > (b) ? (b) : (a) )
#define SWP(a,b) do{int tmp = a; a = b; b = tmp; } while(0)

#define H_SHELL 0
#define H_KNUTH 1
#define H_CIURA 2	
#define H_MAX	3

char * seq_name[] = {"SHELL", "KNUTH", "CIURA"};

int h_values[][30] = {
	// Shell
	{1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576},
	// Knuth
	{1,4,13,40,121,364,1093,3280,9841,29524,88573,265720,797161,2391484},
	// Ciura
	{1,4,10,23,57,132,301,701,1577,3548,7983,17961,40412,90927,204585,460316,1035711}
};

void print_list(int * lista, int n){
	int i = 0;
	if(n == 0) return;

	while(i < n-1){
		printf("%d, ", lista[i]);
		++i;
	}
	printf("%d ", lista[i]); 

}

// Diz, da lista de 'h's possíveis, qual é o índice do H que devemos usar.
// TIPO_H se refere a 0,1, ou 2, se for Shell, Knuth ou Ciura, respc.
int get_h_index(int n, int tipo_h){
	for(int i = 0; i < n; ++i){
		if( h_values[tipo_h][i+1] >= n)
			return i; 
	}
	printf("Asking for invalid h_index! %d  %d\n", n, tipo_h);
	return -999;
}

// O programa anterior era lento porque implementei selection sort ao invés de insertion sort.
void selection_sort_aux(int * lista, int n, int h, int i_iter){
	// Sort com os elementos [i_iter, i_iter + h, ... ]
	int start_i = i_iter; // onde começam os itens desordenados
	// Percorre todos os 'da esquerda' que já estão ordenados.
	while(start_i < n){ 
		// Percorre todo o resto da lista para encontrar o menor
		int menor_i = start_i;
		for(int i = start_i+h; i < n; i += h){
			if(lista[i] < lista[menor_i]){
				menor_i = i;
			}
		}
		// Põe o menor no início 
		SWP(lista[menor_i], lista[start_i]);
		start_i += h;
	}
}


void sort_aux(int * lista, int n, int h, int i_iter){
	// Um insertion sort com os elementos [i_iter, i_iter + h, ... ]

	// onde começam os itens desordenados
	// começamos no segundo item (i_iter + h) porque 
	// a lista de um item único já está ordenada.
	int start_i = i_iter + h; 

	// start_i é o início da lista desordenada.
	while(start_i < n){ 
		int to_insert = lista[start_i];
		int ins_pos = start_i ; // última posição dos ja ordenados.

		while(ins_pos >= h && lista[ins_pos-h] > to_insert ){
			lista[ins_pos] = lista[ins_pos - h]   ;
			ins_pos -= h;
		}
		lista[ins_pos] = to_insert;
				

		start_i += h;
	
	}

}

int shell_sort(int * lista, int n, int algo, bool show){
	int h_i = get_h_index(n, algo);
	int h = 999; // dummy value just to enter the while.

	if(show){
		print_list(lista, n);
		printf("SEQ=%s\n", seq_name[algo]);

	}

	while(h > 1){
		h = h_values[algo][h_i];
		h_i--; // próximo H vai ser um menor na lista

		// Quando h for 100, ele terá que executar sort_aux com
		// i_iter de 0 a 99.
		for(int i_iter = 0; i_iter < h; ++i_iter){		
			//printf("\tSortAux %d %d %d\n", n, h, i_iter);
			sort_aux(lista, n, h, i_iter);
		} 
		
		if(show){
			print_list(lista, n);
			printf("INCR=%d\n", h);
		}
	}

	return 0;
}

int main(){
	int n_lista;
	// Alocar como vetor diretamente sobrecarrega a stack.
	int * lista = (int*) calloc(1000005, sizeof(int));
	int * lista_cp  = (int*) calloc(1000005, sizeof(int));

	while( scanf("%d ", &n_lista) != EOF ){
		for(int i = 0; i < n_lista; ++i){
			scanf("%d", &lista[i]);
		}
		
		bool show_step = false;
		bool show_time = true;

		// algoritmos 0, 1, 2 == shell, knuth, ciura 
		for(int algo = 0; algo < H_MAX; ++algo){
			for(int i = 0; i < n_lista; ++i){ lista_cp[i] = lista[i]; }
			clock_t t_start = clock();
			shell_sort(lista_cp, n_lista, algo, show_step);
			clock_t t_end = clock();

			double n_sec = (t_end - t_start + 0.0) / CLOCKS_PER_SEC;
			if(show_time){
				printf("%s,%d,%lf\n", seq_name[algo], n_lista, n_sec);
			}
		}
	
	}

	free(lista);
	free(lista_cp);

	return 0;
}
