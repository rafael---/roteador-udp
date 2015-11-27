#include "defs.h"

extern Fila *fila_envio;

Fila * inicializar(){
    Fila *f = (Fila*) malloc(sizeof(Fila));
    f->n = 0;
    f->inicio = 0;
    pthread_mutex_init(&f->lock, NULL);
    return f;
}

void inserir(struct mensagem msg)
{
    int fim;

    pthread_mutex_lock(&fila_envio->lock);

    if(fila_envio->n == TAM_FILA){
        printf("Fila de reenvio cheia!\n");
        return;
    }

    fim = (fila_envio->inicio + fila_envio->n) % TAM_FILA;
    fila_envio->vet[fim] = msg;
    fila_envio->n++;

    pthread_mutex_unlock(&fila_envio->lock);
}

char fila_vazia(){
    return (fila_envio->n == 0);
}

struct mensagem remover(){
     struct mensagem ret;
     ret.cod = -1;

     if(!fila_vazia())  {
       ret = fila_envio->vet[fila_envio->inicio];
       fila_envio->inicio = (fila_envio->inicio+1) % TAM_FILA;
       fila_envio->n--;
     }

     return ret;
}
