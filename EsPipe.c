#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main () {

    int fd[2];
    if (pipe(fd) == -1) {
        fprintf(stderr, "Error pipe\n");
        exit(-1);
    }

    pid_t ritorno = fork();
    if (ritorno == -1) {
        fprintf(stderr, "Error fork\n");
        exit(-2);
    }

    if (!ritorno) {
        sleep(10);
        const int prova = 10;
        write(fd[1], &prova, sizeof(prova));

        exit(0);
    }

    printf("Sono pronto a ricevere Boateng dal figlio..\n");
    int buffer;
    read(fd[0], &buffer, sizeof(buffer));
    printf("%d\n", buffer);
    return 0;
}