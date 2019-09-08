#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>

struct processo {
  char rotulo[100];
  int id, tempo;
  struct processo *prox;
};

void create_program_dir() {
  struct stat estado = {0};

  if (stat("./programs", &estado) == -1) {
    printf("Criando dirtorio \"./programs\"\n");
    mkdir("./programs");
  }
}

void cria_programa(int *id){
  FILE *programa;
  char filename[100];

  create_program_dir();
  sprintf(filename, "programs/Teste%02d.txt", *id);

  programa = fopen(filename, "wb");
  if (programa != NULL) {
    srand(time(NULL));
    fprintf(programa,"Processos: %d\n", rand() % 6 + 1);
    fclose(programa);
    (*id)++;
  } else {
    perror(filename);
    exit(EXIT_FAILURE);
  }
}

void add_pronto(struct processo **list, char processo[], int id, int tempo) {
  struct processo *novo = malloc(sizeof(struct processo));
  strcpy(novo->rotulo, processo);
  novo->id = id;
  novo->tempo = tempo;
  novo->prox = *list;
  *list = novo;
}

void carrega_processos(struct processo **pronto, int *id) {
  char linha[100], process_name[100], dirPrograma[100];
  int qtd_processos = 0, i;
  FILE *programa;
  struct dirent *de;
  DIR *dir = opendir("./programs");

  create_program_dir();

  if (dir == NULL) {
    perror("Erro nao foi possivel abrir o diretorio!");
  } else {
    while ((de = readdir(dir)) != NULL) {
      if (strcmp(de->d_name, ".") && strcmp(de->d_name,"..")) {
        sprintf(dirPrograma, "./programs/%s", de->d_name);
        if ((programa = fopen(dirPrograma, "rb")) == NULL) {
          perror("Falha ao carregar o programa!");
        } else {
          while(fgets(linha, sizeof(linha), programa)) {
            sscanf(linha, "Processos:%d", &qtd_processos);
            for (i = 0; i < qtd_processos; i++) {
              sprintf(process_name, "process_%02d", *id);
              add_pronto(pronto, process_name, *id, rand()%15 + 1);
              (*id)++;
            }
          }
          fclose(programa);
        }
      }
    }
  }

  closedir(dir);
}

void imprime_lista(struct processo *list) {
  struct processo *aux = list;
  while (aux != NULL) {
    printf("Nome: %s\nID: %d\nTempo: %d\n\n", aux->rotulo, aux->id, aux->tempo);
    aux = aux->prox;
  }
}

int main(void) {
  int op = 0;
  int id_programa = 0, id_processo = 0;
  char c;
  struct processo *pronto = NULL;

  do {
    puts("1 - Gerar programa");
    puts("2 - Carregar processos");
    puts("3 - Executar processos");
    puts("0 - Sair");
    printf("Insira a opcao desejada: ");
    do {
      scanf("%d", &op);
      while((c = getchar()) != '\n' && c != EOF);
    } while(op < 0 && op > 3);

    switch (op) {
      case 1:
        cria_programa(&id_programa);
        break;
      case 2:
        carrega_processos(&pronto, &id_processo);
        imprime_lista(pronto);
      default: break;
    }
  } while(op != 0);

  return 0;
}
