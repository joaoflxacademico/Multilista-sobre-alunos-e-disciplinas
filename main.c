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
	char nome[50];
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
	for (int i = 0;string[i] != '\0'; i++) 
		string[i] = tolower(string[i]);
}

// desalocar a memoria das disciplinas quando um aluno é excluido
int deletaDisciplinas(Aluno *aluno) {
	Disciplina *proximo, *atual = aluno->disciplinas;
	if (!atual)  return -1;
		
	while(atual != NULL){
		proximo = atual->proximo;
		free(atual);
		atual = proximo;
	}
	
	aluno->disciplinas = NULL;
	aluno->quantidadeDeDisciplinas = 0;
	
	return 1;
}

int cadastrarAluno(ListaGerenciada *lista) {
	Aluno *aluno = (Aluno*)malloc(sizeof(Aluno));
	if (!aluno)
		return -1;
	
	printf("Insira o nome do aluno: ");
	fflush(stdin);
	fgets(aluno->nome, sizeof(aluno->nome), stdin);
	caixaBaixa(aluno->nome);
	printf("Insira a matricula do aluno(xxxyyyzzz): ");
	scanf(" %s", aluno->matricula);
	aluno->media = 0;
	aluno->quantidadeDeDisciplinas = 0;
	aluno->disciplinas = NULL;
	aluno->disciplinasFim = NULL;
		
	
	if (lista->quant == 0) {
		aluno->anterior = NULL;
		aluno->proximo = NULL;
		lista->inicio = aluno;
		lista->fim = aluno;
		lista->quant++;
		printf("\nAluno foi cadastrado!");
		return 1;
	}
	
	Aluno *atual = lista->inicio;
	while(atual!= NULL){
		if(strcmp(aluno->nome, atual->nome) == 0){
			printf("\n Aluno ja cadastrado!");
			free(aluno);
			return -1;}
		else if (strcmp(aluno->nome, atual->nome) < 0){
			aluno->anterior = atual->anterior;
			aluno->proximo = atual;
			if(atual->anterior != NULL)
				atual->anterior->proximo = aluno;
			else lista->inicio = aluno;
			atual->anterior = aluno;
			lista->quant++;
			printf("\nAluno foi cadastrado!");
			return 1;}
		atual = atual->proximo;
	}
	
	// se chegar ao final da lista
	aluno->anterior = lista->fim;
	aluno->proximo = NULL;
	lista->quant++;
	lista->fim->proximo = aluno;
	lista->fim = aluno;
	return 1;
}

int removerAluno(ListaGerenciada *lista) {
	char nome[21];
	printf("Insira o nome do aluno que deseja remover: ");
	fflush(stdin);
	fgets(nome, sizeof(nome), stdin);
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
	fflush(stdin);
	fgets(nome, sizeof(nome), stdin);
	caixaBaixa(nome);
	
	// sem alunos para cadastrar disciplina
	Aluno *auxiliar = lista->inicio;
	if (!auxiliar)
		return -1; 
	
	// retorna em auxiliar, o aluno escolhido
	while(auxiliar != NULL){
		if (strcmp(auxiliar->nome, nome) == 0) break;
		else 	auxiliar = auxiliar->proximo;}
	
	if(auxiliar == NULL) return -1; // aluno não encontrado
	
	// se o aluno foi encontrado aloca uma memoria, evitando memory leaks
	Disciplina *novaDisciplina = (Disciplina*)malloc(sizeof(Disciplina));
	if (!novaDisciplina) 
		return -1;
	
	printf("Insira o nome da disciplina: ");
	fflush(stdin);
	fgets(novaDisciplina->nome, sizeof(novaDisciplina->nome),stdin);
	printf("Insira o periodo(aaaa.x): ");
	scanf(" %s", novaDisciplina->periodo);
	printf("Insira a nota do aluno: ");
	scanf("%f", &novaDisciplina->nota);
	printf("Insira o percentual de presenca: ");
	scanf("%f", &novaDisciplina->percentualDePresenca);
	
	char sit[3]; // estado auxiliar
	//estrutura de decisão baseada na precedência: TR-> RF -> RM se nenhum desses AP
	if(novaDisciplina->percentualDePresenca == 0)  		strcpy(sit,"TR");
	else if(novaDisciplina->percentualDePresenca < 70)  strcpy(sit,"RF");
	else if(novaDisciplina->nota < 5)  				    strcpy(sit,"RM");
	else 												strcpy(sit,"AP");
	strcpy(novaDisciplina->situacao,sit);
	auxiliar->quantidadeDeDisciplinas++;
	
	//não há disciplinas cadastradas para esse aluno
	if(auxiliar->disciplinas == NULL){
		novaDisciplina->proximo = NULL;
		auxiliar->disciplinas = novaDisciplina;
		auxiliar->disciplinasFim = novaDisciplina;}
	else{
		novaDisciplina->proximo = auxiliar->disciplinas;
		auxiliar->disciplinas= novaDisciplina;}
	return 1;
}

int exibirAlunosCadastrados(ListaGerenciada *lista) {
	Aluno *auxiliar = lista->inicio;
	if (!auxiliar)
		return -1;
		
	printf("\n----Alunos Cadastrados----\n");
	for (int i = 0; i < lista->quant; i++) {
		printf("\n[%d] -> %s", i, auxiliar->nome);
		printf("- %s\n", auxiliar->matricula);
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
	char nome[50];
	printf("Insira o nome do aluno que deseja checar: ");
	fflush(stdin);
	fgets(nome, sizeof(nome), stdin);
	caixaBaixa(nome);
	
	//lista vazia
	Aluno *auxiliar = lista->inicio;
	if (!auxiliar){
		printf("\nA lista esta vazia"); 
		return -1;}
		
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
	
	printf("Bem vindo!\n");
	printf("\n >>> Multilista Alunos/Disciplinas! <<< \n\n\n\n");
	
	do {
		printf("\n");
		system("pause");
		system("cls");
		printf("\nCadastrar aluno..........................1\n");
		printf("Cadastrar disciplina de aluno............2\n");
		printf("Remover aluno............................3\n");
		printf("Exibir lista de alunos cadastrados.......4\n");
		printf("Exibir historico de aluno especifico.....5\n");
		printf("Sair.....................................0\n");
		printf("Insira sua opcao: ");
		scanf(" %d", &opcao);
		
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
	
	// quando o programa encerra ele desaloca a lista.
	Aluno *proximo, *atual = lista->inicio;
	while(atual != NULL){
		proximo = atual->proximo;
		deletaDisciplinas(atual);
		free(atual);
		atual = proximo;
	}
	free(lista);
	return 0;
}

