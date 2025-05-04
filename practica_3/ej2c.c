#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N_FILOSOFOS 5
#define ESPERA 5000000
sem_t sem;

pthread_mutex_t tenedor[N_FILOSOFOS];

pthread_mutex_t * izq(int i) { return &tenedor[i]; }
pthread_mutex_t * der(int i) { return &tenedor[(i+1) % N_FILOSOFOS]; }

void pensar(int i)
{
    printf("Filosofo %d pensando...\n", i);
    usleep(random() % ESPERA);
}

void comer(int i)
{
    printf("Filosofo %d comiendo...\n", i);
    usleep(random() % ESPERA);
}

void tomar_tenedores(int i)
{   sem_wait(&sem);
    pthread_mutex_lock(der(i));
    pthread_mutex_lock(izq(i));
}

void dejar_tenedores(int i)
{
    pthread_mutex_unlock(der(i));
    pthread_mutex_unlock(izq(i));
    sem_post(&sem);
}

void * filosofo(void *arg)
{

    int i = arg - (void*)0;
    while (1) {
        tomar_tenedores(i);
        comer(i);
        dejar_tenedores(i);
        pensar(i);
    }
}

int main()
{
    pthread_t filo[N_FILOSOFOS];
    sem_init(&sem, 0, N_FILOSOFOS - 1);
    int i;
    for (i = 0; i < N_FILOSOFOS; i++) {
        pthread_mutex_init(&tenedor[i], NULL);
    }
    for (i = 0; i < N_FILOSOFOS; i++){
        pthread_create(&filo[i], NULL, filosofo, i + (void*)0);
    }
    pthread_join(filo[0], NULL);
    return 0;
}

// a- El programa puede terminar en deadlock ya que hay un caso específico de ejecución
// en el que cada filósofo toma el tenedor a su derecha y todos quedan esperando a tomar el de su izquierda,
// pero no puede pues cada uno de estos ya está tomado. Como todos se encuentran bloqueados esperando un recurso
// que posee el siguiente a su izquierda, se produce un ciclo de espera circular.

// b- Funciona la solución de los filosofos y esta es la demostración:
// Supongamos que la solución no funciona, es decir, se produce un deadlock en algun momento.
// Como hay un deadlock, se tenemos que se cumplen estas 4 condiciones:
// 1- Hay exclusión mutua, lo cual es verdadero pues se estan usando mutex.
// 2- Hay hold and wait, lo cual es verdadero pues si estamos en un deadlock, es porque hay al menos
// un filosofo que tomó un tenedor y está esperando a que el de al lado lo libere.
// 3- No preemption, es verdadero pues los filosofos no pueden sacarle el tenedor al otro.
// 4- Espera circular: si hay una espera circular, es porque cada filosófo tiene en posesión un tenedor pero está esperando
// a que el que está a su lado lo suelte.
// Hay dos casos:
// *todos estan esperando el tenedor que está a su derecha (y además tienen el tenedor de su izquierda): pero esto es absurdo, pues todos comienzan solicitando el tenedor de la derecha,
// pero entonces no puede ser que estén esperando el tenedor de la derecha teniendo el de la izquierda, absurdo.
// *todos estan esperando el tenedor de su izquierda: en particular, el filosofo zurdo está esperando el tenedor de su izquierda, por lo tanto ya posee el tenedor de la derecha,
// pero esto es absurdo, no puede ser que haya tomado primero el de la derecha siendo zurdo.
// Estos aburdos indican que no puede haber una espera circular. Luego no puede haber deadlock.
// Q.E.P.D