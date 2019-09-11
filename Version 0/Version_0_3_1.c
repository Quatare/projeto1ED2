#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

struct processo {
  char rotulo[100], tipo_lista[11];
  int id, tempo, prioridade;
  struct processo *prox, *ant;
};

struct dispositivo{
	char rotulo[100], tipo_lista[11];
	int id, tempo, prioridade;
	struct dispositivo *prox, *ant;
};

void create_program_dir() {
  struct stat estado = {0};

  if (stat("./programs", &estado) == -1) {
    printf("Criando dirtorio \"./programs\"\n");
    mkdir("./programs");
  }
}

//insere elementos em lista
void inserir(struct processo **inicio, int prioridade, int id, int tempo, int tipo, char rotulo[]) {
  struct processo *novo = NULL, *aux = NULL, *anterior = NULL;
  novo = malloc(sizeof(struct processo)); // Novo processo na lista
  aux = *inicio;

  // Atribuição de valores
  strcpy(novo->rotulo , rotulo);
	novo->prioridade = prioridade;
	novo->id = id;
	novo->tempo = tempo;

  // Decide a posição a qual deve ser inserido
  if (tipo) { // 1 = FIFO, 0 = Prioridade
    // Inicio da FIFO
    strcpy(novo->tipo_lista , "FIFO");

    if (*inicio == NULL) { // Lista vazia
      novo->prox = NULL;
      *inicio = novo;
    } else { // Lista contêm elementos
      // Insere no final da lista
      while (aux != NULL) {
        anterior = aux;
        aux = aux->prox;
      }
      anterior->prox = novo;
      novo->prox = NULL;
    }
  } else { // Inicio da lista por prioridade
    strcpy(novo->tipo_lista , "PRIORIDADE");

    if (*inicio == NULL) { // Lista vazia
      novo->prox = NULL;
      *inicio = novo;
      return;
    } else { // Lista contêm elementos
      if (novo->prioridade > (*inicio)->prioridade) { // Inicio da fila
        novo->prox = *inicio;
        *inicio = novo;
      } else {
        // Percorre a lista até achar a posição correta
        while (aux != NULL && (novo->prioridade < aux->prioridade)) {
          anterior = aux;
          aux = aux->prox;
        }
        if (aux == NULL) { // Menor prioridade, fim da lista
          anterior->prox = novo;
          novo->prox = NULL;
        } else { // Prioridade intermediaria, no meio da lista
          novo->prox = anterior->prox;
          anterior->prox = novo;
        }
      }
    }
  }
}

//separa em duas listas
void separar(struct processo **fifo, struct processo **prior, char rotulo[], int id, int tempo, int prioridade) {
	if (tempo >= 7) {//caso tempo muito alto (lista fifo)
		inserir(fifo, prioridade, id, tempo, 1, rotulo);
	} else if (tempo < 7) {//caso não (lista por prioridade)
		inserir(prior, prioridade, id, tempo, 0, rotulo);
  }
}

// Remoção Lista

void remover(struct processo **lista){
	struct processo *aux = NULL;
	aux = *lista;
	*lista = aux->prox;
	free(aux);
}

void InserirDisp(struct processo **disp,int prioridade, int id, int tempo, char tipo_lista[], char rotulo[] ){
	struct processo *novo = NULL, *anterior = NULL, *aux = NULL;
	aux = *disp;
  	novo = malloc(sizeof(struct processo));
  	novo->prioridade = prioridade;
  	novo->id = id;
	novo->tempo = tempo;
	strcpy(novo->tipo_lista, tipo_lista); 
	strcpy(novo->rotulo, rotulo);
	if(*disp == NULL){
		novo->prox = NULL;
		*disp = novo;
	}
	else{
		while(aux != NULL) {
        	anterior = aux;
        	aux = aux->prox;
    	}
    	anterior->prox = novo;
    	novo->prox = NULL;
	}	
}



