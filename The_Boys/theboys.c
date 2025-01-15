// programa principal do projeto "The Boys - 2024/2"
// Autor: Enzo Casadei Ortega da Costa, GRR 20245611

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "conjunto.h"
#include "fprio.h"
#include "lista.h"
#include "functs.h"

#define T_INICIO         0
#define T_FIM_DO_MUNDO   525600
#define N_TAMANHO_MUNDO  20000
#define N_HABILIDADES    10
#define N_HEROIS    ( N_HABILIDADES * 5) //50
#define N_BASES    (N_HEROIS / 5) //10
#define N_MISSOES (T_FIM_DO_MUNDO / 100) //5256


#define COORD_MAX_X (N_TAMANHO_MUNDO -1)
#define COORD_MAX_Y (N_TAMANHO_MUNDO -1)

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


int main ()
{
  //Inicializando mundo

  struct mundo_t mundo;

  mundo.vet_herois = (struct heroi_t *)malloc(sizeof(struct heroi_t) * N_HEROIS);
  mundo.vet_bases = (struct base_t *)malloc(sizeof(struct base_t) * N_BASES);
  mundo.vet_missoes = (struct missao_t *)malloc(sizeof(struct missao_t) * N_MISSOES);

  mundo.tamanho_mundo.x = COORD_MAX_X;
  mundo.tamanho_mundo.y = COORD_MAX_Y;

  mundo.tempo = 0;
  mundo.ev_tratados = 0;

  srand(0);

    //Inicializando LEF
    if (!(mundo.lef = fprio_cria()))
    {
      fprintf(stderr, "Falha ao criar lef para eventos.\n");
      return 1;
    }


    if (!(inicializa_heroi(mundo.vet_herois))) {
      fprintf(stderr, "Falha inicializando herois.\n");
      return 1;
    }

    if (!(inicializa_base(mundo.vet_bases))) {
      fprintf(stderr, "Falha inicializando bases.\n");
      return 1;
    }

    if (!(inicializa_missao(mundo.vet_missoes))) {
      fprintf(stderr, "Falha inicializando missoes.\n");
      return 1;
    }


    if (!(eventos_iniciais(&mundo))) {
    fprintf(stderr, "Falha ao criar eventos iniciais.\n");
    return 1;
    }

    if (!(loop_de_eventos(&mundo))) {
        fprintf(stderr, "Eventos acabaram antes do fim do mundo.\n");
        return 1;
    }

    

    destroi_herois(mundo.vet_herois);
    destroi_bases(mundo.vet_bases);
    destroi_missoes(mundo.vet_missoes);
    fprio_destroi(mundo.lef);


  return (0) ;
}