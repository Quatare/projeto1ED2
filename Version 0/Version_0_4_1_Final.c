/**************************************************************************/
/* Grupo: 5                                                               */
/* Integrantes: Edson Haruyuki Satake Junior, Erik Riyuji Yamamoto Aguena */
/* Mateus Marques Montagnoli, Jo�o Pedro Quadrado, Fabr�cio Gustavo Frank */
/* Projeto desenvolvido em ambiente Windows                               */
/* Compilador TDM-GCC 4.9.2 64-bit Release - Dev-C++ 5.11				  */
/**************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// Estrutura representativa dos processos
struct processo {
  char rotulo[100], tipo_lista[11];	//contem como atributos o rotulo(nome do processo, que tambem leva o programa oriundo),o tipo da lista(lista de prioridades ou pelo metodo round robin)
  int id, tempo, prioridade; // identificador, tempo necess�rio de processamento e nivel de prioridade (entre 0 e 5)
  struct processo *prox; // ponteiros para o proximo elemento
};

//funcao que faz a inser��o de elementos nas Listas Principais(RoundRobin e Prioridade)
void inserir(struct processo **inicio, int prioridade, int id, int tempo, int tipo, char rotulo[]) {
  struct processo *novo = NULL, *aux = NULL, *anterior = NULL;
  novo = malloc(sizeof(struct processo)); // Novo processo na lista alocando dinamicamente
  aux = *inicio;

  // Atribuindo os valores dos parametros no ponteiro novo
  strcpy(novo->rotulo , rotulo);
  novo->prioridade = prioridade;
  novo->id = id;
  novo->tempo = tempo;

  // Decide a posicao a qual deve ser inserido o elemento
  if (tipo) { //se o tipo for 1,o elemento ser� inserido na lista do metodo RoundRobin
    // Inicio da inser��o por roundrobin
    strcpy(novo->tipo_lista , "ROUNDROBIN");

    if (*inicio == NULL) { // Lista vazia
      novo->prox = NULL;
      *inicio = novo;
    } else { // Lista cont�m elementos
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
    } else { // Lista cont�m elementos
      if (novo->prioridade > (*inicio)->prioridade) { // Inicio da fila
        novo->prox = *inicio;
        *inicio = novo;
      } else {
        // Percorre a lista at� achar a posi��o correta
        while (aux != NULL && (novo->prioridade <= aux->prioridade)) {
          anterior = aux;
          aux = aux->prox;
        }
        if (aux == NULL) { // Menor prioridade, fim da lista
          anterior->prox = novo;
          novo->prox = NULL;
        } else { // Prioridade intermedi�ria, no meio da lista
          novo->prox = anterior->prox;
          anterior->prox = novo;
        }
      }
    }
  }
}


// Fun��o que decide em qual lista o processo gerado ir� entrar
void separar_processos(struct processo **rr, struct processo **prior, char rotulo[], int id, int tempo, int prioridade) {
  if (tempo >= 7) {//caso tempo muito alto(maior ou igual a 7),o processo � inserido na lista do m�todo RR
    printf("O processo %s foi colocado na FILA PELO METODO ROUNDROBIN\n",rotulo);
    inserir(rr, prioridade, id, tempo, 1, rotulo);
  } else if (tempo < 7) { //sen�o,o processo � inserido na lista  por prioridade
    printf("O processo %s foi colocado na FILA DE PRIORIDADE\n",rotulo);
    inserir(prior, prioridade, id, tempo, 0, rotulo);
  }
}


// Fun��o que faz a remo��o de um processo na lista
void remover(struct processo **lista){
  struct processo *aux = NULL;
  aux = *lista;
  *lista = aux->prox;
  free(aux);
}


// Fun��o que faz a inser��o de elementos na Lista de Dispositivos
// Recebe como par�metros os valores do processo que anteriormente estava armazenado na fila pronto
void InserirDisp(struct processo **disp,int prioridade, int id, int tempo, char tipo_lista[], char rotulo[] ){
  struct processo *novo = NULL, *anterior = NULL, *aux = NULL;
  aux = *disp;
  novo = malloc(sizeof(struct processo)); //aloca��o din�mica da mem�ria
  novo->prioridade = prioridade; //Atribuindo os valores dos par�metros no ponteiro novo
  novo->id = id;
  novo->tempo = tempo;
  strcpy(novo->tipo_lista, tipo_lista);
  strcpy(novo->rotulo, rotulo);
  if(*disp == NULL){ //verifica se a lista de dispositivo esta vazia
    novo->prox = NULL;
    *disp = novo;
  }
  else{ //lista contem elementos nela
    while(aux != NULL) {//percorre at� o ultimo elemento da lista
      anterior = aux;
      aux = aux->prox;
    }//e insere o processo no final da lista
    anterior->prox = novo;
    novo->prox = NULL;
  }
}

// Fun��o que cria os programas junto com a gera��o de processos
void cria_programa(int *program_id, int *process_id, struct processo **rr, struct processo **prior){
  FILE *programa; // Cari�vel FILE que ser� usada para manipula��es em arquivos
  char filename[100], process_name[100], linha[150];
  int i, qtd_processos;
  int id, tempo, prioridade;

  // Gera��o do nome do arquivo que ser� criado
  sprintf(filename, "Programa_%02d.txt", *program_id);

  programa = fopen(filename, "a"); // Abre ou cria um arquivo no modo de escrita "append"
  if (programa != NULL) { // Verifica se o arquivo pode ser aberto
    srand(time(NULL));
    qtd_processos = rand()%4 + 1;// Gera��o aleat�ria da quantidade de processos que o programa ter�
    printf("\n\nForam gerados %i processos neste programa!Abaixo se encontra os processos criados:\n\n",qtd_processos);
    for(i = 0; i < qtd_processos; i++) { // Cria��o dos processos do programa
      // Gera o nome do processo
      sprintf(process_name, "Processo_%02d_PROG_%02d", *process_id,*program_id);
      // Gera aleatoriamente o tempo total de execu��o do processo e sua prioridade
      fprintf(programa,"Processo:%s ID:%d Tempo:%d Prioridade:%d\n", process_name, *process_id, rand()%15 + 1, rand()%6);
      (*process_id)++;
    }
    fclose(programa); // Fecha o arquivo aberto do modo "append" para poder abr�-lo em outro modo
    (*program_id)++;
    // Carrega n processos
    if((programa = fopen(filename, "r")) != NULL) { // Tenta abrir o arquivo do programa no modo leitura
      while(fgets(linha, sizeof(linha), programa)) { // L� o arquivo linha a linha at� que chegue ao final
        // Leitura dos dados formatados presentes no arquivo representando o programa
        sscanf(linha, "Processo:%s ID:%d Tempo:%d Prioridade:%d", process_name, &id, &tempo, &prioridade);
        // Insere o processo carregado em uma das listas para futura execu��o;
        separar_processos(rr, prior, process_name, id, tempo, prioridade);
      }
      fclose(programa); // Fecha o arquivo que n�o ser� mais utilizado
    } else { // Caso ocorra uma falha na abertura do arquivo no modo leitura, finaliza o programa
      perror("Falha em carregar processos!"); // Erro a ser emitido
      exit(EXIT_FAILURE); // Finaliza��o do programa em caso de erro
    }
  } else { // Caso ocorra uma falha na abertura do arquivo no modo "append", finaliza o programa
  perror(filename);
  exit(EXIT_FAILURE);
}

}

// funcao que Imprime a Lista
void imprime_lista(struct processo *lista) {
  struct processo *aux = lista;
  while (aux != NULL) {// Percorre a lista at� o ponteiro aux ficar nulo,imprimindo todos os valores
    printf("Nome: %s\nID: %d\nTempo: %d\nPrioridade: %d\n\n", aux->rotulo, aux->id, aux->tempo, aux->prioridade);
    aux = aux->prox;
  }
  printf("\n");
}

// fun��o que insere o processo interrompido na lista de dispositivos e remove da lista em que estava
void fazer_interrupcao(struct processo **lista , struct processo **lista_disp){
  struct processo *aux = *lista;
  if(!(*lista == NULL)){ // Verifica se a lista est� vazia
    printf("\nO processo %s esta sendo executado!\n",aux->rotulo);
    printf("O processo %s foi interrompido e colocado na FILA DISPOSITIVO.\n\n",aux->rotulo);
    InserirDisp(lista_disp , aux->prioridade, aux->id, aux->tempo, aux->tipo_lista, aux->rotulo); // Inserindo na Lista de Dispositivos
    remover(lista); // Removendo da Lista em que o elemento se encontrava
  }
}


// fun��o que retorna o processo que se encontra na fila de dispositivos para uma das filas pronto
void retornar_processoDisp(struct processo **rr, struct processo **prior, struct processo **lista_disp){
  if(!(*lista_disp == NULL)){ // // Verifica se a lista est� vazia
    struct processo *aux = *lista_disp;
    printf("Tempo de espera terminado! Processo %s esta retornando para a sua antiga fila.\n", aux->rotulo);
    if(!(strcmp(aux->tipo_lista, "PRIORIDADE"))) // Verifica se o processo retornando era da lista de prioridades
      inserir(prior , aux->prioridade, aux->id, aux->tempo, 0 , aux->rotulo);
    else // Sen�o o processo retornando era da lista do metodo round robin
      inserir(rr , aux->prioridade, aux->id, aux->tempo, 1 , aux->rotulo);
    remover(lista_disp);
  }
}


// fun��o que gera aleatoriamente um n�mero de 1 a 10. Se o numero for maior que 6,o processo ser� interrompido, sen�o ,nao ser�
int gerar_interrupcao(){
  srand(time(NULL));
  int aux = rand()%9 + 1;
  if(aux > 6)
    return 1;
  else
    return 0;
}

// fun��o que executa um processo que est� na lista de prioridades
void executa_prior (struct processo **prior, struct processo **disp){
  int quantum = 3; // Defini��oo do valor do quantum

  if(gerar_interrupcao()){ // Chamada da fun��o que decide se ocorre interrup��o
    fazer_interrupcao(prior , disp); // Realiza��o da interrup��o
  }
  else if (!(*prior == NULL)){ // Verifica se a lista est� vazia
    printf("\nO processo %s esta sendo executado!\nSeu tempo diminuiu de %i",(*prior)->rotulo,(*prior)->tempo);
    (*prior)->tempo -= quantum; // Reduz o tempo total do processo de acordo com o quantum

    if ((*prior)->tempo <= 0){ // Remove o processo da lista se o tempo do processo zerar
      printf(" para 0\n\n");
      printf ("O processo %s da lista de %s foi finalizado!\n\n", (*prior)->rotulo, (*prior)->tipo_lista);
      remover (prior);
    }
    else{ /* Se o tempo n�o zerar realiza a c�pia das informa��es do processo para uma vari�vel auxiliar e insere
      novamente na lista de prioridades na posi��o correta com prioridade reduzida em 1*/
      printf(" para %i\n\n",(*prior)->tempo);
      struct processo *aux = malloc(sizeof(struct processo));
      aux->prioridade = (*prior)->prioridade;
      aux->id = (*prior)->id;
      aux->tempo = (*prior)->tempo;
      strcpy(aux->tipo_lista, (*prior)->tipo_lista);
      strcpy(aux->rotulo, (*prior)->rotulo);
      remover (prior); //Remove o processo da lista

      if (aux->prioridade >= 1) // Reduz a prioridade em 1 se prioridade >= 1
      {
        printf("Sua prioridade foi diminuida.Passou de %i ",aux->prioridade);
        aux->prioridade--;
        printf("para %i\n\n",aux->prioridade);
      }
      inserir(prior, aux->prioridade, aux->id, aux->tempo, 0 , aux->rotulo); // Insere novamente na lista
      free(aux);
    }
  }
}

