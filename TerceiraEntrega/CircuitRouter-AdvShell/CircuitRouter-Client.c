/*
// Projeto SO - exercise 3
// Sistemas Operativos, 2018-19
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
    char template[13];
    char* client_pipe;
    char* adv_pipe_name;

    if (argc != 2) {
        perror("Invalid arguments\n");
        exit(1);
    }

    strcpy(template, "clientXXXXXX");
    client_pipe = mktemp(template);
    if (!strcmp(client_pipe, "")) {
        perror("Error creating temporary pipe.\n");
        exit(1);
    }

    if ((adv_pipe_name = strdup(argv[1])) == NULL) {
        perror("Error duplicating pipe name.\n");
        exit(1);
    }

    if (!(remove(client_pipe))) {
        perror("Error removing client pipe.\n");
        exit(1);
    }
    return 0;
}