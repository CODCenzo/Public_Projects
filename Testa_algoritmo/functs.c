#include <stdio.h>
#include <stdlib.h>
#include "functs.h"


//Funcao para criar um novo vetor
int* criar_vetor(int v[], int tamanho) 
{
  if (v == NULL)
  {
    printf("Vetor base está vazio. \n");
    return NULL;
  }


  int* vet_temp = (int*)malloc(tamanho * sizeof(int));

  if (vet_temp == NULL) 
  {
    printf("Erro ao alocar memória.\n");
    return NULL; 
  }

  //Cria um novo vetor a partir do original
  for (int i = 0; i < tamanho; i++) 
    vet_temp[i] = v[i];

  return vet_temp;
}

void destroi_vetor(int *v)
{
  if (v == NULL)
    printf("Erro ao destruir vetor");

  free(v);
  v = NULL;
}

void preenche_vet (int v[], int tamanho)
{
  //Preenche o vetor com numeros de 0-2048
  srand(0);
  for (int i = 0; i < tamanho; i++)
    v[i] = rand() % 2049;
}

//adicionar mais um meio de selecionar gap
// Shell sort com gap/2
void shellSort(int arr[], int size) {
    // Inicializa o intervalo (gap) como metade do tamanho do vetor
    for (int gap = size / 2; gap > 0; gap /= 2) {
        // Realiza o insertion sort com o gap atual
        for (int i = gap; i < size; i++) {
            int temp = arr[i];
            int j;
            
            // Desloca os elementos do intervalo
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            
            arr[j] = temp;
        }
    }
}


void trocar(int* a, int* b) 
{
  int temp = *a;
  *a = *b;
  *b = temp;
}

// Função para particionar o vetor
int particionar(int vetor[], int baixo, int alto) {
  int pivo = vetor[alto]; // Escolhe o pivô como o último elemento
  int i = (baixo - 1); // Índice do menor elemento

  // Percorre o vetor e coloca os elementos menores que o pivô à esquerda e os maiores à direita
  for (int j = baixo; j < alto; j++) {
      if (vetor[j] <= pivo) {
          i++;
          trocar(&vetor[i], &vetor[j]);
      }
  }
  trocar(&vetor[i + 1], &vetor[alto]); // Coloca o pivô na posição correta
  return (i + 1); // Retorna o índice do pivô
}

// Função principal do Quick Sort
void quickSort(int vetor[], int baixo, int alto) {
    if (baixo < alto) {
        // Particiona o vetor e obtém o índice do pivô
        int pivo = particionar(vetor, baixo, alto);

        // Ordena recursivamente as duas sublistas
        quickSort(vetor, baixo, pivo - 1);
        quickSort(vetor, pivo + 1, alto);
    }
}


// Função para realizar a ordenação Cocktail Shaker Sort
void cocktailShakerSort(int vetor[], int tamanho) {
  int inicio = 0;
  int fim = tamanho - 1;
  int trocou;

  do {
      trocou = 0;
      
      // Passagem da esquerda para a direita (Bubble Sort tradicional)
      for (int i = inicio; i < fim; i++) {
          if (vetor[i] > vetor[i + 1]) {
              trocar(&vetor[i], &vetor[i + 1]);
              trocou = 1;
          }
      }

      // Diminuir o fim, pois o último elemento está ordenado
      fim--;

      // Passagem da direita para a esquerda
      for (int i = fim; i > inicio; i--) {
          if (vetor[i] < vetor[i - 1]) {
              trocar(&vetor[i], &vetor[i - 1]);
              trocou = 1;
          }
      }

      // Aumentar o início, pois o primeiro elemento está ordenado
      inicio++;

  } while (trocou); // Continua enquanto houver trocas

}

int busca_Bin(int vetor[], int tamanho, int chave) {
  int inicio = 0;
  int fim = tamanho - 1;
  int meio;

  while (inicio <= fim) {
      meio = (inicio + fim) / 2;

      // Verifica se a chave está na posição do meio
      if (vetor[meio] == chave) {
          return meio;  // Chave encontrada, retorna o índice
      }
      // Se a chave é menor que o valor no meio, a busca continua na metade inferior
      else if (vetor[meio] > chave) {
          fim = meio - 1;
      }
      // Se a chave é maior que o valor no meio, a busca continua na metade superior
      else {
          inicio = meio + 1;
      }
  }

  return -1;  // Chave não encontrada, retorna -1
}

int pesq_sequencial(int vetor[], int tamanho, int chave) {
    for (int i = 0; i < tamanho; i++) {
        if (vetor[i] == chave) {
            return i;  // Retorna o índice onde o elemento foi encontrado
        }
    }
    return -1;  // Retorna -1 se a chave não for encontrada
}


void imprime_vetor(int *v)
{
  for (int i = 0; i < 20; i++)
  {
    printf("%d ", v[i]);
  }
  printf("\n");
}