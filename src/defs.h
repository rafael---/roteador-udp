/*
  Desenvolvido por: Rafael Hengen Ribeiro
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

/* DEBUG MODE. Descomentar a linha abaixo para exibir informações sobre a matriz de distancia */
/* #define DEBUG 1 */

#define MAX         15 /* Custo máximo de um roteador ao seu vizinho */
#define MAX_NODES   7 /* Número máximo de nodos permitidos */
#define INFINITO    (MAX * MAX_NODES + 1) /* Custo INFINITO. Valor impossível de ser obtido, informa que o enlace n está acessivel */
#define INTERVALO   2 /* Intervalo entre envios de vetor-distancia, em segundos */
#define OFFLINE     (4.0*INTERVALO) /* Se um vizinho não envia um vetor-distancia há 4*INTERVALO segundos, é considerado OFFLINE */
#define TENTATIVAS  3 /* Tentativas de envio de uma mensagem */
#define TIMEOUT     3 /* Tempo de expiração de uma confirmação, em segundos. Após esse tempo uma nova mensagem é enviada */

/*
  Verifica qual é a menor distancia até um nodo
*/
#define menor_distancia(x) ((vizinho[(x)] >= 0 && vizinho[(x)] < dist[(x)].distancia)? vizinho[(x)] : dist[(x)].distancia)
/*
  Verifica qual é o próximo salto para chegar em um nodo
*/
#define next_hop(x) ((vizinho[(x)] >= 0 && vizinho[(x)] < dist[(x)].distancia)? x : dist[(x)].prox_salto)

/*
  Estrutura das mensagens enviadas
*/
struct mensagem {
  unsigned int cod; /* Identificador único */
  char type; /* Tipo da mensagem. 'D'- Dados. 'C' - Confirmação. 'E' - Erro. 'S' - Estado do enlace (vetor-distancia) */
  short tentativa;
  short saltos;
  short fonte, destino; /* Identificador do roteador fonte e do destino */
  char msg[255];
};

typedef struct _l
{
    struct mensagem msg;
    struct _l *ant, *prox;
} nodo;

/*
  Matriz de distancias
*/
struct distancia {
  int distancia;
  int prox_salto;
};

/* Fila (FIFO) de mensagens aguardando reenvio */
typedef struct
{
    nodo *inicio, *fim;
    pthread_mutex_t lock; /* Mutex. Acesso à fila é exclusivo*/
    int tam;
} fila;

/* Variáveis globais */
int id; /* ID do roteador */
unsigned short ports[MAX_NODES+1]; /* Mapeamento de portas */
char ips[MAX_NODES+1][16]; /* Mapeamento de endereços IP */

/*
  Se um roteador x é vizinho então vizinho[x] = Custo
  Senão, vizinho[x] = -1
*/
int vizinho[MAX_NODES+1];
/* Última atualização do enlace vizinho */
clock_t last_update[MAX_NODES+1];

/*
  Código atual da mensagem.
  Toda mensagem tem um identificador único
*/
int cod;

/* Distância de id a todos - Matriz de distancias */
struct distancia dist[MAX_NODES+1];

/*
  Vetores distancia recebidos
  vetor_distancia[i] contem todas as distancias recebidas de i
*/
int vetor_distancia[MAX_NODES+1][MAX_NODES+1];

fila *fila_envio;
