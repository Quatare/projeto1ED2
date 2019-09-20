// Estrutura representativa dos processos
typedef struct processo {
  char rotulo[100], tipo_lista[11];	//contem como atributos o rotulo(nome do processo, que tambem leva o programa oriundo),o tipo da lista(lista de prioridades ou pelo metodo round robin)
  int id, tempo, prioridade, espaco; // identificador, tempo necessário de processamento e nivel de prioridade
  struct processo *prox; // ponteiros para o proximo elemento
}processo;
