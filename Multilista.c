#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct alunos{
    int id;
    char nome[20];
    struct alunos *proxAlu;
}listaAlun;

typedef struct disci{
    int id;
    char nome[20];
    int tamanho;
    struct alunos *listaAlu;
    struct disci *proxDisc;
}listaDisc;

typedef struct geral{
    int tamanho;
    struct disci *discInicial;
    struct disci *discFinal;
}listaGeral;

int inicializaGeral(listaGeral *materias, listaDisc *disciInicial){
    materias->discFinal = disciInicial;
    materias->discInicial = disciInicial;
    materias->tamanho = 1;
    disciInicial->listaAlu = NULL;
    disciInicial->proxDisc = NULL;
    disciInicial->tamanho = 0;

    printf("\nNome da disciplina: ");
    scanf("%s",disciInicial->nome);
    getchar();
    printf("\nID da disciplina: ");
    scanf("%d", &disciInicial->id);
    getchar();

    return 1;
}

int insereDisciplina(listaGeral *materias){
    listaDisc *novaDisc = (listaDisc *) malloc(sizeof(listaDisc));
    materias->tamanho++;

    printf("\nNome da disciplina: ");
    scanf("%s", novaDisc->nome);
    getchar();
    printf("\nID da disciplina: ");
    scanf("%d", &novaDisc->id);
    getchar();

    novaDisc->proxDisc = NULL;
    novaDisc->listaAlu = NULL;
    novaDisc->tamanho = 0;


    materias->discFinal->proxDisc = novaDisc;
    materias->discFinal = novaDisc;

    return 1;
}

int exibeDisc(listaGeral *materias){
    listaDisc *discAtual;
    discAtual = materias->discInicial;

    printf("\nDisciplinas registradas: ");
    while(discAtual != NULL){
        printf("\nDisciplina: %s \n  Codigo: %d\n", discAtual->nome, discAtual->id);
        listaAlun *alunoAtual = discAtual->listaAlu;
        printf("Numero de alunos registrados: %d", discAtual->tamanho);
        if(alunoAtual != NULL){
            while(alunoAtual != NULL){
                printf("\n   ||Aluno: %s \n   ||Matricula: %d\n", alunoAtual->nome, alunoAtual->id);
                alunoAtual = alunoAtual->proxAlu;
            }
        }
        else{
            printf("\nNenhum aluno matriculado.\n");
        }
        discAtual = discAtual->proxDisc;
    }
    return 1;
}

int adicionarAluno(listaDisc *discAtual, char nome[20], int id){
    listaAlun *novoAluno = (listaAlun *) malloc(sizeof(listaAlun));
    strcpy(novoAluno->nome, nome);
    novoAluno->id = id;
    novoAluno->proxAlu = NULL;

    printf("\nAluno matriculado.");
    novoAluno->proxAlu = discAtual->listaAlu;
    discAtual->listaAlu = novoAluno;

    discAtual->tamanho++;
    return 1;
}

int matricula(listaGeral *materias){
    listaDisc *atualDisci = materias->discInicial;
    char nomeDisc[20];
    char nome[20];
    int id;

    printf("\nDigite nome da disciplina: ");
    scanf("%s", nomeDisc);
    getchar();

    while(atualDisci != NULL){
        if(strcmp(atualDisci->nome,nomeDisc) == 0){
            break;
        }
        atualDisci = atualDisci->proxDisc;
    }

    if(atualDisci == NULL){
        printf("\nDisciplina nao encontrada.");
        return 1;
    }

    printf("\nDigite o nome do aluno(a): ");
    scanf("%s", &nome);
    getchar();
    printf("\nDigite a matricula do aluno: ");
    scanf("%d", &id);

    adicionarAluno(atualDisci,nome,id);
    return 1;
}

int excluiMatricula(listaGeral *materias){
    listaDisc *atualDisc = materias->discInicial;
    listaAlun *atualAlun;
    listaAlun *anterAlun = NULL;
    char nome[20];
    char nomeDisc[20];
    int id;

    printf("\nDigite nome da disciplina: ");
    scanf("%s", &nomeDisc);
    getchar();
    printf("\nDigite o nome do aluno(a): ");
    scanf("%s", &nome);
    getchar();
    printf("\nDigite a matricula do aluno: ");
    scanf("%d", &id);

    while(atualDisc != NULL){
        if(!strcmp(atualDisc->nome,nomeDisc)){
            break;
        }
        atualDisc = atualDisc->proxDisc;
    }
    atualAlun = atualDisc->listaAlu;
    while(atualAlun != NULL){
        if((strcmp(atualAlun->nome,nome) == 0) && atualAlun->id == id){
            if(atualAlun == atualDisc->listaAlu){
                atualDisc->listaAlu = atualAlun->proxAlu;
            }
            else{
                anterAlun->proxAlu = atualAlun->proxAlu;
            }
            free(atualAlun);
            atualDisc->tamanho--;
            printf("\nAluno %s, matricula %d removido.", nome, id);
            return 1;
        }
        anterAlun = atualAlun;
        atualAlun = atualAlun->proxAlu;
    }
    printf("\nAluno nao encontrado.");
    return 0;
}

void main(){
    listaGeral materias;
    listaDisc disciplinaInicial;
    int choice = 0;

    do{
        if(choice == 1){
            inicializaGeral(&materias, &disciplinaInicial);
        }
        if(choice == 2){
            insereDisciplina(&materias);
        }
        if(choice == 3){
            matricula(&materias);
        }
        if(choice == 4){
            excluiMatricula(&materias);
        }
        if(choice == 5){
            exibeDisc(&materias);
        }
        printf("\nEscolha das opcoes:\n");
        printf("0: encerra o programa\n");
        printf("1: incializa\n");
        printf("2: cria disciplina\n");
        printf("3: matricula aluno em disciplina\n");
        printf("4: exclui aluno de uma disciplina\n");
        printf("5: exibe geral\n :");
        scanf("%d", &choice);
        getchar();
    }while(choice != 0);
}
