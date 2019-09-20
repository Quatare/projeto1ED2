void removerMem(memoria **bloco, memoria **bloco_inicio) {
	memoria *aux = (*bloco);
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

void desalocaMem(memoria **bloco_mem, int process_id){
	memoria *aux = *bloco_mem;
	while(aux->process_id != process_id && aux->prox != NULL){ // Encontrar o Bloco do processo que vai ser desalocado
		aux = aux->prox;
	}
	memoria *aux_ant = aux->ant;
	memoria *aux_prox = aux->prox;
	if(aux != NULL){
    if (aux->process_id == process_id) {
			// O bloco a ser desalocado e o primeiro da lista
			if(aux_ant == NULL){
				memoria *novo = malloc(sizeof(memoria));
				if(aux_prox->ocupado == 0){ // O proximo bloco não esta com processo
					novo->espaco = aux_prox->espaco + aux->espaco; // Soma dos espaços
					aux->espaco = novo->espaco;
					aux->ocupado = 0; // Libera a ocupação do bloco
					remover(&aux_prox, bloco_mem ); // Remove o proximo bloco, ja que os espaco foi juntado ao bloco desalocado
					aux->process_id = -1; // o bloco agora é um bloco livre
				} else{ // Caso o proximo esta ocupado
					aux->ocupado = 0;
					aux->process_id = -1;
				}
			} else {
				// O bloco a ser desalocado é o último
				if(aux_prox == NULL){
					memoria *novo = malloc(sizeof(memoria));
					if(aux_ant->ocupado == 0){ // Bloco anterior esta livre
						novo->espaco = aux_ant->espaco + aux->espaco; // Soma os espa�os
						aux->espaco = novo->espaco;
						aux->ocupado = 0;
						aux->process_id = -1;
						remover(&aux_ant, bloco_mem); // Remove o bloco anterior, ja que os espa�os foram juntados
					}
					else{ // Caso o anterior esta ocupado
						aux->ocupado = 0;
						aux->process_id = -1;
					}
				}
				else{ // O bloco esta no meio da lista
					if (aux_prox->ocupado == 0 && aux_ant->ocupado == 1){ // So o proximo bloco esta livre
						aux->espaco += aux_prox->espaco; // Soma os espaços livres
						aux->ocupado = 0;
						remover(&aux_prox, bloco_mem ); // Remove o proximo bloco, ja que os espacos foram somados
						aux->process_id = -1;
					} else if (aux_prox->ocupado == 1 && aux_ant->ocupado == 0){ // So o bloco anterior esta livre
						aux->espaco += aux_ant->espaco; // Soma os espaços livres
						aux->ocupado = 0;
						aux->process_id = -1;
						remover(&aux_ant, bloco_mem); // Remove o bloco anterior, ja que os espaços foram somados
					} else if (aux_prox->ocupado == 1 && aux_ant->ocupado == 1){ // Os dois espaços adjacentes estão ocupados
						aux->ocupado = 0;
						aux->process_id = -1;
					}	else{	// Os dois espaços adjacentes estao livres
						aux->espaco += aux_ant->espaco + aux_prox->espaco; // Soma os espacos livres
						aux->ocupado = 0;
						aux->process_id = -1;
						remover(&aux_ant, bloco_mem); // Remove o bloco anterior, ja que os espa�os foram somados
						remover(&aux_prox, bloco_mem); // Remove o proximo bloco, ja que os espa�os foram somados
					}
				}
			}
    }
	} else
    printf("\n\nERROR, ID de memoria incorreto\n\n");
}
