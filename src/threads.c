#include "func.h"

extern int vizinho[MAX_NODES+1];
extern fila *fila_envio;
extern int id;
extern int cod;

void * thread_send_state(void * v)  {
  int i;

  while(1)  {
    for(i = 0; i < MAX_NODES+1; i++) {
      if(vizinho[i] > 0)  {
        #ifdef DEBUG
          printf("Enviando status para %d", i);
        #endif
        send_state(i);
      }
    }

    sleep(INTERVALO); /* Bloqueia a thread por INTERVALO segundos */
  }
}

void * loop_rec(void * v){
  struct sockaddr_in si_me, si_other;

  int s, i , recv_len;
  socklen_t slen = sizeof(si_other);

  struct mensagem msg;
  struct mensagem reply;

  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
  {
    fprintf(stderr, "Erro ao inicializar socket!\n");
    return NULL;
  }

  memset((char *) &si_me, 0, sizeof(si_me));

  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(ports[id]);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1) {
    fprintf(stderr, "Erro no socket\n");
    return NULL;
  }

  while(1)
  {
    /* Aguarda o recebimento de alguma mensagem */
    if ((recv_len = recvfrom(s, &msg, sizeof(struct mensagem), 0, (struct sockaddr *) &si_other, &slen)) == -1) {
      fprintf(stderr, "Erro ao receber mensagem!\n");
      exit(0);
    }

    if(msg.type == 'D') {
      if(msg.saltos > MAX_NODES)  {
        int temp = msg.destino;
        msg.type = 'E'; /* Mensagem de retorno de erro. Mensagem está em loop */
        msg.fonte = id;
        msg.destino = msg.fonte;
        msg.fonte = temp;
        msg.saltos = 0;

        send_msg(msg, next_hop(msg.destino), 'E', 0);
        continue;
      }
      if(msg.destino == id) {
        printf("\nPacote recebido de %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Roteador fonte: %d - PCK-id: %d - Msg: \'%s\'\n" , msg.fonte,msg.cod,msg.msg);

        reply.cod = msg.cod;
        reply.msg[0] = '\0';
        reply.destino = msg.fonte;
        reply.fonte = id;
        reply.type = 'C';

        send_msg(reply, next_hop(reply.destino), 'C', 0);
      }
      else  {
        int nh = next_hop(msg.destino);
        printf("Encaminhando pacote de %d para %d, originado em %d\n", id, nh, msg.fonte);
        msg.saltos++;
        if(vizinho[nh] > 0 && status(nh) == 0) {
          int temp = msg.destino;
          msg.destino = msg.fonte;
          msg.fonte = temp;
          send_msg(msg, next_hop(msg.destino), 'E', msg.tentativa);
        }
        else
          send_msg(msg, nh, 'D', msg.tentativa);
      }
    }
    else if(msg.type == 'C')  {
      if(msg.destino == id) {
        printf("\nConfirmação de entrega: pck: %d para %d\n",msg.cod,msg.fonte);
      }
      else  {
        msg.saltos++;
        send_msg(msg, next_hop(msg.destino), 'C', msg.tentativa);
      }
    }
    else if(msg.type == 'E')  {
      if(msg.destino == id) {
        printf("\nErro ao tentar enviar msg %d para o destino %d\n",msg.cod, msg.fonte);

        if(msg.tentativa < TENTATIVAS) {
          int temp = msg.destino;
          msg.destino = msg.fonte;
          msg.fonte = temp;
          msg.tentativa++;
          printf("Tentando novamente.\n");

          send_msg(msg, next_hop(msg.destino), 'D', msg.tentativa);
        }
        else  {
          printf("Mensagem perdida. Tentativas expiradas\n");
        }
      } else  {
        send_msg(msg, next_hop(msg.destino), 'D', msg.tentativa);
      }
    }
    else if(msg.type == 'S')  {
      int qtd_vizinhos = msg.msg[0];

      last_update[msg.fonte] = clock();

      for(i = 1; i < qtd_vizinhos; i++) {
        vetor_distancia[msg.fonte][i] = msg.msg[i];
        #ifdef DEBUG
          printf("%d - %d\n", i, vetor_distancia[msg.fonte][i]);
        #endif
      }

      recalcular_distancias();
    }
  }

  close(s);
  return NULL;
}
