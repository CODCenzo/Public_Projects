#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "conjunto.h"
#include "fprio.h"
#include "lista.h"
#include "functs.h"

//Limitadores da simulação:
#define T_INICIO         0
#define T_FIM_DO_MUNDO   525600
#define N_TAMANHO_MUNDO  20000
#define N_HABILIDADES    10
#define N_HEROIS    ( N_HABILIDADES * 5) //50
#define N_BASES    (N_HEROIS / 5) //10
#define N_MISSOES (T_FIM_DO_MUNDO / 100) //5256

#define MIN_PACIENCIA 0
#define MAX_PACIENCIA 100
#define MIN_VELOCIDADE 50
#define MAX_VELOCIDADE 5000 //metros/min
#define MIN_HAB_HEROI 1
#define MAX_HAB_HEROI 3
#define MIN_LOTACAO 3
#define MAX_lotacao 10
#define MIN_HAB_MISSAO 6
#define MAX_HAB_MISSAO 10
#define MIN_PERIGO 0
#define MAX_PERIGO 100
#define MIN_LOTACAO 3
#define MAX_LOTACAO 10

#define COORD_MAX_X (N_TAMANHO_MUNDO -1)
#define COORD_MAX_Y (N_TAMANHO_MUNDO -1)

//Tipos para o loop eventos
#define EV_CHEGA 0
#define EV_ESPERA 1
#define EV_DESISTE 2
#define EV_AVISA 3
#define EV_ENTRA 4
#define EV_SAI 5
#define EV_VIAJA 6 
#define EV_MISSAO 7
#define EV_FIM 8
#define EV_MORRE 9


//Outras funções:

int aleat(int min, int max)
{
  int num_aleat = min + rand() % (max - min + 1);

  return num_aleat;
};


int dist_cartesiana(struct coordenada a, struct coordenada b)
{
  float diff_x = (float) a.x - b.x;
  float diff_y = (float) a.y - b.y;

  return (int) sqrt(powf(diff_x, 2) + powf(diff_y, 2));
}

//Inicialização das entidades:

int inicializa_heroi (struct heroi_t *heroi)
{
  int max_hab_heroi;

  for (int i = 0; i < N_HEROIS; i++)
  {
    heroi[i].id_heroi = i;
    heroi[i].exp = 0;
    heroi[i].paciencia = aleat(MIN_PACIENCIA,MAX_PACIENCIA);
    heroi[i].velocidade = aleat(MIN_VELOCIDADE,MAX_VELOCIDADE);
    max_hab_heroi = aleat(MIN_HAB_HEROI,MAX_HAB_HEROI);
    heroi[i].status = 1;
    heroi[i].base_heroi = 0;

    if (!(heroi[i].habilidades = cjto_aleat(max_hab_heroi, N_HABILIDADES)))
      return 0;
  }

  return 1;
};

void destroi_herois(struct heroi_t *herois)
{
    for (int i = 0; i < N_HEROIS; i++)
      cjto_destroi(herois[i].habilidades);
    
    free(herois);
}


int inicializa_base (struct base_t *base)
{
  for(int i = 0; i < N_BASES; i++)
  {
    base[i].id_base = i;
    base[i].fila_max = 0;
    base[i].n_missoes = 0;

    //Fazendo o cast dos aleatorios
    base[i].local = (struct coordenada) {aleat(0, COORD_MAX_X), aleat(0, COORD_MAX_Y)};
    base[i].lotacao = aleat(MIN_LOTACAO, MAX_LOTACAO);

    if (!(base[i].presentes = cjto_cria(50)) || !(base[i].fila_espera = lista_cria()))
      return 0;
    
  }
  return 1;
}

void destroi_bases(struct base_t *bases)
{
  for (int i = 0; i < N_BASES; i++)
  {
    cjto_destroi(bases[i].presentes);
    lista_destroi(bases[i].fila_espera);
  }
  free(bases);
}

int inicializa_missao (struct missao_t *missao)
{
  for (int i = 0; i < N_MISSOES; i++)
  {
    int max_hab_missao = aleat(MIN_HAB_MISSAO,MAX_HAB_MISSAO);

    missao[i].id_missao = i;
    missao[i].local = (struct coordenada) {aleat(0, COORD_MAX_X), aleat(0, COORD_MAX_Y)};
    missao[i].perigo = aleat(MIN_PERIGO,MAX_PERIGO);
    missao[i].tentativas = 0;
    missao[i].completas = 0;
    missao[i].min_tentativas = 0;
    missao[i].max_tentativas = 0;

    if (!(missao[i].habilidades = cjto_aleat (max_hab_missao, N_HABILIDADES)))
      return 0;

  }
  return 1;
};

