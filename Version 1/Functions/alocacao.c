void alocaMem(memoria **bloco, processo **lista_espera, int prioridade, int process_id, int tempo, char process_name[] int mem_solicitada) {
  memoria *melhor_local = NULL;

  // Obtenção do menor bloco de memória suficiente - Best Fit
  melhor_local = best_fit(bloco, mem_solicitada);

  if (melhor_local != NULL) { // Existe o melhor local
    memoria *aux = *bloco;
    memoria *novo = malloc(sizeof(memoria));
    // Inserção do novo processo alocado na memória
    // Atribuições
    novo->ocupado = 1;
    novo->process_id = process_id;
    novo->espaco = mem_solicitada;
    novo->prox = melhor_local;
    novo->ant = melhor_local->ant;
    // Atualização da quantidade de memória livre
    melhor_local->espaco -= mem_solicitada;
    // Inserção na lista
    if (melhor_local->ant == NULL) { // Primeiro endereço
      *bloco = novo;
    } else {
      (melhor_local->ant)->prox = novo;
    }
    melhor_local->ant = novo;
  } else { // Não há endereços válidos
    puts("Erro: Memoria insuficiente ou muito fragmentada! O processo será colocado na fila de espera");
    inserir(lista_espera, prioridade, process_id, tempo, 1, process_name, mem_solicitada);
  }
}

memoria* best_fit(memoria **bloco, int mem_solicitada) {
  memoria *aux = *bloco;
  memoria *melhor_posicao = NULL;
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
