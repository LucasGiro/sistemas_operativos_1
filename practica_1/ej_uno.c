#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main() {

    int *p = malloc(100000);

    free(p);

    FILE *f = fopen("test.txt", "r");

    ungetc('c', f);

    char c = getchar();

    printf("%c %c\n", c, fgetc(f
    ));
    

    fclose(f);

    return 0;
}