void destroi_missoes (struct missao_t *missoes)
{
  for (int i = 0; i < N_MISSOES; i++)
    cjto_destroi(missoes[i].habilidades);

  free(missoes);
}

// Eventos:
struct evento_t *cria_evento(int tempo, int tipo, int dado_1, int dado_2)
{
  struct evento_t *novo;
  
  if (!(novo = malloc(sizeof(struct evento_t))))
    return NULL;

  novo->tempo = tempo;
  novo->tipo = tipo;
  novo->dado_1 = dado_1;
  novo->dado_2 = dado_2;

  return novo;
}

//Cria um evento e o insere na lef
#define NOVO_EVENTO(mundo, tipo, tempo, dado_1, dado_2) {\
        struct evento_t *n = cria_evento((tempo), (tipo), (dado_1), (dado_2));\
        fprio_insere((mundo)->lef, n, tipo, tempo);\
    }


int chega(struct mundo_t *mundo, int tempo, int heroi, int base)
{
  struct base_t *b = &mundo->vet_bases[base];
  struct heroi_t *h = &mundo->vet_herois[heroi];

  int tam_fila = lista_tamanho(b->fila_espera);

  if (((cjto_card(b->presentes) < b->lotacao) && (tam_fila == 0)) || (h->paciencia > (10*tam_fila)))
  {
    NOVO_EVENTO(mundo, EV_ESPERA, tempo, heroi, base);
    return 1;
  }

  NOVO_EVENTO(mundo, EV_DESISTE, tempo, heroi, base);
  return 0;
}


void avisa(struct mundo_t *mundo, int tempo, int base)
{
  struct base_t *b = &mundo->vet_bases[base];
  int item_retirado;
  int tam_lista;

  tam_lista = lista_tamanho(b->fila_espera);

  if (tam_lista > b->fila_max)
    b->fila_max = tam_lista;

  while ((cjto_card(b->presentes) < b->lotacao) && (lista_tamanho(b->fila_espera) > 0))
  {
    lista_retira(b->fila_espera, &item_retirado,0);
    cjto_insere(b->presentes, item_retirado);
    printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", tempo, base, item_retirado);

    NOVO_EVENTO(mundo, EV_ENTRA, tempo, item_retirado, base);
  }
}


int entra(struct mundo_t *mundo,  int heroi, int base, int tempo)
{
  struct heroi_t *h = &mundo->vet_herois[heroi];
  int TPB = 15 + (h->paciencia * aleat(1,20));
  NOVO_EVENTO(mundo, EV_SAI, TPB + tempo, heroi, base);

  return TPB + tempo;
}


void sai(struct mundo_t *mundo,  int heroi, int base, int tempo)
{
  int dest = aleat(0, N_BASES - 1);
  cjto_retira(mundo->vet_bases[base].presentes, heroi);

  NOVO_EVENTO(mundo, EV_VIAJA, tempo, heroi, dest);
  NOVO_EVENTO(mundo, EV_AVISA, tempo, 0, base);
}


void viaja(struct mundo_t *mundo,  int heroi, int destino, int tempo)
{
  struct heroi_t *h = &mundo->vet_herois[heroi];
  struct base_t *d = &mundo->vet_bases[destino];
  struct coordenada local_base = mundo->vet_bases[h->base_heroi].local;
  int distancia = dist_cartesiana(local_base, d->local);
  int duracao = distancia / h->velocidade;

  printf("%6d: VIAJA  HEROI %2d BASE %2d BASE %2d DIST %d VEL %d CHEGA %d\n",
        tempo,
        heroi,
        h->base_heroi,
        destino,
        distancia,
        h->velocidade,
        duracao + tempo);

  NOVO_EVENTO(mundo, EV_CHEGA, duracao + tempo, heroi, destino);
}

// //Como ignorar os eventos dps de morto?
void morre(struct mundo_t *mundo,  int tempo, int heroi, int base)
{
  struct heroi_t *h = &mundo->vet_herois[heroi];
  struct base_t *b = &mundo->vet_bases[base];

  cjto_retira(b->presentes, heroi);
  h->status = 0;

  NOVO_EVENTO(mundo, EV_AVISA, tempo, base, 0);
}

//Usada para organizar as bases em relação a distância da missão
struct BaseDistancia 
{
  int base;
  int dist;
};

