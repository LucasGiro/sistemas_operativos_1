#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include<semaphore.h>

#define M 12
#define N 12
#define SZ 16

/*
 * El buffer guarda punteros a enteros, los
 * productores los consiguen con malloc() y los
 * consumidores los liberan con free()
 */
int *buffer[SZ];

int ultimo = -1;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t ocupados;
sem_t lugares;

void enviar(int *p)
{
	buffer[ultimo+1] = p;
	ultimo++;
	return;
}

int * recibir()
{
	int *dato = buffer[ultimo];
	ultimo--;
	return dato;
}

void * prod_f(void *arg)
{
	int id = arg - (void*)0;
	while (1) {

        sem_wait(&lugares);
		
		pthread_mutex_lock(&mutex);

		int *p = malloc(sizeof *p);
		*p = random() % 100;
		printf("Productor %d: produje %p->%d\n", id, p, *p);
		enviar(p);
		sem_post(&ocupados);
        pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

void * cons_f(void *arg)
{
	int id = arg - (void*)0;
	while (1) {

        sem_wait(&ocupados);

		pthread_mutex_lock(&mutex);

		int *p = recibir();
		printf("Consumidor %d: obtuve %p->%d\n", id, p, *p);
		free(p);
		sem_post(&lugares);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main()
{
	pthread_t productores[M], consumidores[N];
	int i;

	sem_init(&ocupados, 0, 0);
    sem_init(&lugares, 0, SZ);

	for (i = 0; i < M; i++)
		pthread_create(&productores[i], NULL, prod_f, i + (void*)0);

	for (i = 0; i < N; i++)
		pthread_create(&consumidores[i], NULL, cons_f, i + (void*)0);

	pthread_join(productores[0], NULL); /* Espera para siempre */
	return 0;
}
