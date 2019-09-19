typedef struct memoria {
  struct memoria *prox, *ant;
  int espaco, ocupado, process_id;
}Mem;
