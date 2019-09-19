#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "testando.h" // Inclui a struct Memoria



void alocaMem(Memoria **bloco, int *process_id);

Memoria *best_fit(Memoria **bloco, int mem_solicitada);

void imprime(Memoria *bloco);

void remover(Memoria **bloco, Memoria **bloco_inicio);

void desalocaMem(Memoria **bloco, int process_id);


int main() {
  srand(time(NULL));
  int op = 0, id = 0, teste;
  char c;
  Memoria *bloco_mem = malloc(sizeof(Memoria));
  bloco_mem->prox = bloco_mem->ant = NULL;
  bloco_mem->espaco = 256;
  bloco_mem->ocupado = 0;
  bloco_mem->process_id = -1;

  do {
    puts("1 - Alocar memoria");
    puts("2 - Imprimir");
    puts("3 - Testar Desaloca");
	puts("0 - Sair");
    
    do {
      scanf("%d", &op);
      while((c = getchar()) != '\n' && c != EOF);
    } while(op < 0 && op > 3);
    printf("\n");

    switch (op) {
      case 1:
        alocaMem(&bloco_mem, &id);
        break;
      case 2:
        imprime(bloco_mem);
        break;
      case 3:
      	printf("Isto é apenas um teste, digite o id da memoria a ser desalocada:");
      	scanf("%d",&teste);
      	desalocaMem(&bloco_mem,teste);
      default:
        break;
    }
  } while(op != 0);

  return 0;
}

void alocaMem(Memoria **bloco, int *process_id) {
  Memoria *aux = *bloco;
  Memoria *melhor_local = NULL;
  Memoria *novo = malloc(sizeof(Memoria));
  // AtribuiÃ§Ãµes
  novo->ocupado = 1;
  novo->espaco = rand() % 63 + 1;
  novo->process_id = *process_id;
  // ObtenÃ§Ã£o do menor bloco de memÃ³ria suficiente - Best Fit
  melhor_local = best_fit(bloco, novo->espaco);

  if (melhor_local != NULL) { // Existe o melhor local
    // AtualizaÃ§Ã£o da quantidade de memÃ³ria livre
    melhor_local->espaco -= novo->espaco;
    // InserÃ§Ã£o do novo processo alocado na memÃ³ria
    novo->prox = melhor_local;
    novo->ant = melhor_local->ant;
    if (melhor_local->ant == NULL) { // Primeiro endereÃ§o
      *bloco = novo;
    } else {
      (melhor_local->ant)->prox = novo;
    }
    //printf("Teste1\n");
    melhor_local->ant = novo;
    (*process_id)++;
  } else { // NÃ£o hÃ¡ endereÃ§os vÃ¡lidos
    puts("Erro: memoria insuficiente ou muito fragmentada");
  }
}

Memoria* best_fit(Memoria **bloco, int mem_solicitada) {
  Memoria *aux = *bloco;
  Memoria *melhor_posicao = NULL;
  int espaco_do_menor_bloco_possivel = 0;
  int flag = 0; // Marca a existÃªncia de pelo menos um espaÃ§o vÃ¡lido
  // Busca pelo primeiro endereÃ§o vÃ¡lido para a alocaÃ§Ã£o
  while (aux != NULL && flag == 0) {
    if (aux->ocupado == 0 && aux->espaco >= mem_solicitada) {
      melhor_posicao = aux;
      espaco_do_menor_bloco_possivel = aux->espaco;
      flag = 1;
    }
    aux = aux->prox;
  }
  // Verifica se existe pelo menos um local vÃ¡lido
  if (flag == 0) { // NÃ£o existe memÃ³ria disponÃ­vel
    return NULL; // Melhor posiÃ§Ã£o nÃ£o existe, pois falta memÃ³ria
  } else {
    // Verifica se Ã© realmente o melhor local
    while (aux != NULL) {
      if (aux->ocupado == 0 && aux->espaco >= mem_solicitada) {
        if (aux->espaco < espaco_do_menor_bloco_possivel) {
          espaco_do_menor_bloco_possivel = aux->espaco;
          melhor_posicao = aux;
        }
      }
      aux = aux->prox;
    }
    return melhor_posicao;
  }
}