// fun��o que executa um processo que esta na lista pelo metodo RoundRobin
void executa_rr (struct processo **rr, struct processo **disp){
  int quantum = 5; //Defini��o do valor do quantum

  if(gerar_interrupcao()){ // Chamada da fun��o que decide se ocorre interrup��o
    fazer_interrupcao(rr , disp); // Realiza��o da interrup��o
  }
  else if (!(*rr == NULL)){ // Verifica se a lista est� vazia
    printf("\nO processo %s esta sendo executado utilizando o processo de Round-Robin.\nSeu tempo diminuiu de %i",(*rr)->rotulo,(*rr)->tempo);

    (*rr)->tempo -= quantum; // Reduz o tempo total do processo de acordo com o quantum

    if((*rr)->tempo <= 0){ // Remove o processo da lista se o tempo do processo zerar
      printf(" para 0\n\n");
      printf ("O processo %s da lista %s foi finalizado!\n\n", (*rr)->rotulo, (*rr)->tipo_lista);
      remover (rr); // Remove o processo da lista
    }
    else{
      printf(" para %i\n\n",(*rr)->tempo);
      struct processo *aux= malloc(sizeof(struct processo));
      aux->prioridade = (*rr)->prioridade;
      aux->id = (*rr)->id;
      aux->tempo = (*rr)->tempo;
      strcpy(aux->tipo_lista, (*rr)->tipo_lista);
      strcpy(aux->rotulo, (*rr)->rotulo);
      remover(rr);
      inserir(rr, aux->prioridade, aux->id, aux->tempo, 1 , aux->rotulo); // Insere novamente no final da lista
      free(aux);
    }
  }
}

