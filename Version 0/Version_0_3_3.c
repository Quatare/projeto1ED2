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

//Inserção de elementos nas Listas Principais (RR e Prioridade)

void inserir(struct processo **inicio, int prioridade, int id, int tempo, int tipo, char rotulo[]) {
  struct processo *novo = NULL, *aux = NULL, *anterior = NULL;
  novo = malloc(sizeof(struct processo)); // Novo processo na lista
  aux = *inicio;

  // AtribuiÃ§Ã£o de valores
  strcpy(novo->rotulo , rotulo);
  novo->prioridade = prioridade;
  novo->id = id;
  novo->tempo = tempo;

  // Decide a posiÃ§Ã£o a qual deve ser inserido
  if (tipo) { // 1 = FIFO, 0 = Prioridade
    // Inicio da FIFO
    strcpy(novo->tipo_lista , "FIFO");

    if (*inicio == NULL) { // Lista vazia
      novo->prox = NULL;
      *inicio = novo;
    } else { // Lista contÃªm elementos
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
    } else { // Lista contÃªm elementos
      if (novo->prioridade >= (*inicio)->prioridade) { // Inicio da fila
        novo->prox = *inicio;
        *inicio = novo;
      } else {
        // Percorre a lista atÃ© achar a posiÃ§Ã£o correta
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

//Separar em duas listas

void separar(struct processo **fifo, struct processo **prior, char rotulo[], int id, int tempo, int prioridade) {
	if (tempo >= 7) {//caso tempo muito alto (lista fifo)
		inserir(fifo, prioridade, id, tempo, 1, rotulo);
	} else if (tempo < 7) {//caso nÃ£o (lista por prioridade)
		inserir(prior, prioridade, id, tempo, 0, rotulo);
  }
}

// RemoÃ§Ã£o Lista

void remover(struct processo **lista){
	struct processo *aux = NULL;
	aux = *lista;
	*lista = aux->prox;
	free(aux);
}

// Inserção na Lista de Dispositivos

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

// Criador de Programas

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

// Impressão de Lista

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
	if(!(*lista == NULL)){ // Ver se há algum Elemento a ser Interrompido 
		printf("Processo %s foi Interrompido! Processo em Dispositivo.\n",aux->rotulo);
		InserirDisp(lista_disp , aux->prioridade, aux->id, aux->tempo, aux->tipo_lista, aux->rotulo); // Inserindo na Lista de Dispositivos
		remover(lista); // Removendo da Lista em que o Elemento se Encontrava
	}
}

// Voltar Processo para Pronto

void retornar_processoDisp(struct processo **fifo, struct processo **prior, struct processo **lista_disp){
	if(!(*lista_disp == NULL)){ // Ver se Tem algum Elemento a ser Retornado
		struct processo *aux = *lista_disp;
		printf("Tempo de espera terminado! Processo %s esta RETORNANDO.\n", aux->rotulo);
		if(!(strcmp(aux->tipo_lista, "PRIORIDADE"))) // Processo Retornando era da Lista de Prioridades
			inserir(prior , aux->prioridade, aux->id, aux->tempo, 0 , aux->rotulo);
		else // Processo Retornando era da Lista fifo
			inserir(fifo , aux->prioridade, aux->id, aux->tempo, 1 , aux->rotulo);
		remover(lista_disp);
	}
}

// Gerador de Interrupção

int ver_interrupcao(){
	int aux = rand()%9 + 1;
	printf("\nInterruptor%d\n",aux);
	if(aux > 5)
		return 1;
	else 
		return 0;
}

//Executa processo na lista de prioridades
void executa_prior (struct processo **prior, struct processo **disp){
	int quantum = 3; //Definição do valor do quantum para teste
	if(ver_interrupcao()){
		fazer_interrupcao(prior , disp);
	}
	else if (!(*prior == NULL)){ //Verifica se a lista está vazia
	  		(*prior)->tempo -= quantum; //Reduz o tempo total do processo de acordo com o quantum
	    
	   		if ((*prior)->tempo <= 0){ //Remove o processo da lista se o tempo do processo zerar
			    printf ("Processo%s da lista de %s foi finalizado!\n\n", (*prior)->rotulo, (*prior)->tipo_lista);
			    remover (prior);
	   		}
	    	else{ /*Se o tempo não zerar realiza a cópia das informações do processo para um processo auxiliar e insere 
				novamente na lista de prioridades na posição correta com prioridade reduzida em 1*/
	        	struct processo *aux;
	        	aux->prioridade = (*prior)->prioridade;
  	        	aux->id = (*prior)->id;
	       		aux->tempo = (*prior)->tempo;
	        	strcpy(aux->tipo_lista, (*prior)->tipo_lista); 
	        	strcpy(aux->rotulo, (*prior)->rotulo);
	        	printf("Processo De Prioridade sendo executado: %s\n",(*prior)->rotulo);
	       		remover (prior); //Remove o processo da lista
	        
	        	if (aux->prioridade >= 1) //Reduz a prioridade em 1 se prioridade >= 1
	            	aux->prioridade--;
		    	inserir(prior, aux->prioridade, aux->id, aux->tempo, 0 , aux->rotulo); //Insere novamente na lista 
			}
	}
}

//Executa processo na Lista FIFO

void executa_fifo (struct processo **fifo, struct processo **disp){
	int quantum = 5;
	if(ver_interrupcao()){ // Ver se ocorre Interrupção
		fazer_interrupcao(fifo , disp);	 // Fazendo a Interrupção
	}	
	else{
		if (!(*fifo == NULL)){ //Verifica se a lista está vazia
			(*fifo)->tempo -= quantum;
			if((*fifo)->tempo <= 0){
				printf ("Processo %s da lista %s foi finalizado!\n\n", (*fifo)->rotulo, (*fifo)->tipo_lista);
				remover (fifo); //Remove o processo da lista
			}
			else{
				struct processo *aux;
	        	aux->prioridade = (*fifo)->prioridade;
  	        	aux->id = (*fifo)->id;
	       		aux->tempo = (*fifo)->tempo;
	        	strcpy(aux->tipo_lista, (*fifo)->tipo_lista); 
	        	strcpy(aux->rotulo, (*fifo)->rotulo);
	        	printf("Processo De Round-Robin sendo executado: %s\n",(*fifo)->rotulo);
				remover(fifo);
				inserir(fifo, aux->prioridade, aux->id, aux->tempo, 1 , aux->rotulo); //Insere novamente na lista 
			} 
			
		}
	}
}

//Alterna a execução das duas filas pronto, executa 4x prior e 1x fifo, >>NECESSITA DE TESTES<<

void alterna_pronto (struct processo **fifo, struct processo **prior, struct processo **disp, int cont_exec){
	if ((!(*fifo == NULL) && cont_exec % 5 == 0) || (*prior == NULL))
	    executa_fifo (fifo,disp);
    else 
	    executa_prior (prior,disp);   
}

int main(void) {
  int op = 0;
  int id_programa = 0, id_processo = 0;
  int atual = 1; // So para teste de interrupção, arrumar depois da alternação
  int cont_inter = 0; // So para teste de interrupção
  int cont_exec = 0;
  char c;
  struct processo *disp = NULL;
  struct processo *fifo = NULL, *prior = NULL;

  do {
    puts("1 - Gerar programa");
    puts("2 - Imprimir listas");
    puts("3 - Executar processos");
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
      	printf ("--------------\n");
        puts("FIFO:");
        imprime_lista(fifo);
        printf ("--------------\n");
        puts("PRIORIDADE:");
        imprime_lista(prior);
        printf ("--------------\n");
        puts("DISPOSITIVO:");
        imprime_lista(disp);
        printf ("--------------\n");
        break;
      case 3:
      	cont_exec++;
      	cont_inter++;
      	alterna_pronto (&fifo, &prior, &disp, cont_exec);
		if((cont_inter % 5) == 0){ // A Cada 5 Unidades o Primeiro Elemento de Dispositivo Volta a sua Lista de Origem
				retornar_processoDisp(&fifo , &prior, &disp);
			}
    	break;
      default: break;
    }
  } while(op != 0);

  return 0;
}
