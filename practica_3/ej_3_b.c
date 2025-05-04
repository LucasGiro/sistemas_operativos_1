#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "cstack.h"



int main() {

    /*int *numbers = malloc(sizeof(int)*10);
    numbers[0] = 2;
    numbers[1] = 1;
    numbers[2] = 5;
    numbers[3] = 7;
    numbers[4] = 4;

    CStack stack = cstack_create();
    cstack_push(stack, &numbers[0]);
    cstack_push(stack, &numbers[1]);
    cstack_push(stack, &numbers[2]);
    cstack_push(stack, &numbers[3]);
    cstack_push(stack, &numbers[4]);

    int *tope = cstack_pop(stack);
    printf("tope: %d\n", *tope);
    tope = cstack_pop(stack);
    printf("tope: %d\n", *tope);
    tope = cstack_pop(stack);
    printf("tope: %d\n", *tope);
    tope = cstack_pop(stack);
    printf("tope: %d\n", *tope);
    tope = cstack_pop(stack);
    printf("tope: %d\n", *tope);
    cstack_destroy(stack);
    free(numbers);*/

    int i = 0;
    printf("%d\n", i++);

    return 0;
}