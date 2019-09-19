typedef struct processo {
  char rotulo[100], tipo_lista[11];
  int id, tempo, prioridade, espaco;
  struct processo *prox, *ant;
}processo;
