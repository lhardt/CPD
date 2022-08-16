#include <stdio.h>
#include <stdlib.h>

typedef struct HashTable { 
	// 
	char ** lista; 

} HashTable;

HashTable * criaTabela(int tamanho){
	HashTable * table = calloc(1, sizeof(HashTable));
	table->lista = calloc(tamanho, sizeof(char));
	return table;
}

int busca(HashTable * table, char * item){
	return -1;
}

int insere(HashTable * table, char* item){
	return -1;
}

int main(){
	char nome[32], sobrenome[32];
	
	HashTable * table = criaTabela(503);

	while( EOF != scanf("%s %s", nome, sobrenome) )){
		char * str = calloc(64, sizeof(char*));
		sprintf(str, "%s %s", nome, sobrenome);
		insere(table, str);
	}
	
		

	return 0;
}
