#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "memoria.h"
#include "processo.h"
//as estruturas foram mudadas e at� o presente momento n�o deram nenhum erro,ps testei mta coisa
void espera(processo **esp,int id,int espa,int tempo,int prioridade,char rotulo[100],char tipo_lista[11])//lista de espera recebe tudo que a estrutura de processo nova deve ter
{//esta func�o deve estar ok
	processo *novo = (processo*)malloc(sizeof(processo));
	novo->espaco=espa;
	novo->id = id;
	
	novo->prioridade = prioridade;
	strcpy(novo->rotulo,rotulo);
	strcpy(novo->tipo_lista,tipo_lista);
	processo *aux = *esp;
	if(*esp == NULL)
	{
		novo->prox=NULL;
		novo->ant=NULL;
		*esp = novo;
		printf("sda");
	}
	else
	{
		while(aux->prox != NULL)
		{
			aux = aux->prox;
		}
		aux->prox=novo;
		novo->ant=aux;
		novo->prox=NULL;
	}
}
void busca_memoria(Mem **memoria,processo **esp)//deveria buscar na memoria por espa�o livre
{
	Mem *aux=*memoria;
	processo *busca=*esp;
	if(*memoria == NULL)//memoria vazia
	{
		printf("memoria vazia, insira processo na memoria\n");
	}
	else if(*esp == NULL)//lista vazia
	{
		printf("n�o a processos em espera para entrar na memoria\n");
	}
	else
	{
		while(aux != NULL)
		{
			printf("entrou no 1 while\n");
			if(aux->ocupado == 0)//achei alguem que ta livre
			{
				Mem *novo=(Mem*)malloc(sizeof(Mem));
				printf("achou valor vazio\n");
				busca=*esp;
				while(busca != NULL && busca->espaco > aux->espaco)
				{
					printf("buscando lista\n");//crasha aki n sei pq provavelmente ponteiro
					busca=busca->prox;
				}
				if(busca->espaco == aux->espaco)//caso valor seja igual
				{
					printf("caso igual\n");
					aux->process_id=busca->id;
					aux->ocupado=1;
					if(busca->ant == NULL)
					{
						(busca->prox)->ant=NULL;
						free(busca);
					}
					else if(busca->prox == NULL)
					{
						(busca->ant)->prox=NULL;
						free(busca);
					}
					else
					{
						(busca->prox)->ant=busca->ant;
						(busca->ant)->prox=busca->prox;
						free(busca);
					}
				}
				else if(aux->espaco > busca->espaco)//caso menor
				{
					printf("caso menor\n");//print pra ver onde ta
					int valor=aux->espaco - busca->espaco;
					aux->process_id=busca->id;
					aux->ocupado=1;
					aux->espaco=busca->espaco;
					novo->espaco=valor;
					novo->ocupado=0;
					novo->process_id=-1;
					novo->prox=aux->prox;
					(aux->prox)->ant=novo;
					novo->ant=aux;
					aux->prox=novo;
					if(busca->ant == NULL)//remover lista
					{
						(busca->prox)->ant=NULL;
						free(busca);
					}
					else if(busca->prox == NULL)
					{
						(busca->ant)->prox=NULL;
						free(busca);
					}
					else
					{
						(busca->prox)->ant=busca->ant;
						(busca->ant)->prox=busca->prox;
						free(busca);
					}
				}
				else("valor n encontrado\n");//nunca nem entrou aki mas ok
			}
			printf("id:%d\n",aux->process_id);//visualizar
			printf("tempo:%d\n",aux->espaco);//visualizar
			aux = aux->prox;
		}
	}
}
Mem *best_fit(Mem **bloco, int mem_solicitada) {
  Mem *aux = *bloco;
  Mem *melhor_posicao = NULL;
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

void alocaMem(Mem **bloco, processo **esp,int *process_id) {
  Mem *aux = *bloco;
  Mem *melhor_local = NULL;
  Mem *novo = (Mem*)malloc(sizeof(Mem));
  // Atribuições
  novo->ocupado = 1;
  novo->espaco = rand() % 60 + 2;
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
  		espera(esp,novo->process_id,novo->espaco,10,1,"a","a");//aki da warning pq eu to testano porra
  		(*process_id)++;
  }
}
void remover(Mem **bloco, Mem **bloco_inicio){
	Mem *aux = (*bloco);
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
				Mem *aux2 = aux;
				aux = aux->ant;
				aux->prox = NULL;
				free(aux2);
				aux2 = NULL;
			}
			else{ // O bloco a ser Removido esta no meio
				aux->ant->prox = aux->prox;
				aux->prox->ant = aux->ant;
				Mem *aux2 = aux->prox;
				free(aux);
				aux = aux2;
			}
		}
	}
}


