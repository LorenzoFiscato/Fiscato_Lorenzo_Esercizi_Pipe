#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>

#define MAX_STRINGA 100

struct Messaggio {
    char stringa_maiuscola[MAX_STRINGA];
    unsigned int numero_vocali;
};

int isVocale(char c) {
    const char vocali[] = {'a', 'e', 'i', 'o', 'u', '\0'};
    c = tolower(c);
    for (int i = 0; vocali[i] != '\0'; i++)
        if (c == vocali[i]) return 1;
    return 0;
}

int main () {

    int fd1[2];
    int fd2[2];

    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        fprintf(stderr, "Error pipe\n");
        exit(-1);
    }

    pid_t ritorno = fork();
    if (ritorno == -1) {
        fprintf(stderr, "Error fork\n");
        exit(-2);
    }

    if (!ritorno) {
        struct Messaggio messaggio;
        messaggio.numero_vocali = 0;
        char stringa_minuscola[MAX_STRINGA];
        read(fd1[0], stringa_minuscola, MAX_STRINGA);

        for (int i = 0; stringa_minuscola[i] != '\0'; i++) {
            messaggio.stringa_maiuscola[i] = toupper(stringa_minuscola[i]);
            if (isVocale(stringa_minuscola[i])) messaggio.numero_vocali++;
        }

        messaggio.stringa_maiuscola[strlen(stringa_minuscola)] = '\0';
        write(fd2[1], &messaggio, sizeof(struct Messaggio));

        exit(0);
    }

    char stringa_input[MAX_STRINGA];
    struct Messaggio messaggio;

    printf("Padre: Inserisci una stringa di testo: ");
    if (fgets(stringa_input, MAX_STRINGA, stdin) != NULL) {
        stringa_input[strcspn(stringa_input, "\n")] = '\0';
    }

    write(fd1[1], stringa_input, strlen(stringa_input));

    read(fd2[0], &messaggio, sizeof(struct Messaggio));
    wait(NULL);

    printf("%s\n", messaggio.stringa_maiuscola);
    printf("Numero di vocali: %d\n", messaggio.numero_vocali);

    return 0;
}