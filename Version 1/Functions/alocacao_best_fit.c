#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct memoria {
  struct memoria *prox, *ant;
  int espaco, ocupado, process_id;
};

void alocaMem(struct memoria **bloco, int *process_id);

struct memoria* best_fit(struct memoria **bloco, int mem_solicitada);

void imprime(struct memoria *bloco);

int main() {
  srand(time(NULL));
  int op = 0, id = 0;
  char c;
  struct memoria *bloco_mem = malloc(sizeof(struct memoria));
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
      default:
        break;
    }
  } while(op != 0);

  return 0;
}

void alocaMem(struct memoria **bloco, int *process_id) {
  struct memoria *aux = *bloco;
  struct memoria *melhor_local = NULL;
  struct memoria *novo = malloc(sizeof(struct memoria));
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

struct memoria* best_fit(struct memoria **bloco, int mem_solicitada) {
  struct memoria *aux = *bloco;
  struct memoria *melhor_posicao = NULL;
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

void imprime(struct memoria *bloco) {
  struct memoria *aux = bloco;
  puts("Memoria:");
  while (aux != NULL) {
    printf("\tID do processo: %d\n\tMemoria usada: %d\n\tEstado: %d\n\n", aux->process_id, aux->espaco, aux->ocupado);
    aux = aux->prox;
  }
}
// Possível uso na remoção
// if (anterior->ocupado == 1 && (aux->prox)->ocupado == 1) { // Entre dois endereços ocupados
//   aux->espaco -= novo->espaco; // Reduz o equivalente do tamanho que será ocupado pelo novo processo
//   anterior->prox = novo;
//   novo->prox = aux;
// } else if (anterior->ocupado == 1) { // Endereço anterior não está sendo ocupado
//
// }