// fun��o que alterna a execu��o das duas filas pronto, executando 4x a lista de prioridades e 1x a lista pelo metodo rr
void alterna_pronto (struct processo **rr, struct processo **prior, struct processo **disp, int cont_exec){
  if ((*prior == NULL) && (*rr == NULL) && (*disp == NULL))
    printf ("\nTodas as filas estao vazias! Gere mais programas\n\n");
  else if ((*prior == NULL) && (*rr == NULL) && (*disp != NULL))
    printf ("\nTodos os processos estao na fila de dispositivos!\n\n");

  if ((!(*rr == NULL) && cont_exec % 4 == 0) || (*prior == NULL))
    executa_rr(rr,disp);
  else
    executa_prior(prior,disp);
}

// fun��o que permite a cria��o da interface de carregamento, como aparece "CRIANDO PROGRAMA" e "IMPRIMINDO" usando a fun��o sleep
void interface_loading(int opcao)
{
  int i;
  if (opcao ==1)
  {
    for(i=0;i < 3;i++)
    {
      printf(". ");
      Sleep(225);
    }
    printf(" CRIANDO PROGRAMA ");
    for(i=0;i < 3;i++)
    {
      printf(". ");
      Sleep(225);
    }
  }
  else if(opcao ==2)
  {
    for(i=0;i < 3;i++)
    {
      printf(". ");
      Sleep(300);
    }
    printf(" IMPRIMINDO ");
    for(i=0;i < 3;i++)
    {
      printf(". ");
      Sleep(300);
    }
    printf("\n");
  }
}

