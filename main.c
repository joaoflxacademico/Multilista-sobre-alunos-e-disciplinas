#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Disciplina {
	char nome[31];
	char periodo[7];
	char situacao[3];
	float nota;
	float percentualDePresenca;
	struct Disciplina *proximo;
} Disciplina;

typedef struct Aluno {
	char nome[21];
	char matricula[10];
	float media;
	int quantidadeDeDisciplinas;
	struct Disciplina *disciplinas, *disciplinasFim;
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

void caixaBaixa(char *string) {
	for (int i = 0; i < string[i]; i++) {
		string[i] = tolower(string[i]);
	}
}

// essa funcao é usada para desalocar a memoria das disciplinas quando 
// um aluno é excluido
int deletaDisciplinas(Aluno *aluno) {
	Disciplina *auxiliar = aluno->disciplinas;
	if (!auxiliar)
		return -1;
		
	for (int i = 0; i < aluno->quantidadeDeDisciplinas; i++) {
		free(aluno->disciplinas);
		auxiliar = auxiliar->proximo;
		aluno->disciplinas = auxiliar;
	}
	free(auxiliar);
	return 1;
}

int cadastrarAluno(ListaGerenciada *lista) {
	Aluno *aluno = (Aluno*)malloc(sizeof(Aluno));
	if (!aluno)
		return -1;
	
	printf("Insira o nome do aluno: ");
	scanf(" %s", aluno->nome);
	caixaBaixa(aluno->nome);
	printf("Insira a matricula do aluno(xxxyyyzzz): ");
	scanf(" %s", aluno->matricula);
	aluno->media = 0;
	aluno->quantidadeDeDisciplinas = 0;
	aluno->disciplinas = NULL;
		
	if (lista->quant == 0) {
		aluno->anterior = NULL;
		aluno->proximo = NULL;
		lista->inicio = aluno;
		lista->fim = aluno;
	}
	else {
		aluno->anterior = lista->fim;
		aluno->proximo = NULL;
		lista->fim->proximo = aluno;
		lista->fim = aluno;
	}
	lista->quant++;
	return 1;
}

int removerAluno(ListaGerenciada *lista) {
	char nome[21];
	printf("Insira o nome do aluno que deseja remover: ");
	scanf(" %s", nome);
	caixaBaixa(nome);
	
	Aluno *auxiliar = lista->inicio;
	if (!auxiliar) 
		return -1;
		
	for (int i = 0; i < lista->quant; i++) {
		if (strcmp(auxiliar->nome, nome) == 0) {
			deletaDisciplinas(auxiliar); // desaloca as disciplinas
			
			// caso o removido seja elemento unico da lista, inicializa a
			// lista como vazia novamente
			if (lista->fim == auxiliar && lista->inicio == auxiliar) {
				iniciaListaVazia(lista);
				return 1;
			}
			// caso o removido seja o ultimo na lista, altera o ponteiro 
			//"fim" dela
			else if (lista->fim == auxiliar) 
				lista->fim = auxiliar->anterior;
				
			// caso o removido seja o primeiro na lista, altera o ponteiro 
			//"inicio" dela
			else if (lista->inicio == auxiliar) 
				lista->inicio = auxiliar->proximo;
		
			auxiliar->anterior->proximo = auxiliar->proximo;
			auxiliar->proximo->anterior = auxiliar->anterior;
			
			lista->quant--;
			free(auxiliar);
			return 1; // encontrou o aluno
		}
		auxiliar = auxiliar->proximo;	
	}
	return -1; // o aluno nao foi encontrado na lista
}

int cadastrarDisciplina(ListaGerenciada *lista) {
	char nome[21];
	printf("Insira o nome do aluno em que deseja cadastrar a disciplina: ");
	scanf(" %s", nome);
	caixaBaixa(nome);
	
	Disciplina *novaDisciplina = (Disciplina*)malloc(sizeof(Disciplina));
	if (!novaDisciplina) 
		return -1;
		
	printf("Insira o nome da disciplina: ");
	scanf(" %s", novaDisciplina->nome);
	printf("Insira o periodo(aaaa.x): ");
	scanf(" %s", novaDisciplina->periodo);
	printf("Insira a nota do aluno: ");
	scanf("%f", &novaDisciplina->nota);
	printf("Insira o percentual de presenca: ");
	scanf("%f", &novaDisciplina->percentualDePresenca);
	
	if( novaDisciplina->nota >= 5 && novaDisciplina->percentualDePresenca > 70 ){
	    strcpy(novaDisciplina->situacao,"AP");
	}else{
	    if(novaDisciplina->nota < 5){
	        strcpy(novaDisciplina->situacao,"RM");
	    }else{
	        if(novaDisciplina->percentualDePresenca == 0){
	            strcpy(novaDisciplina->situacao,"TR");
	        }else{
	            strcpy(novaDisciplina->situacao,"RF");
	        }
	    }
	}
	caixaBaixa(novaDisciplina->situacao);
	
	Aluno *auxiliar = lista->inicio;
	if (!auxiliar)
		return -1;
	
	for (int i = 0; i < lista->quant; i++) {
		if (strcmp(auxiliar->nome, nome) == 0) {
			novaDisciplina->proximo = NULL;
			if (auxiliar->quantidadeDeDisciplinas == 0) {
				auxiliar->disciplinas = novaDisciplina;
				auxiliar->disciplinasFim = novaDisciplina;
			}
			// caso nao seja a primeira disciplina, sera adicionada sempre no final
			else {
				auxiliar->disciplinasFim->proximo = novaDisciplina;
				auxiliar->disciplinasFim = novaDisciplina;
			}
			auxiliar->quantidadeDeDisciplinas++;
			return 1; // encontrou o aluno
		} 
		auxiliar = auxiliar->proximo;
	}
	return -1; // o aluno nao foi encontrado na lista
}

int exibirAlunosCadastrados(ListaGerenciada *lista) {
	Aluno *auxiliar = lista->inicio;
	if (!auxiliar)
		return -1;
		
	printf("\n----Alunos Cadastrados----\n");
	for (int i = 0; i < lista->quant; i++) {
		printf("[%d]%s", i, auxiliar->nome);
		printf(",%s", auxiliar->matricula);
		auxiliar = auxiliar->proximo;
	}
	return 1;
}

int exibirDisciplinas(Disciplina *disciplinas, int n) { // usada em exibirHistoricoDeAluno
	Disciplina *auxiliar = disciplinas;
	if (!auxiliar)
		return -1;
		
	for (int i = 0; i < n; i++) {
		printf("\n----Nome disciplina %d: %s----\n", i+1, auxiliar->nome);
		printf("->Nota: %.1f\n", auxiliar->nota);
		printf("->Percentual de presenca: %.1f%%\n", auxiliar->percentualDePresenca);
		printf("->Situacao: %s\n", auxiliar->situacao);
		printf("->Periodo: %s\n", auxiliar->periodo);
		auxiliar = auxiliar->proximo;
	}
	return 1;
}

int exibirHistoricoDeAluno(ListaGerenciada *lista) {
	char nome[21];
	printf("Insira o nome do aluno que deseja checar: ");
	scanf(" %s", nome);
	caixaBaixa(nome);
	
	Aluno *auxiliar = lista->inicio;
	if (!auxiliar) 
		return -1;
		
	for (int i = 0; i < lista->quant; i++) {
		if (strcmp(auxiliar->nome, nome) == 0) {
			exibirDisciplinas(auxiliar->disciplinas, auxiliar->quantidadeDeDisciplinas);
			return 1; // aluno encontrado
		}
		auxiliar = auxiliar->proximo;
	}
	return -1; // aluno nao encontrado
}

int main(){
	int opcao;
	ListaGerenciada *lista = (ListaGerenciada*)malloc(sizeof(ListaGerenciada));
	iniciaListaVazia(lista);
	
	printf("Bem vindo!");
	do {
		printf("\nCadastrar aluno..........................1\n");
		printf("Cadastrar disciplina de aluno............2\n");
		printf("Remover aluno............................3\n");
		printf("Exibir lista de alunos cadastrados.......4\n");
		printf("Exibir historico de aluno especifico.....5\n");
		printf("Sair.....................................0\n");
		printf("Insura sua opcao: ");
		scanf("%d", &opcao);
		
		switch(opcao) {
			case(1):
				cadastrarAluno(lista);
				break;
			case(2):
				cadastrarDisciplina(lista);
				break;
			case(3):
				removerAluno(lista);
				break;
			case(4):
				exibirAlunosCadastrados(lista);
				break;
			case(5):
				exibirHistoricoDeAluno(lista);
				break;
			case(0):
				printf("Encerrando o programa...");
				break;
			default:
				printf("Opcao invalida!\n");
				break;
		}
	} while (opcao != 0);
	return 0;
}

