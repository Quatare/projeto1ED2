void remover(memoria **bloco, memoria **bloco_inicio){
	memoria *aux = (*bloco);
	printf("%d -- %d -- %d",aux->espaco , aux->ocupado,aux->process_id);
	if(aux != NULL){ // Se existir o bloco de memoria
		if(aux == (*bloco_inicio)){ // Talvez n�o precisa desse.
			(*bloco_inicio) = aux->prox;
			if((*bloco_inicio) == NULL)
				(*bloco_inicio)->ant = NULL;
			free(aux);
			aux = (*bloco_inicio);
		}
		else{
			if(aux->prox == NULL){ // O Bloco a ser Removido e o ultimo
				memoria *aux2 = aux;
				aux = aux->ant;
				aux->prox = NULL;
				free(aux2);
				aux2 = NULL;
			}
			else{ // O bloco a ser Removido esta no meio
				aux->ant->prox = aux->prox;
				aux->prox->ant = aux->ant;
				memoria *aux2 = aux->prox;
				free(aux);
				aux = aux2;
			}
		}
	}
}


void desalocaMem(memoria **bloco_mem, int process_id){
	memoria *aux = (*bloco_mem);
	memoria *novo = malloc(sizeof(memoria));
	while(aux->process_id != process_id && aux->prox != NULL){ // Encontrar o Bloco do processo que vai ser desalocado
		aux = aux->prox;
	}
	memoria *aux_ant = aux->ant;
	memoria *aux_prox = aux->prox;
	if(aux != NULL){
		if(aux_ant == NULL){// O bloco a ser desalocado e o primeiro da lista
			if(aux_prox->ocupado == 0){ // O proximo bloco n�o esta com processo
				novo->espaco = aux_prox->espaco + aux->espaco; // Soma dos espa�os
				aux->espaco = novo->espaco;
				aux->ocupado = 0; // Libera a ocupa��o do bloco
				remover(&aux_prox, bloco_mem ); // Remove o proximo bloco, ja que os espaco foi juntado ao bloco desalocado
				aux->process_id = -1; // o bloco agora � um bloco livre
				printf("Primeiro");
			}
			else{ // Caso o proximo esta ocupado
				aux->ocupado = 0;
				aux->process_id = -1;
			}
		}
		else {
			if(aux_prox == NULL){ // O bloco a ser desalocado � o �ltimo
			 	if(aux_ant->ocupado == 0){ // Bloco anterior esta livre
					novo->espaco = aux_ant->espaco + aux->espaco; // Soma os espa�os
					aux->espaco = novo->espaco;
					aux->ocupado = 0;
					aux->process_id = -1;
					remover(&aux_ant, bloco_mem); // Remove o bloco anterior, ja que os espa�os foram juntados
					printf("Segundo");
			 	}
				else{ // Caso o anterior esta ocupado
					aux->ocupado = 0;
					aux->process_id = -1;
				}
			}
			else{ // O bloco esta no meio da lista
				if(aux_prox->ocupado == 0 && aux_ant->ocupado == 1){ // So o proximo bloco esta livre
				   novo->espaco = aux_prox->espaco + aux->espaco; // Soma os espa�os livres
					aux->espaco = novo->espaco;
					aux->ocupado = 0;
					remover(&aux_prox, bloco_mem ); // Remove o proximo bloco, ja que os espacos foram somados
					aux->process_id = -1;
					printf("Terceiro");
				}
				else{
					if(aux_prox->ocupado == 1 && aux_ant->ocupado == 0){ // So o bloco anterior esta livre
						novo->espaco = aux_ant->espaco + aux->espaco; // Soma os espa�os livres
						aux->espaco = novo->espaco;
						aux->ocupado = 0;
						aux->process_id = -1;
						remover(&aux_ant, bloco_mem); // Remove o bloco anterior, ja que os espa�os forma somados
						printf("Quarto");
					}
					else{
						if(aux_prox->ocupado == 1 && aux_ant->ocupado == 1){ // Os dois espa�os adjacentes estao ocupados
							aux->ocupado = 0;
							aux->process_id = -1;
						}
						else{	// Os dois espa�os adjacentes estao livres
							novo->espaco = aux_ant->espaco + aux_prox->espaco + aux->espaco; // Soma os espacos livres
							aux->espaco = novo->espaco;
							aux->ocupado = 0;
							aux->process_id = -1;
							remover(&aux_ant, bloco_mem); // Remove o bloco anterior, ja que os espa�os foram somados
							remover(&aux_prox, bloco_mem); // Remove o proximo bloco, ja que os espa�os foram somados
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
