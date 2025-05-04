#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define M 6
#define N 6
#define SZ 6

/*
 * El buffer guarda punteros a enteros, los
 * productores los consiguen con malloc() y los
 * consumidores los liberan con free()
 */
int *buffer[SZ];

int ultimo = -1;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t no_full;
pthread_cond_t no_empty;

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
		
		pthread_mutex_lock(&mutex);

		while (ultimo >= SZ-1) {
			pthread_cond_wait(&no_full, &mutex);
		}

		int *p = malloc(sizeof *p);
		*p = random() % 100;
		printf("Productor %d: produje %p->%d\n", id, p, *p);
		enviar(p);
		pthread_cond_signal(&no_empty);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

void * cons_f(void *arg)
{
	int id = arg - (void*)0;
	while (1) {

		pthread_mutex_lock(&mutex);

		while (ultimo < 0) {
			pthread_cond_wait(&no_empty, &mutex);
		}

		int *p = recibir();
		printf("Consumidor %d: obtuve %p->%d\n", id, p, *p);
		free(p);
		pthread_cond_signal(&no_full);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main()
{
	pthread_t productores[M], consumidores[N];
	int i;

	pthread_cond_init(&no_full, NULL);
	pthread_cond_init(&no_empty, NULL);

	for (i = 0; i < M; i++)
		pthread_create(&productores[i], NULL, prod_f, i + (void*)0);

	for (i = 0; i < N; i++)
		pthread_create(&consumidores[i], NULL, cons_f, i + (void*)0);

	pthread_join(productores[0], NULL); /* Espera para siempre */
	return 0;
}
