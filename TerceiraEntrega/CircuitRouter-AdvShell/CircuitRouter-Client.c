/*
// Projeto SO - exercise 3
// Sistemas Operativos, 2018-19
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 100
#define TEMPLATE 13

int main(int argc, char** argv) {
    char template[TEMPLATE];
    char input[BUFFER_SIZE];
    char buf[BUFFER_SIZE + TEMPLATE];


    char* client_pipe_name;
    int client_pipe;
    char* adv_pipe_name;
    int adv_pipe;
    int message_size;

    if (argc != 2) {
        perror("Invalid arguments\n");
        exit(1);
    }

    strcpy(template, "clientXXXXXX");
    client_pipe_name = mktemp(template);
    if (!strcmp(client_pipe_name, "")) {
        perror("Error creating temporary pipe.\n");
        exit(1);
    }

    client_pipe = mkfifo(client_pipe_name, 0666);
    if(client_pipe != 0) {
        perror("Error creating named pipe.\n");
        exit(-1);
    }

    if ((adv_pipe_name = strdup(argv[1])) == NULL) {
        perror("Error duplicating pipe name.\n");
        exit(1);
    }
    while(1){
        if ((adv_pipe = open(adv_pipe_name, O_WRONLY)) < 0) exit (-1);
        /*Reads from stdin and writes in the adv pipe*/
        
        printf("Escreva aqui: ");
        fgets(input, BUFFER_SIZE, stdin);
        sprintf(buf, "%s %s", client_pipe_name, input);
        message_size = strlen(input) + strlen(client_pipe_name) + 1;
        write(adv_pipe, buf, message_size);
        bzero(buf, BUFFER_SIZE);
        
        close(adv_pipe);

        if ((client_pipe = open(client_pipe_name, O_RDONLY)) < 0) exit (-1);

        while(read(client_pipe, buf, BUFFER_SIZE) < 0){
            if(errno == EINTR)
                continue;
            else{
                perror("Error reading from pipe\n");
                exit(-1);
            }
        }
        
        puts(buf);
        bzero(buf, BUFFER_SIZE);
        close(client_pipe);
    }

    return 0;
}