#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Disciplina {
	char nome[31];
	int cargaHoraria;
	float notas[3];
	struct Disciplina *proximo;
} Disciplina;

typedef struct Aluno {
	char nome[21];
	float media;
	int quantidadeDeDisciplinas;
	struct Disciplina *disciplina;
	struct Aluno *proximo, *anterior;
} Aluno;

typedef struct ListaGerenciada {
	Aluno *inicio, *fim;
	int quant;
} ListaGerenciada;

void iniciaListaVazia(ListaGerenciada *lista) {
	lista->inicio = NULL;
	lista->fim = NULL;
	lista->quant = 0;
}

// pensei em fazer uma insercao ordenada por ordem alfabetica. Como
// aluno tb tem bastante atributos pensei tb coletarmos as informacoes
// dentro da propria funcao adicionarAluno
int adicionarAluno(ListaGerenciada *lista) {
	return 0;
}

int main(){
	
	return 0;
}

