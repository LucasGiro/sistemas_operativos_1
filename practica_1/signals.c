#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>

void handleSignals(int n_signal) {

    if (n_signal == SIGINT) {
        printf("se presionó ctr+c\n");
        signal(SIGINT, SIG_DFL);
    }

    if (n_signal == SIGQUIT) {
        printf("recibi la señal SIGQUIT");
    }

}

int main() {

    void (*signalReturn)(int);
    signal(SIGTSTP, SIG_IGN); // ignorar la señal
    signal(SIGINT, handleSignals);
    signal(SIGQUIT, handleSignals);

    raise(SIGINT);
    printf("todavía no me cerré\n");
    raise(SIGINT);

    return 0;
}