#include <assert.h>
#include <string.h>
#include <pthread.h>

#include "tsp-types.h"
#include "tsp-genmap.h"
#include "tsp-print.h"
#include "tsp-tsp.h"

/* dernier minimum trouvé */
int minimum;

pthread_mutex_t mutex_cuts;
pthread_mutex_t mutex_sol_len;

void init_tsp()
{
    pthread_mutex_init(&mutex_cuts, NULL);
    pthread_mutex_init(&mutex_sol_len, NULL);
}

/* résolution du problème du voyageur de commerce */
int present (int city, int hops, tsp_path_t path)
{
    for (int i = 0; i < hops; i++) {
        if (path [i] == city) {
            return 1;
        }
    }
    
    return 0 ;
}



void tsp (int hops, int len, tsp_path_t path, long long int *cuts, tsp_path_t sol, int *sol_len)
{
  if (len + cutprefix[(nb_towns-hops)] >= minimum) {
      pthread_mutex_lock(&mutex_cuts);
      (*cuts)++ ;
      pthread_mutex_unlock(&mutex_cuts);
      return;
    }
    
    if (hops == nb_towns) {
	    int me = path [hops - 1];
	    int dist = distance[me][0]; // retourner en 0
            if ( len + dist < minimum ) {
		    minimum = len + dist;
            pthread_mutex_lock(&mutex_sol_len);
		    *sol_len = len + dist;
            pthread_mutex_unlock(&mutex_sol_len);
		    memcpy(sol, path, nb_towns*sizeof(int));
		    print_solution (path, len+dist);
	    }
    } else {
        int me = path [hops - 1];        
        for (int i = 0; i < nb_towns; i++) {
            if (!present (i, hops, path)) {
                path[hops] = i;
                int dist = distance[me][i];
                tsp (hops + 1, len + dist, path, cuts, sol, sol_len);
            }
        }
    }
}

