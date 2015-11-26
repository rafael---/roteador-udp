#include "func.h"

extern fila *fila_envio;

char vazia()  {
  return (fila_envio->tam == 0);
}

void adicionar(struct mensagem msg)
{
  nodo * novo = (nodo*) malloc(sizeof(nodo));
  novo->msg = msg;
  novo->prox = NULL;
  if(fila_envio->fim)
    fila_envio->fim->prox = novo;
  fila_envio->fim = novo;
  if(!fila_envio->inicio)
    fila_envio->inicio = fila_envio->fim;
  fila_envio->tam++;
}

fila * inicializar ()
{
    fila * l =(fila*) malloc(sizeof(fila));
    l->inicio = l->fim = NULL;
    l->tam = 0;
    pthread_mutex_init(&l->lock, NULL);
    return l;
}

nodo * buscar(int msg_id)  {
  nodo * p = fila_envio->inicio;
  while(p != NULL && p->msg.cod != msg_id)
		p = p->prox;
  return p;
}

nodo * dequeue()  {
    nodo * p = fila_envio->inicio;
    if(p == NULL)
        return NULL;

    fila_envio->inicio = fila_envio->inicio->prox;

    if(fila_envio->inicio == NULL) {
        fila_envio->fim = NULL;
    }
    else  {
        fila_envio->inicio->ant = NULL;
     }
    fila_envio->tam--;

    return p;
}

void imprimir()
{
    nodo * p = fila_envio->inicio;
    printf("\nElementos na fila de reenvio\n");
    for(; p; p = p->prox)
        printf("MSG-ID: %d\n",p->msg.cod);
    putchar('\n');
}
