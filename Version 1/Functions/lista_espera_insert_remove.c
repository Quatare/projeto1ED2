// Lista de espera recebe tudo que a estrutura de processo nova deve ter
void insere_espera(processo **esp, int id, int espa, int tempo, int prioridade, char rotulo[100], char tipo_lista[11]);

// Procura se tem elementos da fila de espera que podem ser inseridos na memoria
void busca_memoria(Mem **memoria,processo **esp);