void desalocaMem(Mem **bloco_mem, int process_id){
	Mem *aux_ant = (*bloco_mem)->ant;
	Mem *aux_prox = (*bloco_mem)->prox;
	Mem *aux = (*bloco_mem);
	Mem *novo = (Mem*)malloc(sizeof(Mem));
	while(aux->process_id != process_id){ // Encontrar o Bloco do processo que vai ser desalocado
		aux = aux->prox;
	}
	if(aux_ant == NULL){// O bloco a ser desalocado e o primeiro da lista
		if(aux_prox->ocupado == 0){ // O proximo bloco não esta com processo
			novo->espaco = aux_prox->espaco + aux->espaco; // Soma dos espaços
			aux->espaco = novo->espaco;
			aux->ocupado = 0; // Libera a ocupação do bloco
			remover(&aux_prox, bloco_mem ); // Remove o proximo bloco, ja que os espaco foi juntado ao bloco desalocado
			aux->process_id = -1; // o bloco agora é um bloco livre
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
			}
			else{
				if(aux_prox->ocupado == 1 && aux_ant->ocupado == 0){ // So o bloco anterior esta livre
					novo->espaco = aux_ant->espaco + aux->espaco; // Soma os espaços livres
					aux->espaco = novo->espaco;
					aux->ocupado = 0;
					aux->process_id = -1;
					remover(&aux_ant, bloco_mem); // Remove o bloco anterior, ja que os espaços forma somados
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
					}
				}
			}
		}
	}
}

void imprime(Mem *bloco,processo *esp) {
  Mem *aux = bloco;
  processo *aux2 = esp;
  puts("Memoria:");
  while (aux != NULL) {
    printf("\tID do processo: %d\n\tMemoria usada: %d\n\tEstado: %d\n\n", aux->process_id, aux->espaco, aux->ocupado);
    aux = aux->prox;
  }
    puts("espera:");

  while (aux2 != NULL) {
    printf("\tID do processo: %d\n\tespaco: %d\n\tchar: %s\n\n", aux2->id, aux2->espaco, aux2->rotulo);
    aux2 = aux2->prox;
  }  
}
int main() {
  srand(time(NULL));
  int op = 0, id = 0, teste;
  char c;
  Mem *bloco_mem = (Mem*)malloc(sizeof(Mem));
  bloco_mem->prox = bloco_mem->ant = NULL;
  bloco_mem->espaco = 256;
  bloco_mem->ocupado = 0;
  bloco_mem->process_id = -1;
  processo *espera=NULL;
  do {
    puts("1 - Alocar memoria");
    puts("2 - Imprimir");
    puts("3 - Testar Desaloca");
    puts("4 - Testar alocar");
	puts("0 - Sair");
    
    do {
      scanf("%d", &op);
      while((c = getchar()) != '\n' && c != EOF);
    } while(op < 0 && op > 4);
    printf("\n");

    switch (op) {
      case 1:
        alocaMem(&bloco_mem,&espera ,&id);
        break;
      case 2:
        imprime(bloco_mem,espera);
        break;
      case 3:
      	printf("Isto é apenas um teste, digite o id da memoria a ser desalocada:");
      	scanf("%d",&teste);
      	desalocaMem(&bloco_mem,teste);
      	break;
	  case 4:
	  	busca_memoria(&bloco_mem,&espera);
	  	break;
      default:
        break;
    }
  } while(op != 0);

  return 0;
}

