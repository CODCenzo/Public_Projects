#ifndef functs
#define functs


int* criar_vetor(int v[], int tamanho);

void destroi_vetor(int *v);

void preenche_vet (int v[], int tamanho);

void shellSort(int arr[], int size);

void trocar(int* a, int* b);

int particionar(int vetor[], int baixo, int alto);

void quickSort(int vetor[], int baixo, int alto);

void cocktailShakerSort(int vetor[], int tamanho);

int busca_Bin(int vetor[], int tamanho, int chave);

int pesq_sequencial(int vetor[], int tamanho, int chave);

void imprime_vetor(int *v);

#endif