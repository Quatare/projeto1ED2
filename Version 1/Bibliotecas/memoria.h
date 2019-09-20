// Estrutura representativa dos blocos de memória
typedef struct bloco_memoria {
  struct bloco_memoria *prox, *ant;
  int espaco, ocupado, process_id;
}memoria;
