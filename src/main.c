#include "func.h"

extern int vizinho[MAX_NODES+1];
extern int id;
extern unsigned short ports[MAX_NODES+1];
extern char ips[MAX_NODES+1][16];
extern clock_t last_update[MAX_NODES+1];
extern struct distancia dist[MAX_NODES+1];
extern int vetor_distancia[MAX_NODES+1][MAX_NODES+1];
extern Fila *fila_envio;

int main(int argc, char ** argv)
{
  int i;

  pthread_t receiver, state;

  FILE * F;

  if(!(F = fopen("roteador.config", "r")))  {
    fprintf(stderr, "Erro ao tentar abrir arquivo roteador.config\n");
    return 0;
  }

  /* Le as configurações dos roteadores de um arquivo */
  while(fscanf(F, "%d", &i)!=EOF){
    fscanf(F, "%hu %s", &ports[i], ips[i]);
    if(feof(F))
      break;
  }

  if(argc == 1) {
    printf("\tÉ necessário informar o ID do roteador.\n\t\tFormato: %s <id>\n\n",argv[0]);
    return 0;
  }

  if(!sscanf(argv[1], "%d", &id)) {
    printf("É necessário passar um número entre 1 e %d como parâmetro.\n",MAX_NODES);
    return 0;
  }

  if(id > MAX_NODES)  {
    printf("Número de roteador inválido > %d\n", MAX_NODES);
    return 0;
  }
  else if(id == 0)  {
    printf("Número de roteador inválido 0. O número do roteador deve estar entre 1 e %d\n",MAX_NODES);
    return 0;
  }

  add_enlaces("enlaces.config");

  fila_envio = inicializar();

  pthread_create(&receiver, NULL, loop_rec, NULL);
  pthread_create(&state, NULL, thread_send_state, NULL);

  source();

  return 0;
}

void add_enlaces(const char * fname)  {
  FILE * f = NULL;
  int i = 1,j, u, v, C;

  if(!(f = fopen(fname,"r"))) {
    fprintf(stderr, "Erro ao ler arquivo %s\n", fname);
    exit(0);
  }

  for(; i <= MAX_NODES; i++)  {
    vizinho[i] = -1;
    dist[i].distancia = INFINITO;
    dist[i].prox_salto = 0;
    last_update[i] = -1;

    for(j = 1; j <= MAX_NODES; j++)  {
      vetor_distancia[i][j] = INFINITO;
    }
  }

  vizinho[id] = 0;

  while(!feof(f)) {
    fscanf(f, "%d %d %d\n", &u, &v, &C);
    if(u == id || v == id)  {
      if(u == id) {
        vizinho[v] = C;
      } else  {
        vizinho[u] = C;
      }
    }
  }

  fclose(f);
}

void help() {
  puts("O comando help mostra este menu de ajuda.");
  puts("O comando send envia uma mensagem para o roteador destino desejado");
  puts("\tFormato: send <id> mensagem");
  puts("\tExemplos:\n\t\t\tsend 1 Olá!");
  puts("\t\t\tsend 2 Hello router2!");
  puts("\tAs mensagens podem ter até 255 caracteres, incluindo espaços");
  puts("O comando status mostra o status atual de um roteador");
  puts("\tFormato: status <id>");
}
