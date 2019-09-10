#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef struct Lista//cria lista
{
	int prioridade , id , tempo;//variaveis inteiras
	char rotulo[2];
	char lista[10];
	struct Lista *prox;
	struct Lista *ant;
}List;
// Inserir
List *Inserir(List *inicio, int prioridade , int id , int tempo , int esc , char rotulo[])//insere elementos em lista 
{

	List *novo,*fim=inicio,*aux;//auxiliares
	novo = (List*)malloc(sizeof(List)); // Alocando memoria
	strcpy(novo -> rotulo , rotulo);//atribui char
	novo -> prioridade = prioridade;//atribui inteiro
	novo -> id = id;//atribui inteiro
	novo -> tempo = tempo;//atribui inteiro
	if(esc == 1)//caso fifo
	{
		strcpy(novo -> lista , "FIFO");
		if(inicio == NULL) // Lista vazia
		{
			novo -> prox = NULL;
			novo -> ant = NULL;
			inicio = novo;
			return inicio; // Como so o inicio foi modificado, retorna-se ele
		}
		else  // Caso não, insere no final
		{
			while(fim->prox != NULL )
			{
				fim=fim->prox;
			}
			novo -> ant = fim;
			fim -> prox = novo;
			novo -> prox = NULL;
			fim = novo;
			return inicio; // 
		}
	}
	else if(esc == 2)//caso prioridade
	{
		strcpy(novo -> lista , "PRIORIDADE");
		if(inicio == NULL) // Lista vazia
		{
			novo -> prox = NULL;
			novo -> ant = NULL;
			return novo;
		}
		else//insere 
		{
			if(novo->prioridade > inicio->prioridade)//caso novo elemento seja maior que inicio,entao insere inicio
			{
				inicio -> ant = novo; 
				novo->prox=inicio;
				novo->ant = NULL;
				return novo;//retorna novo inicio
			}
			else//insere meio; fim
			{
				aux=inicio;
				while(aux->prox != NULL && (aux->prox)->prioridade >= novo->prioridade)//busca onde sera colocado o elemento
				{
					aux=aux->prox;
				}
				novo->prox = aux->prox;//coloc elemento em sua posição
				novo->ant = aux;
				if(aux->prox != NULL)//caso seja no fim
				{
					(aux->prox)->ant = novo;
				}
				aux->prox = novo;
				return inicio;
			}
		}
	}
}
void separar(List **fifo,List **prior,int prioridade,int id,int tempo,char rotulo[])//separa em duas listas
{
	if(tempo >= 7)//caso tempo muito alto(lista fifo)
	{
		*fifo=Inserir(*fifo,prioridade,id,tempo,1,rotulo);	
	}
	else if(tempo < 7)//caso não(lista por prioridade)
	{
		*prior = Inserir(*prior,prioridade,id,tempo,2,rotulo); 
	}
}

int main(void) {
	List *fifo, *prior;//lista prior==prioridade,fifo==first in first out
	fifo = prior = NULL;
	int parada=1;
	char rot[2];
	int id,temp,priori;
	do{//cria n elementos de lista
		printf("digite o id do programa\n");
		scanf("%d",&id);
		getchar();
		printf("digite a prioridade do programa\n");
		scanf("%d",&priori);
		printf("digite o tempo do programa\n");
		scanf("%d",&temp);
		printf("digite o rotulo do programa\n");
		getchar();
		scanf("%[^\n]",&rot);
		getchar();
		separar(&fifo,&prior,priori,id,temp,rot);//manda tudo pra separar
		printf("digite 0\n");
		scanf("%d",&parada);
		getchar();
	}while(parada != 0);

	List *aux1,*aux2;//apartir desta linha é para visualização das listas
	aux1=fifo;
	aux2= prior;
	while(aux1 != NULL)
	{
		printf("id-%d\n",aux1->id);
		printf("prio-%d\n",aux1->prioridade);
		printf("%s\n",aux1->lista);
		printf("temp-%d\n",aux1->tempo);
		aux1=aux1->prox;
	}
	while(aux2 != NULL)
	{
		printf("id-%d\n",aux2->id);
		printf("prio-%d\n",aux2->prioridade);
		printf("%s\n",aux2->lista);
		printf("temp-%d\n",aux2->tempo);
		aux2=aux2->prox;
	}
}
