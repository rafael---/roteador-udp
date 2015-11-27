#include "defs.h"

/* - Arquivo - rec.c - */
void recalcular_distancias(void); /* Recalcula as distancias a partir dos vetores-distancia recebidos */

 /* - Arquivo - status.c - */
 char status(int ); /* Estado de um enlace. 1 ONLINE (vizinho). 0 OFFLINE (vizinho). -1 DESCONHECIDO */
 void send_state(int ); /* Enviar o estado do enlace e vetor-distancia */

 /* - Arquivo - threads.c - */
 void * thread_send_state(void * ); /* Thread que envia o vetor-distancia a cada INTERVALO segundos */
 void * confirmar(void * ); /* Thread para confirmação de uma mensagem */
 void * loop_rec(void *); /* Loop infinito para receber mensagens e processá-las */

 /* - Arquivo - send.c -

  send_msg - Função genérica para o envio de mensagens
    Argumentos:
      msg -> Estrutura da mensagem a ser enviada
      nh -> Next-Hop. Identificador do próximo salto
      tipo -> Tipo da mensagem
      tentativa -> Número da tentativa
 */
void send_msg(struct mensagem msg, int nh, char tipo, int tentativa);
void source(void); /* Thread que fica esperando um comando do usuário, send por exemplo */

 /* Arquivo - main.c */
 void help(); /* Mostra um menu de ajuda */
 void add_enlaces(const char * ); /* Adiciona os enlaces a partir de um arquivo */
 int main(int argc, char ** argv); /* Função principal */

/* - Arquivo - fila.c */
void inserir(struct mensagem msg); /* Adiciona uma mensagem ao fim da fila de reenvio */
Fila * inicializar(); /* Inicializa a fila  de reenvio */
char fila_vazia(); /* Verifica se a fila está vazia. */
struct mensagem remover(); /* Remove da fila a mensagem mais antiga */