void missao(struct mundo_t *mundo, int missao, int tempo)
{
  struct BaseDistancia bases[N_BASES];
  struct BaseDistancia ult;
  struct cjto_t *uniao;
  struct missao_t *m = &mundo->vet_missoes[missao];
  int i, j;

  m->tentativas++;

  printf("%6d: MISSAO %d HAB REQ: ", tempo, missao);
  cjto_imprime(m->habilidades);
  printf("\n");

  for (i = 0; i < N_BASES; i++) 
  {
    bases[i] = (struct BaseDistancia) {
    .base = i,
    .dist = dist_cartesiana(mundo->vet_bases[i].local, m->local)};
  }

  //Ordena as bases em um vetor utilizando InsertionSort
  for (i = 1; i < N_BASES; i++) 
  {
    ult = bases[i];
    j = i - 1;
    while (ult.dist < bases[j].dist && j >= 0) 
    {
      bases[j + 1] = bases[j];
      j--;
    }
    bases[j + 1] = ult;
  }
    
  //Acessa as bases e verifica se são aptas para a missão
  for (i = 0; i < N_BASES; i++) 
  {
    uniao = une_habilidades_base(mundo, &mundo->vet_bases[bases[i].base]);

    printf("%6d: MISSAO %d HAB BASE %d: ",
          tempo,
          missao,
          bases[i].base);
    cjto_imprime(uniao);
    printf("\n");


    if (cjto_contem(uniao, m->habilidades) == 1) 
    {
      printf("%6d: MISSAO %d CUMPRIDA BASE %d HEROIS: ",
            tempo,
            missao,
            bases[i].base);
      cjto_imprime(mundo->vet_bases[bases[i].base].presentes);
      printf("\n");

      completa(mundo, missao, bases[i].base, tempo);
      cjto_destroi(uniao);
      return;
    }
    cjto_destroi(uniao);
  }

  printf("%6d: MISSAO %d IMPOSSIVEL\n", tempo, missao);

  if (tempo + 24 * 60 < T_FIM_DO_MUNDO)
    NOVO_EVENTO(mundo, EV_MISSAO, tempo + 24 * 60, missao, 0);

  return;
}

void completa(struct mundo_t *mundo, int missao, int base, int tempo)
{
  struct base_t *b = &mundo->vet_bases[base];
  struct missao_t *m = &mundo->vet_missoes[missao];
  int risco;

  mundo->vet_missoes[missao].completas = 1;
  b->n_missoes++;

  for (int i = 0; i < N_HEROIS; i++)
  {
    struct heroi_t *h = &mundo->vet_herois[i];

    if (cjto_pertence(b->presentes, h->id_heroi) == 1)
    {
      risco = m->perigo / (h->paciencia + h->exp + 1);

      if (risco > aleat(0,30))
      {
        printf("%6d: MORRE  HEROI %2d MISSAO %d\n",
              tempo,
              h->id_heroi,
              missao);
        NOVO_EVENTO(mundo, EV_MORRE, tempo, h->id_heroi, base);
      }
      else
        h->exp++;
    }
  }
}

//Cria um conjunto o as habilidades da base
struct cjto_t *une_habilidades_base(struct mundo_t *mundo, struct base_t *b)
{
  struct cjto_t *uniao_herois = cjto_cria(10);
  struct cjto_t *aux;

  
  for (int i = 0; i < N_HEROIS; i++)
  {
    struct heroi_t *h = &mundo->vet_herois[i];

    if (cjto_pertence(b->presentes, h->id_heroi) == 1)
    {
      aux = cjto_uniao(h->habilidades,uniao_herois);
      free(uniao_herois);
      uniao_herois = aux;
      // free(aux);
    }
  }

  return uniao_herois;
}


int eventos_iniciais (struct mundo_t *mundo)
{
  struct evento_t *e;
  int tempo;
  int base;
  int heroi;
  int missao;
  int i;

  for (i = 0; i < N_HEROIS; i++)
  {
    tempo = aleat(0,4320);
    base = aleat(0, N_BASES-1);
    heroi = i;

    if (!(e = cria_evento(tempo, EV_CHEGA, heroi, base)))
      return 0;

    fprio_insere(mundo->lef, e, EV_CHEGA, e->tempo);
  }

  for (i = 0; i < N_MISSOES; i++)
  {
    tempo = aleat(0, T_FIM_DO_MUNDO);
    missao = i;

    if (!(e = cria_evento(tempo, EV_MISSAO, missao, 0)))
      return 0;

    fprio_insere(mundo->lef, e, EV_MISSAO, e->tempo);
  }
  
  tempo = T_FIM_DO_MUNDO;

  if (!(e = cria_evento(tempo, EV_FIM, 0, 0)))
    return 0;

  fprio_insere(mundo->lef, e, EV_FIM, e->tempo);
  return 1;
}