void cria_programa(int *program_id, int *process_id, struct processo **fifo, struct processo **prior){
  FILE *programa;
  char filename[100], process_name[100], linha[150];
  int i, qtd_processos = rand()%4 + 1;
  int id, tempo, prioridade;

  create_program_dir();
  sprintf(filename, "programs/Teste%02d.txt", *program_id);

  programa = fopen(filename, "a");
  if (programa != NULL) {
    srand(time(NULL));
    for(i = 0; i < qtd_processos; i++) { // Gera n processos
      sprintf(process_name, "Processo_%02d", *process_id);
      fprintf(programa,"Processo:%s ID:%d Tempo:%d Prioridade:%d\n", process_name, *process_id, rand()%15 + 1, rand()%6);
      (*process_id)++;
    }
    fclose(programa);
    (*program_id)++;
    // Carrega n processos
    if((programa = fopen(filename, "r")) != NULL) {
      while(fgets(linha, sizeof(linha), programa)) {
        sscanf(linha, "Processo:%s ID:%d Tempo:%d Prioridade:%d", process_name, &id, &tempo, &prioridade);
        separar(fifo, prior, process_name, id, tempo, prioridade);
      }
      fclose(programa);
    } else {
      perror("Falha em carregar processos!");
      exit(EXIT_FAILURE);
    }
  } else {
    perror(filename);
    exit(EXIT_FAILURE);
  }
}

void imprime_lista(struct processo *lista) {
  struct processo *aux = lista;
  while (aux != NULL) {
    printf("Nome: %s\nID: %d\nTempo: %d\nPrioridade: %d\n\n", aux->rotulo, aux->id, aux->tempo, aux->prioridade);
    aux = aux->prox;
  }
  printf("\n");
}

// Colocando Processo em Dispositivo
void fazer_interrupcao(struct processo **lista , struct processo **lista_disp){
	struct processo *aux = *lista;
	if(!(*lista == NULL)){
		printf("Processo %s foi Interrompido! Processo em Dispositivo.\n",aux->rotulo);
		InserirDisp(lista_disp , aux->prioridade, aux->id, aux->tempo, aux->tipo_lista, aux->rotulo);
		remover(lista);
	}
}

// Voltar Processo para Pronto

void retornar_processoDisp(struct processo **fifo, struct processo **prior, struct processo **lista_disp){
	if(!(*lista_disp == NULL)){
		struct processo *aux = *lista_disp;
		printf("Tempo de espera terminado! Processo %s esta RETORNANDO.\n", aux->rotulo);
		if(!(strcmp(aux->tipo_lista, "PRIORIDADE"))) // Processo Retornando era da Lista de Prioridades
			inserir(prior , aux->prioridade, aux->id, aux->tempo, 0 , aux->rotulo);
		else // Processo Retornando era da Lista fifo
			inserir(fifo , aux->prioridade, aux->id, aux->tempo, 1 , aux->rotulo);
		remover(lista_disp);
	}
}

// Finalizar Processo

void finalizar_processoDisp(struct processo **lista){
	struct processo *aux = *lista;
	printf("Processo %s foi finalizado! Processo foi retirado de PRONTO.\n", aux->rotulo);
	remover(lista);
}




int ver_interrupcao(){
	int aux = rand()%9 + 1;
	if(aux > 5)
		return 1;
	else 
		return 0;
}

int main(void) {
  int op = 0;
  int id_programa = 0, id_processo = 0;
  int atual = 1; // So para teste de interrup��o, arrumar depois da alterna��o
  int cont = 0; // So para teste de interrup��o
  char c;
  struct processo *disp = NULL;
  struct processo *fifo = NULL, *prior = NULL;

  do {
    puts("1 - Gerar programa");
    puts("2 - Imprimir listas");
    puts("3 - Executar processos");
    puts("5 - Teste Interrupcao");
	puts("0 - Sair");
    printf("Insira a opcao desejada: ");
    do {
      scanf("%d", &op);
      while((c = getchar()) != '\n' && c != EOF);
    } while(op < 0 && op > 3);
    printf("\n");

    switch (op) {
      case 1:
        cria_programa(&id_programa, &id_processo, &fifo, &prior);
        break;
      case 2:
        puts("FIFO:");
        imprime_lista(fifo);
        puts("Prioridade:");
        imprime_lista(prior);
        puts("Dispositivo:");
        imprime_lista(disp);
        break;
		case 5:
			cont++;
			if(ver_interrupcao()){
				if(atual){ // Lista de prioridades na CPU
					fazer_interrupcao(&prior , &disp);	
					imprime_lista(disp);
				}
				else{// Lista fifo na CPU
					fazer_interrupcao(&fifo,&disp);
					imprime_lista(disp);
				}
			}
			
			// Parte da troca
			/*
			else{
				if(atual){ // Lista de prioridades na CPU
					remover(&prior);
					inserir()
				}
				else // Lista fifo na CPU
					remover(&fifo);
			}*/
			
			
			
			if((cont % 4) == 0){ // A cada 2 unidades o primeiro elemento de dispositivo volta
				retornar_processoDisp(&fifo , &prior, &disp);
			}
			break;
      default: break;
    }
  } while(op != 0);

  return 0;
}
