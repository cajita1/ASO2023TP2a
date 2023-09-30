
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_THREADS 2
#define CANTIDAD_INICIAL_HAMBURGUESAS 20

int cantidad_restante_hamburguesas = CANTIDAD_INICIAL_HAMBURGUESAS;
int turno = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void region_critica() {
    pthread_mutex_lock(&mutex);
}

void region_nocritica() {
    pthread_mutex_unlock(&mutex);
}

void *comer_hamburguesa(void *tid) {
    int hamburguesas_por_hilo = CANTIDAD_INICIAL_HAMBURGUESAS / NUMBER_OF_THREADS;

    for (int i = 0; i < hamburguesas_por_hilo; i++) {
        while (turno != (int)tid) {
            region_critica();
        }

        if (cantidad_restante_hamburguesas > 0) {
            printf("Hola! Soy el hilo(comensal) %d. Me voy a comer una hamburguesa. Quedan %d.\n", tid, cantidad_restante_hamburguesas);
            cantidad_restante_hamburguesas--;
        } else {
            printf("SE TERMINARON LAS HAMBURGUESAS :( \n");
        }

        turno = 1 - (int)tid;
        region_nocritica();
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUMBER_OF_THREADS];
    int status, i, ret;

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        printf("Hola, soy el hilo principal. Estoy creando el hilo %d\n", i);
        status = pthread_create(&threads[i], NULL, comer_hamburguesa, (void *)i);
        if (status != 0) {
            printf("Algo salió mal al crear el hilo. Recibí el código de error %d\n", status);
            exit(-1);
        }
    }

    for (i = 0; i < NUMBER_OF_THREADS; i++) {
        void *retval;
        ret = pthread_join(threads[i], &retval);
    }

    printf("Todas las hamburguesas se han consumido. Cantidad restante: %d\n", cantidad_restante_hamburguesas);

    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);
}
