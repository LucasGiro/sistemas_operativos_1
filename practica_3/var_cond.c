#include <stdio.h>
#include<stdlib.h>
#include <semaphore.h>

struct bucket {
    sem_t *mutex;
    sem_t *sem;
};

struct var_cond {
  struct bucket **arr;
  int arr_len;
  int capacidad;
  sem_t mutex;  
};

void var_cond_init(struct var_cond *var) {
    int capacidad = 10;
    var->arr = malloc(sizeof(struct bucket *) * capacidad);
    var->arr_len = 0;
    sem_init(&var->mutex, 0, 1);
}

void var_cond_wait(struct var_cond *var, sem_t *mutex) {
    sem_wait(&var->mutex);
    if (var->arr_len == var->capacidad) {
        var->arr = realloc(var->arr, sizeof(struct bucket *) * var->capacidad * 2);
        var->capacidad = var->capacidad * 2;
    }
    sem_t sem;
    struct bucket *b = malloc(sizeof(struct bucket));
    sem_init(&sem, 0, 0);
    b->sem = &sem;
    b->mutex = mutex;
    var->arr[var->arr_len] = b;
    sem_post(&var->mutex);
    sem_post(&mutex);
    sem_wait(&sem);
    sem_wait(&mutex);
}

void var_cond_signal(struct var_cond *var) {
    sem_wait(&var->mutex);
    if (var->arr_len == 0) {
        sem_post(&var->mutex);
        return;
    }
    struct bucket *b = var->arr[var->arr_len-1];
    sem_post(b->sem);
    var->arr_len--;
    free(b);
    sem_post(&var->mutex);
}

void var_cond_broadcast(struct var_cond *var) {
    sem_wait(&var->mutex);
    if (var->arr_len == 0) {
        sem_post(&var->mutex);
        return;
    }
    for (int i = 0; i < var->arr_len; i++) {
        struct bucket *b = var->arr[var->arr_len-1];
        sem_post(b->sem);
        var->arr_len--;
        free(b);
    }
    sem_post(&var->mutex);
}