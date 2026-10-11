#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_N 20
#define MAX_RANDOM 100
int main() {
    srand(time(NULL));

    int fd1[2], fd2[2];
    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        fprintf(stderr, "Error pipe\n");
        exit(-1);
    }

    pid_t pidB = fork();
    if (pidB == -1) {
            fprintf(stderr, "Error fork\n");
            exit(-2);
        }
    if (pidB == 0) {
        close(fd1[1]);
        close(fd2[0]);

        int n;
        while (read(fd1[0], &n, sizeof(int)) > 0) {
            if (n < 50) continue;
            int quadrato = n * n;
            write(fd2[1], &quadrato , sizeof(int));
        }

        close(fd1[0]);
        close(fd2[1]);
        exit(0);
    }

    pid_t pidC = fork();

    if (pidC == -1) {
        fprintf(stderr, "Error fork\n");
        exit(-3);
    }
    if (!pidC) {
        close(fd1[1]);
        close(fd1[0]);
        close(fd2[1]);
        int tot = 0, cont = 0, n;
        while (read(fd2[0], &n, sizeof(int)) > 0) {
            printf("%d\n", n);
            tot += n;
            cont++;
        }
        printf("Media: %.2f\n", (double)tot / cont);
        close(fd2[0]);
        exit(0);
    }
    close(fd1[0]);
    close(fd2[0]);
    close(fd2[1]);
    for (int i = 0; i < MAX_N; i++) {
        int n = rand() % MAX_RANDOM + 1;
        write(fd1[1], &n, sizeof(int));
    }
    close(fd1[1]);
    wait(NULL);
    wait(NULL);

    return 0;
}