//Percorre a LEF realizando os eventos
int loop_de_eventos(struct mundo_t *mundo)
{
    struct evento_t *e;
    int tamanho;
    int lixo_1 = 0;
    int lixo_2 = 0;

    while (1) 
    {
      e = fprio_retira(mundo->lef, &lixo_1, &lixo_2);
      mundo->ev_tratados++;

        switch (e->tipo) 
        {
        case EV_FIM:
            printf("%6d: FIM\n", e->tempo);
            relatorio(mundo);
            free(e);
          return 1;

        case EV_CHEGA:
          if (!(e->dado_1))
            break;

          printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) \n",
                   e->tempo,
                   e->dado_1,
                   e->dado_2,
                   cjto_card(mundo->vet_bases[e->dado_2].presentes),
                   mundo->vet_bases[e->dado_2].lotacao);

          chega(mundo, e->tempo, e->dado_1, e->dado_2);

          break;

        case EV_ESPERA:
          if (!(e->dado_1))
            break;

          tamanho = lista_tamanho(mundo->vet_bases[e->dado_2].fila_espera);
          printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n",
                   e->tempo,
                   e->dado_1,
                   e->dado_2,
                   tamanho);
            
          lista_insere(mundo->vet_bases[e->dado_2].fila_espera, e->dado_1, tamanho + 1);
          NOVO_EVENTO(mundo, EV_AVISA, e->tempo, e->dado_1, e->dado_2);
          break;

        case EV_DESISTE:
            if (!(e->dado_1))
              break;

            tamanho = lista_tamanho(mundo->vet_bases[e->dado_2].fila_espera);
            printf("%6d: DESIST HEROI %2d BASE %d (%2d)\n",
                   e->tempo,
                   e->dado_1,
                   e->dado_2,
                   tamanho);
            NOVO_EVENTO(mundo, EV_VIAJA, e->tempo, e->dado_1, aleat(0, N_BASES - 1));
            break;

        case EV_AVISA:
          // if (!e->dado_1)
          //   break;

            printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA ",
                   e->tempo,
                   e->dado_2,
                   cjto_card(mundo->vet_bases[e->dado_2].presentes),
                   mundo->vet_bases[e->dado_2].lotacao);

            lista_imprime(mundo->vet_bases[e->dado_2].fila_espera);
            printf("\n");
            avisa(mundo, e->tempo, e->dado_2);
            break;

        case EV_ENTRA:
            if (!(e->dado_1))
              break;

            printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n",
                   e->tempo,
                   e->dado_1,
                   e->dado_2,
                   cjto_card(mundo->vet_bases[e->dado_2].presentes),
                   mundo->vet_bases[e->dado_2].lotacao,
                   entra(mundo, e->dado_1, e->dado_2, e->tempo));
            break;

        case EV_SAI:
            if (!(e->dado_1))
              break;

            printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n",
                   e->tempo,
                   e->dado_1,
                   e->dado_2,
                   cjto_card(mundo->vet_bases[e->dado_2].presentes),
                   mundo->vet_bases[e->dado_2].lotacao);
            sai(mundo, e->dado_1, e->dado_2, e->tempo);
            break;

        case EV_VIAJA:
            if (!(e->dado_1))
              break;

            viaja(mundo, e->dado_1, e->dado_2, e->tempo);
            break;

        case EV_MISSAO:
            missao(mundo, e->dado_1, e->tempo);
            break;
          
        case EV_MORRE:
            if(!(e->dado_1))
              break;
        

            morre(mundo, e->tempo, e->dado_1, e->dado_2);
            break;
        }

        free(e);
    }

    return 0;
}

void relatorio(struct mundo_t *mundo)
{

  struct heroi_t *h;
  struct missao_t *m;
  struct base_t *b;
  int completas = 0;
  int tentativas = 0;
  int i;
  int mortos = 0;

  for (i = 0; i < N_HEROIS; i++) 
  {
    h = &mundo->vet_herois[i];
    if (h->status == 1)
      printf("HEROI %2d VIVO PAC %3d VEL %4d EXP %4d HAB [ ",
              i,
              h->paciencia,
              h->velocidade,
              h->exp);
    else{
        printf("HEROI %2d MORTO PAC %3d VEL %4d EXP %4d HAB [ ",
        i,
        h->paciencia,
        h->velocidade,
        h->exp);

        mortos++;}

    cjto_imprime(h->habilidades);
    printf(" ]\n");
  }

  for (i = 0; i < N_BASES; i++)
  {
    b = &mundo->vet_bases[i];
    printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %4d\n",
          i,
          b->lotacao,
          b->fila_max,
          b->n_missoes);
  }


  for (i = 0; i < N_MISSOES; i++) {
      m = &mundo->vet_missoes[i];
      completas += m->completas;
      tentativas += m->tentativas;
  }

  printf("EVENTOS TRATADOS: %7ld \n", mundo->ev_tratados);
  printf("MISSOES CUMPRIDAS %d/%d (%.1f%%)\n",
          completas,
          N_MISSOES,
          (float) completas * 100 / N_MISSOES);

  printf("TENTATIVAS/MISSAO: %.1f\n", (float) tentativas / N_MISSOES);
  printf("TAXA MORTALIDADE: %.1f%%\n", (float) mortos*100 / N_HEROIS);

}