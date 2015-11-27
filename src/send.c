#include "func.h"

extern int id;
extern unsigned short ports[MAX_NODES+1];
extern char ips[MAX_NODES+1][16];

void send_msg(struct mensagem msg, int nh, char tipo, int tentativa)
{
  struct sockaddr_in si_other;
  int s, slen=sizeof(si_other);

  if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
  {
    fprintf(stderr, "Erro ao inicializar socket!\n");
    return;
  }

  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;

  msg.tentativa = tentativa;
  msg.saltos = 0;

  msg.type = tipo;

  si_other.sin_port = htons(ports[nh]);

  if(status(nh) == 0) {
    fprintf(stderr, "Roteador %d OFFLINE\n", nh);
    close(s);
    return;
  }

  if (inet_aton(ips[nh], &si_other.sin_addr) == 0)
  {
    fprintf(stderr, "Destino inválido: %d!\n",nh);
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

void source()
{
  int i, s;
  char com[50];
  pthread_t confirmacao;
  struct mensagem msg;

  while(1)
  {
    msg.cod = cod++;
    msg.fonte = id;
    msg.saltos = 0;

    scanf("%s",com);
    if(strcmp(com, "send")) {
      if(!strcmp(com,"help")) {
        help();
        continue;
      } else if(!strcmp(com,"status"))  {
        scanf("%d", &i);
        s = status(i);
        switch(s) {
          case 1: printf("Roteador %d: OK\n",i);
                  break;
          case -1:printf("Roteador %d DESCONHECIDO\n",i);
                  break;
          case 0: printf("Roteador %d OFFLINE\n",i);
                  break;
        }
        continue;
      }
      else if(!strcmp(com,"exit"))  {
        exit(0);
      }
      printf("Comando inválido \'%s\'\n",com);
      continue;
    }
    scanf("%d",&i);
    fflush(stdin);
    fgets(msg.msg, 255, stdin);
    msg.msg[strlen(msg.msg)-1] = '\0';
    msg.destino = i;

    send_msg(msg, next_hop(i), 'D', 0);

    inserir(msg);
    pthread_create(&confirmacao, NULL, confirmar, NULL);
  }
}
