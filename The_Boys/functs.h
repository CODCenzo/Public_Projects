
#ifndef FUNCTS
#define FUNCTS


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

#define COORD_MAX_X (N_TAMANHO_MUNDO -1)
#define COORD_MAX_Y (N_TAMANHO_MUNDO -1) //nao tenho certeza

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

struct evento_t {
    int tempo;  /* tempo logico do evento */
    int tipo;   /* inteiro para diferenciar os diferentes tipos de evento */
    int dado_1;  /* um inteiro para guardar dados do evento */
    int dado_2;  /* um inteiro para guardar dados do evento */
};

struct coordenada
{
  int x;
  int y;
};

struct heroi_t
{
  int id_heroi;
  struct cjto_t *habilidades;
  int paciencia;
  int velocidade;
  int exp;
  int base_heroi;
  int status; //1 = vivo, 0 = morto
};

struct base_t
{
  int id_base;
  int lotacao;
  int fila_max;
  int n_missoes;
  struct cjto_t *presentes;
  struct lista_t *fila_espera;
  struct coordenada local;
};


struct missao_t
{
  int id_missao;
  struct cjto_t *habilidades;
  int perigo;
  struct coordenada local;
  int tentativas;
  int completas;
  int min_tentativas;
  int max_tentativas;
};


struct mundo_t
{
  struct heroi_t *vet_herois;
  struct base_t *vet_bases;
  struct missao_t *vet_missoes;
  struct coordenada tamanho_mundo; //Definir coordenadas maximas do mundo
  int tempo;
  struct fprio_t *lef;
  long int ev_tratados;
};

struct candidato_missao
{
  int id_base;
  int dist_missao;
  struct cjto_t *habilidades_base;
  int status; //1 apto, 0 não apto
  
};

int aleat(int min, int max);

int dist_cartesiana(struct coordenada a, struct coordenada b);

void insertionSort_dist( struct candidato_missao *candidato, int tam);

int inicializa_heroi (struct heroi_t heroi[]);

void destroi_herois(struct heroi_t herois[]);

int inicializa_base (struct base_t base[]);

void destroi_bases(struct base_t bases[]);

int inicializa_missao (struct missao_t missao[]);

void destroi_missoes (struct missao_t missoes[]);

int chega(struct mundo_t *mundo, int tempo, int heroi, int base);

void avisa(struct mundo_t *mundo, int tempo, int base);

int entra(struct mundo_t *mundo,  int heroi, int base, int tempo);

void sai(struct mundo_t *mundo,  int heroi, int base, int tempo);

void viaja(struct mundo_t *mundo,  int heroi, int destino, int tempo);

void preenche_candidatos(struct mundo_t *mundo,  struct candidato_missao bases_cand[]);

void missao(struct mundo_t *mundo, int missao, int tempo);

int eventos_iniciais (struct mundo_t *mundo);

int loop_de_eventos(struct mundo_t *mundo);

void relatorio(struct mundo_t *mundo);

struct evento_t *cria_evento(int tempo, int tipo, int dado_1, int dado_2);

struct cjto_t *une_habilidades_base(struct mundo_t *mundo, struct base_t *b);

void completa(struct mundo_t *mundo, int missao, int base, int tempo);

void morre(struct mundo_t *mundo,  int tempo, int heroi, int base);

#endif