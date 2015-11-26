#include "func.h"

extern int vizinho[MAX_NODES+1];
extern int id;
extern unsigned short ports[MAX_NODES+1];
extern char ips[MAX_NODES+1][16];
extern clock_t last_update[MAX_NODES+1];
extern struct distancia dist[MAX_NODES+1];

char status(int router) {
  clock_t tempo_atual = clock();

  if(router == id)
    return 1;
  if(vizinho[router] < 0)
    return -1;
  if(last_update[router] < 0 || (float)(tempo_atual - last_update[router])/100. >= OFFLINE)  {
    dist[router].distancia = INFINITO;
    return 0;
  }
  return 1;
}

void send_state(int destino)
{
  struct sockaddr_in si_other;
  int s, slen=sizeof(si_other), i;

  struct mensagem msg;

  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
  {
    fprintf(stderr, "Erro ao inicializar socket!\n");
    return;
  }

  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;

  msg.fonte = id;
  msg.destino = destino;
  msg.tentativa = 0;

  msg.type = 'S';

  for(i = 1; i <= MAX_NODES; i++)  {
    if(i != id) {
      msg.msg[i] = (status(i) == 0)? INFINITO : menor_distancia(i);
    }
    else
      msg.msg[i] = INFINITO;
  }

  msg.msg[0] = MAX_NODES;

  si_other.sin_port = htons(ports[destino]);

  if (inet_aton(ips[destino], &si_other.sin_addr) == 0)
  {
    fprintf(stderr, "Destino inválido: %d!\n",destino);
    close(s);
    return;
  }

  if (sendto(s, &msg, sizeof(struct mensagem) , 0 , (struct sockaddr *) &si_other, slen)==-1)
  {
    fprintf(stderr, "Erro ao enviar mensagem!\n");
    close(s);
    return;
  }

  close(s);
}