int main(void) {
  int op = 0;
  int id_programa = 0, id_processo = 0;
  int cont_inter = 0;
  int cont_exec = 0;
  char c; // Vari�vel auxiliar para tratamento de entradas n�o desejadas no scanf
  struct processo *disp = NULL;
  struct processo *rr = NULL, *prior = NULL;
  printf("BEM VINDO AO PROGRAMA\n\n");
  do {
    puts("1 - Gerar programa"); //opcoes que o programa tem
    puts("2 - Imprimir listas");
    puts("3 - Executar processos");
    puts("4 - Sair");
    printf("Insira a opcao desejada: ");
    do {
      scanf("%d", &op); //leitura da opcao selecionada pelo usuario
      // Limpa o resto da entrada n�o lida do buffer para evitar erros
      while((c = getchar()) != '\n' && c != EOF);
    } while(op < 1 && op > 4);
    printf("\n");

    switch (op) { // switch case para separar as opcoes do usuario
      case 1: // se o usuario digitou 1,ser� gerado um novo programa com varios processos
        interface_loading(1);
        cria_programa(&id_programa, &id_processo, &rr, &prior);
        printf("\nPrograma criado com sucesso!\n\n");
        break;
      case 2: // se o usuario digitou 2,ser� impresso todos os processos
        interface_loading(2);
        printf ("--------------\n");
        puts("Abaixo se encontra os processos que estao na FILA PRONTO separados em 2 estruturas:\n");
        puts("\nFILA PELO METODO RR(ROUND-ROBIN):");
        imprime_lista(rr); //imprimindo a lista pelo metodo rr
        printf ("--------------\n");
        puts("FILA DE PRIORIDADE:");
        imprime_lista(prior); //imprimindo a lista prioridade
        printf ("--------------\n");
        puts("Abaixo se encontra os processos que estao na FILA DE DISPOSITIVO:");
        imprime_lista(disp); //imprimindo a lista de dispositivos
        printf ("--------------\n");
        break;
      case 3: // se o usuario digitou 3, o pr�ximo processo ser� executado
        cont_exec++;
        cont_inter++;
        alterna_pronto (&rr, &prior, &disp, cont_exec);
        // A Cada 5 Unidades o Primeiro Elemento da lista Dispositivo Volta a sua Lista de Origem (prioridade ou rr)
        if((cont_inter % 4) == 0) {
          retornar_processoDisp(&rr , &prior, &disp); //MUDEI DE 5 PRA 4
        }
        break;
      case 4: //opcao sair do programa
        printf("Encerrando o programa");
        break;
      default:
        printf("\nOpcao errada!Digite novamente.\n\n");
      break;
    }
  } while(op != 4);

  return 0;
}
