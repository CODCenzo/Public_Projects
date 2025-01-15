// TAD Fila de prioridades (FPRIO) genérica
// Carlos Maziero, DINF/UFPR, Out 2024
// Implementação com lista encadeada simples

// A COMPLETAR
#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

struct fprio_t *fprio_cria ()
{
	struct fprio_t *fprio = (struct fprio_t *)malloc(sizeof(struct fprio_t));

	if (!fprio)
		return NULL;

	fprio->prim = NULL;
	fprio->num = 0;

	return fprio;
}

int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio)
{

	struct fpnodo_t *nodo = (struct fpnodo_t *)malloc(sizeof(struct fpnodo_t));
	struct fpnodo_t *aux_1;
	struct fpnodo_t *aux_2;


	if ((!nodo) || (!f) || (!item))
  {
    free(nodo);
    return -1;
  }
		
  //Verifica itens iguais
	aux_1 = f->prim;
	while(aux_1 != NULL)
	{
		if (aux_1->item == item)
		{
			free(nodo);
			return-1;
		}
		aux_1 = aux_1->prox;
	}

	nodo->prox = NULL;
	nodo->prio = prio;
	nodo->tipo = tipo;
	nodo->item = item;

	//Inserir item na lista vazia
	if ((f->num == 0) && (f->prim == NULL))
		f->prim = nodo;

	
	//Inserir na primeira posição
	if (nodo->prio < f->prim->prio)
	{
		nodo->prox = f->prim;
		f->prim = nodo;
	}
	else
	{
	  //Inserir no meio e no final;
		aux_1 = f->prim;
		aux_2 = f->prim->prox;

		while((aux_2 != NULL) && (nodo->prio > aux_1->prio))  
		{
			aux_1 = aux_2;
			aux_2 = aux_2->prox;
		}
		//Percorre nodos com mesma prioridade
		if (aux_2 != NULL)
		{
			while(aux_1->prio == aux_2->prio)
			{
			aux_1 = aux_2;
			aux_2 = aux_2->prox;
			}
		}

		//Insere no meio
		if (aux_2 != NULL)
		{
			aux_1->prox = nodo;
			nodo->prox = aux_2;
		}
		else
		{
			//Insere no final
			aux_1->prox = nodo;
			nodo->prox = NULL;
		}
	}

	f->num++;
	return f->num;
}

void *fprio_retira (struct fprio_t *f, int *tipo, int *prio)
{
  struct fpnodo_t *aux_1;

	if (!f)
		return NULL;

  if (f->num == 0 || !tipo|| !prio)
    return NULL;

  aux_1 = f->prim;

  void *item;

  *tipo = aux_1->tipo;
  *prio = aux_1->prio;
  item = aux_1->item;
  
  f->prim = aux_1->prox;
  f->num = f->num-1;

  free(aux_1);

  return item;
}

struct fprio_t *fprio_destroi (struct fprio_t *f)
{
  struct fpnodo_t *aux_1;

	if (!f)
		return NULL;

  while (f->num != 0)
  {
    aux_1 = f->prim->prox;
    free(f->prim->item);
    free(f->prim);

    f->prim = aux_1;
    f->num--;
  }

  free(f);
  
  return NULL;
}

int fprio_tamanho (struct fprio_t *f)
{
  if (!f || f->num < 0)
    return -1;

  return f->num;
}

void fprio_imprime (struct fprio_t *f)
{
  struct fpnodo_t *aux_1;
  aux_1 = f->prim;

  while (aux_1 != NULL)
  {
    printf("(%d %d)", aux_1->tipo, aux_1->prio);
    aux_1 = aux_1->prox;
  }
}