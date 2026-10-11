#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int arr[] = {1,2,3,4,5,6,7,8,9,10};
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
        int totPari = 0;
        for (int i = 0; i < 10; i++) {
            if (arr[i] % 2 == 0) totPari += arr[i];
        }
        write(fd[1], &totPari, sizeof(totPari));
        exit(0);
    }

    int totDispari = 0;
    for (int i = 0; i < 10; i++) {
        if (arr[i] % 2 == 1) totDispari += arr[i];
    }

    int totBuffer;
    read(fd[0], &totBuffer, sizeof(totBuffer));

    wait(NULL);
    printf("Totale pari: %d\n", totBuffer);
    printf("Totale dispari: %d\n", totDispari);
    printf("Totale complessivo: %d\n", totBuffer + totDispari);


    return 0;
}