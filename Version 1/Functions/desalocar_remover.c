#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "testando.h"



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
      	printf("Isto � apenas um teste, digite o id da memoria a ser desalocada:");
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
  // Atribuições
  novo->ocupado = 1;
  novo->espaco = rand() % 63 + 1;
  novo->process_id = *process_id;
  // Obtenção do menor bloco de memória suficiente - Best Fit
  melhor_local = best_fit(bloco, novo->espaco);

  if (melhor_local != NULL) { // Existe o melhor local
    // Atualização da quantidade de memória livre
    melhor_local->espaco -= novo->espaco;
    // Inserção do novo processo alocado na memória
    novo->prox = melhor_local;
    novo->ant = melhor_local->ant;
    if (melhor_local->ant == NULL) { // Primeiro endereço
      *bloco = novo;
    } else {
      (melhor_local->ant)->prox = novo;
    }
    //printf("Teste1\n");
    melhor_local->ant = novo;
    (*process_id)++;
  } else { // Não há endereços válidos
    puts("Erro: memoria insuficiente ou muito fragmentada");
  }
}

Memoria* best_fit(Memoria **bloco, int mem_solicitada) {
  Memoria *aux = *bloco;
  Memoria *melhor_posicao = NULL;
  int espaco_do_menor_bloco_possivel = 0;
  int flag = 0; // Marca a existência de pelo menos um espaço válido
  // Busca pelo primeiro endereço válido para a alocação
  while (aux != NULL && flag == 0) {
    if (aux->ocupado == 0 && aux->espaco >= mem_solicitada) {
      melhor_posicao = aux;
      espaco_do_menor_bloco_possivel = aux->espaco;
      flag = 1;
    }
    aux = aux->prox;
  }
  // Verifica se existe pelo menos um local válido
  if (flag == 0) { // Não existe memória disponível
    return NULL; // Melhor posição não existe, pois falta memória
  } else {
    // Verifica se é realmente o melhor local
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
	if(aux != NULL){
		if(aux == (*bloco_inicio)){
			(*bloco_inicio) = aux->prox;
			if((*bloco_inicio) == NULL)
				(*bloco_inicio)->ant = NULL;
			free(aux);
			aux = (*bloco_inicio);
		}
		else{
			if(aux->prox == NULL){
				Memoria *aux2 = aux;
				aux = aux->ant;
				aux->prox = NULL;
				free(aux2);
				aux2 = NULL;
			}
			else{
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
	Memoria *aux_ant = (*bloco_mem)->ant;
	Memoria *aux_prox = (*bloco_mem)->prox;
	Memoria *aux = (*bloco_mem);
	Memoria *novo = malloc(sizeof(Memoria));
	while(aux->process_id != process_id){
		aux = aux->prox;
	}
	if(aux_ant == NULL){
		if(aux_prox->ocupado == 0){
			novo->espaco = aux_prox->espaco + aux->espaco;
			aux->espaco = novo->espaco;
			aux->ocupado = 0;
			remover(&aux_prox, bloco_mem );
			aux->process_id = -1;
		}
		else{
			aux->ocupado = 0;
			aux->process_id = -1;
		}
	}
	else {
		if(aux_prox == NULL){
		 	if(aux_ant->ocupado == 0){
				novo->espaco = aux_ant->espaco + aux->espaco;
				aux->espaco = novo->espaco;
				aux->ocupado = 0;
				aux->process_id = -1;
				remover(&aux_ant, bloco_mem);
		 	}
			else{
				aux->ocupado = 0;
				aux->process_id = -1;
			}
		}
		else{
			if(aux_prox->ocupado == 0 && aux_ant->ocupado == 1){
			   novo->espaco = aux_prox->espaco + aux->espaco;
				aux->espaco = novo->espaco;
				aux->ocupado = 0;
				remover(&aux_prox, bloco_mem );
				aux->process_id = -1;
			}
			else{
				if(aux_prox->ocupado == 1 && aux_ant->ocupado == 0){
					novo->espaco = aux_ant->espaco + aux->espaco;
					aux->espaco = novo->espaco;
					aux->ocupado = 0;
					aux->process_id = -1;
					remover(&aux_ant, bloco_mem);
				}
				else{
					if(aux_prox->ocupado == 1 && aux_ant->ocupado == 1){
						aux->ocupado = 0;
						aux->process_id = -1;
					}
					else{	
						novo->espaco = aux_ant->espaco + aux_prox->espaco + aux->espaco;
						aux->espaco = novo->espaco;
						aux->ocupado = 0;
						aux->process_id = -1;
						remover(&aux_ant, bloco_mem);
						remover(&aux_prox, bloco_mem);
					}
				}
			}
		}
	}
}

void imprime(Memoria *bloco){
  Memoria *aux = bloco;
  puts("Memoria:");
  while (aux != NULL) {
    printf("\tID do processo: %d\n\tMemoria usada: %d\n\tEstado: %d\n\n", aux->process_id, aux->espaco, aux->ocupado);
    aux = aux->prox;
  }
}

