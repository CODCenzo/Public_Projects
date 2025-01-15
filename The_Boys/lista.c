// TAD lista de números inteiros
// Carlos Maziero - DINF/UFPR, Out 2024
//
// Implementação do TAD - a completar
//
// Implementação com lista encadeada dupla não-circular

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

struct lista_t *lista_cria ()
{
	struct lista_t *lst = (struct lista_t *)malloc(sizeof(struct lista_t));

	if (!lst)
		return NULL;

	lst->prim = NULL;
	lst->ult = NULL;
  lst->tamanho = 0;

	return lst;
}

struct lista_t *lista_destroi (struct lista_t *lst)
{
  struct item_t *aux;
  
	if (!lst)
		return NULL;

  while (lst->tamanho != 0)
  {
    aux = lst->prim->prox;
    free(lst->prim);

    lst->prim = aux;
    lst->tamanho--;
  }

  free(lst);
  
  return NULL;
}

int lista_insere (struct lista_t *lst, int item, int pos)
{
  if (!lst)
    return -1;

  struct item_t *nodo = (struct item_t *)malloc(sizeof(struct item_t));
  struct item_t *aux;
  int i = 0;

  if (!nodo)
    return -1;

  nodo->valor = item;

  //Insere na fila vazia
  if (lst->tamanho == 0)
  {
    lst->prim = nodo;
    lst->ult = nodo;

    nodo->ant = NULL;
    nodo->prox = NULL;
  }
  else if (lst->tamanho != 0)
  {
    //Insere no começo
    if (pos == 0)
    {
      aux = lst->prim;
      aux->ant = nodo;

      nodo->ant = NULL;
      nodo->prox = aux;

      lst->prim = nodo;
    }
    //Insere no fim da lista
    else if (pos > lst->tamanho || pos == -1)
    {
      aux = lst->ult;
      aux->prox = nodo;

      nodo->ant = aux;
      nodo->prox = NULL;

      lst->ult = nodo;
    }
    //Insere no meio
    else
    {
      aux = lst->prim;
      while (i < pos && aux->prox != NULL)
      { 
        aux = aux->prox;
        i++;
      }

      nodo->ant = aux->ant;
      nodo->prox = aux;

      aux->ant->prox = nodo;
      aux->ant = nodo;
    }
  }

  return lst->tamanho ++;
}

int lista_retira (struct lista_t *lst, int *item, int pos)
{
  if ((!lst) || (!lst->tamanho))
    return -1;

  struct item_t *aux;
  int i = 0;

  if (lst->tamanho == 1)
  {
    *item = lst->prim->valor;
    free(lst->prim);
    lst->prim = NULL;
    lst->ult = NULL;
  }
  else
  {
    //Retira do começo
    if (pos == 0)
    {
      aux = lst->prim->prox;
      aux->ant = NULL;
      *item = lst->prim->valor;
      free(lst->prim);
      lst->prim = aux;
    }
    //retira do final
    else if (pos >= lst->tamanho || pos == -1)
    {
      aux = lst->ult->ant;
      aux->prox = NULL;
      *item = lst->ult->valor;
      free(lst->ult);
      lst->ult = aux;
    }
    //Retira do meio
    else
    {
      aux = lst->prim;
      while (i < pos && aux->prox != NULL)
      { 
        aux = aux->prox;
        i++;
      }
      aux->ant->prox = aux->prox;
      aux->prox->ant = aux->ant;
      *item = aux->valor;
      free(aux);
    }
  }
  return lst->tamanho--;
}

int lista_consulta (struct lista_t *lst, int *item, int pos)
{
  if ((!lst) || (!lst->tamanho))
    return -1;

  struct item_t *aux;
  int i = 0;

  aux = lst->prim;
  
  if (pos >= lst->tamanho)
    return -1;

  if (pos == -1)
    *item = lst->ult->valor;
  else
  {
    while (i != pos && aux->prox != NULL)
    {
      aux = aux->prox;
      i++;
    }
    *item = aux->valor;
  }

  return lst->tamanho;
}

int lista_procura (struct lista_t *lst, int valor)
{
  if ((!lst) || (!lst->tamanho))
    return -1;

  struct item_t *aux;
  int i = 0;

  aux = lst->prim;

  while (aux != NULL)
  {
    if (aux->valor == valor)
      return i;

    aux = aux->prox;
    i++;
  }

  return -1;
}

int lista_tamanho (struct lista_t *lst)
{
  if (!lst)
    return -1;
  
  return lst->tamanho;
}

void lista_imprime (struct lista_t *lst)
{
  if (lst != NULL && lst->tamanho != 0)
  {
    struct item_t *aux;
    aux = lst->prim;

    while (aux != NULL)
    {
      printf("%d ", aux->valor);
      aux = aux->prox;
    }
  }
}