void remover(Memoria **bloco, Memoria **bloco_inicio){
	Memoria *aux = (*bloco);
	printf("%d -- %d -- %d",aux->espaco , aux->ocupado,aux->process_id);
	if(aux != NULL){ // Se existir o bloco de memoria
		if(aux == (*bloco_inicio)){ // Talvez não precisa desse.
			(*bloco_inicio) = aux->prox;
			if((*bloco_inicio) == NULL)
				(*bloco_inicio)->ant = NULL;
			free(aux);
			aux = (*bloco_inicio);
		}
		else{
			if(aux->prox == NULL){ // O Bloco a ser Removido e o ultimo
				Memoria *aux2 = aux;
				aux = aux->ant;
				aux->prox = NULL;
				free(aux2);
				aux2 = NULL;
			}
			else{ // O bloco a ser Removido esta no meio
				aux->ant->prox = aux->prox;
				aux->prox->ant = aux->ant;
				Memoria *aux2 = aux->prox;
				free(aux);
				aux = aux2;
			}
		}
	}
}


void desalocaMem(Memoria **bloco_mem, int process_id){
	Memoria *aux = (*bloco_mem);
	Memoria *novo = malloc(sizeof(Memoria));
	while(aux->process_id != process_id && aux->prox != NULL){ // Encontrar o Bloco do processo que vai ser desalocado
		aux = aux->prox;
	}
	Memoria *aux_ant = aux->ant;
	Memoria *aux_prox = aux->prox;
	if(aux != NULL){	
		if(aux_ant == NULL){// O bloco a ser desalocado e o primeiro da lista
			if(aux_prox->ocupado == 0){ // O proximo bloco não esta com processo
				novo->espaco = aux_prox->espaco + aux->espaco; // Soma dos espaços
				aux->espaco = novo->espaco;
				aux->ocupado = 0; // Libera a ocupação do bloco
				remover(&aux_prox, bloco_mem ); // Remove o proximo bloco, ja que os espaco foi juntado ao bloco desalocado
				aux->process_id = -1; // o bloco agora é um bloco livre
				printf("Primeiro");
			}
			else{ // Caso o proximo esta ocupado
				aux->ocupado = 0;
				aux->process_id = -1;
			}
		}
		else {
			if(aux_prox == NULL){ // O bloco a ser desalocado é o último
			 	if(aux_ant->ocupado == 0){ // Bloco anterior esta livre
					novo->espaco = aux_ant->espaco + aux->espaco; // Soma os espaços
					aux->espaco = novo->espaco;
					aux->ocupado = 0;	
					aux->process_id = -1;
					remover(&aux_ant, bloco_mem); // Remove o bloco anterior, ja que os espaços foram juntados
					printf("Segundo");
			 	}
				else{ // Caso o anterior esta ocupado
					aux->ocupado = 0;
					aux->process_id = -1;
				}
			}
			else{ // O bloco esta no meio da lista
				if(aux_prox->ocupado == 0 && aux_ant->ocupado == 1){ // So o proximo bloco esta livre
				   novo->espaco = aux_prox->espaco + aux->espaco; // Soma os espaços livres
					aux->espaco = novo->espaco;
					aux->ocupado = 0;
					remover(&aux_prox, bloco_mem ); // Remove o proximo bloco, ja que os espacos foram somados
					aux->process_id = -1;
					printf("Terceiro");
				}
				else{
					if(aux_prox->ocupado == 1 && aux_ant->ocupado == 0){ // So o bloco anterior esta livre
						novo->espaco = aux_ant->espaco + aux->espaco; // Soma os espaços livres
						aux->espaco = novo->espaco;
						aux->ocupado = 0;
						aux->process_id = -1;
						remover(&aux_ant, bloco_mem); // Remove o bloco anterior, ja que os espaços forma somados
						printf("Quarto");
					}
					else{
						if(aux_prox->ocupado == 1 && aux_ant->ocupado == 1){ // Os dois espaços adjacentes estao ocupados
							aux->ocupado = 0;
							aux->process_id = -1;
						}
						else{	// Os dois espaços adjacentes estao livres
							novo->espaco = aux_ant->espaco + aux_prox->espaco + aux->espaco; // Soma os espacos livres
							aux->espaco = novo->espaco;
							aux->ocupado = 0;
							aux->process_id = -1;
							remover(&aux_ant, bloco_mem); // Remove o bloco anterior, ja que os espaços foram somados
							remover(&aux_prox, bloco_mem); // Remove o proximo bloco, ja que os espaços foram somados
							printf("quinto");
						}
					}
				}
			}
		}
	}
	else
		printf("\n\nERROR, ID de memoria incorreto\n\n");
}

void imprime(Memoria *bloco){
  Memoria *aux = bloco;
  puts("Memoria:");
  while (aux != NULL) {
    printf("\tID do processo: %d\n\tMemoria usada: %d\n\tEstado: %d\n\n", aux->process_id, aux->espaco, aux->ocupado);
    aux = aux->prox;
  }
}

