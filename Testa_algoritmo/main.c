#include <stdio.h>
#include <stdlib.h>
#include "functs.h"

#define MAX 10000


int main()
{
  int vet_original[MAX];
  int tamanho = 1024;
  int escolha;
  int chave;

  preenche_vet(vet_original,tamanho);

  printf("\n");
  printf("Bem vindo ao trabalho pratico de ALG II\n");
  printf("Você pode navegar pelo menu apertando os números antes de cada instrução.\n");


  while (1)
  {
    printf("1. Imprimir parte do vetor \n");
    printf("2. Reiniar o vetor \n");
    printf("3. Realizar busca sequencial \n");
    printf("4. Ordenar usando ShellSort\n");
    printf("\n");
    scanf("%d", &escolha);
    printf("\n");

    switch (escolha)
    {
    case 1:
      imprime_vetor(vet_original);
      printf("\n");
      break;
    
    case 2:
      preenche_vet(vet_original,tamanho);
      printf("\n");
      break;
    
    case 3:
      printf("Isira a chave a ser buscada: ");
      scanf("%d", &chave);
      int resultado = pesq_sequencial(vet_original,tamanho,chave);
      printf("Este valor pode ser encontrado no indice: %d \n", resultado);
      printf("\n");


    default:
      break;
    }











  }
  

  return 0;
}
