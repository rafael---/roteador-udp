#include "func.h"

extern int vizinho[MAX_NODES+1];
extern int id;
extern int vetor_distancia[MAX_NODES+1][MAX_NODES+1];
extern unsigned short ports[MAX_NODES+1];
extern char ips[MAX_NODES+1][16];
extern struct distancia dist[MAX_NODES+1];

void recalcular_distancias()  {
  int i = 1, j;
  for(; i <= MAX_NODES; i++) {
    if(vizinho[i] < 0)
      continue;
    for(j = 1; j <= MAX_NODES; j++)  {
      if(vizinho[i] + vetor_distancia[i][j] < dist[j].distancia) {
        dist[j].distancia = vizinho[i] + vetor_distancia[i][j];
        dist[j].prox_salto = i;
      }
      else if(vizinho[j] >= 0)  {
        dist[j].distancia = vizinho[j];
        dist[j].prox_salto = j;
      }
      else if(vizinho[i] > 0 && dist[j].prox_salto == 0) {
        dist[j].prox_salto = i;
      }
    }
  }

  #ifdef DEBUG
    printf("Novas distâncias: \n");
    for(i = 1; i < MAX_NODES+1; i++)  {
      if(vizinho[i] >= 0 && dist[i].distancia > vizinho[i])  {
            printf("V%d: %d. NH: %d\n",i,vizinho[i],i);
      }
      else {
        printf("%d: %d. NH: %d\n",i,dist[i].distancia,dist[i].prox_salto);
      }
    }
  #endif /* DEBUG */
}
