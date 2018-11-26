
/*
// Projeto SO - exercise 3
// Sistemas Operativos, 2018-19
*/

#include "lib/commandlinereader.h"
#include "lib/vector.h"
#include "CircuitRouter-AdvShell.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/select.h>


#define COMMAND_EXIT "exit"
#define COMMAND_RUN "run"

#define MAXARGS 3
#define BUFFER_SIZE 100

void waitForChild(vector_t *children) {
    while (1) {
        child_t *child = malloc(sizeof(child_t));
        if (child == NULL) {
            perror("Error allocating memory");
            exit(EXIT_FAILURE);
        }
        child->pid = wait(&(child->status));
        if (child->pid < 0) {
            if (errno == EINTR) {
                /* Este codigo de erro significa que chegou signal que interrompeu a espera
                   pela terminacao de filho; logo voltamos a esperar */
                free(child);
                continue;
            } else {
                perror("Unexpected error while waiting for child.");
                exit (EXIT_FAILURE);
            }
        }
        vector_pushBack(children, child);
        return;
    }
}

void printChildren(vector_t *children) {
    for (int i = 0; i < vector_getSize(children); ++i) {
        child_t *child = vector_at(children, i);
        int status = child->status;
        pid_t pid = child->pid;
        if (pid != -1) {
            const char* ret = "NOK";
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                ret = "OK";
            }
            printf("CHILD EXITED: (PID=%d; return %s)\n", pid, ret);
        }
    }
    puts("END.");
}

int main (int argc, char** argv) {

    char *args[MAXARGS + 1];
    char buffer[BUFFER_SIZE];
    int MAXCHILDREN = -1;
    vector_t *children;
    int runningChildren = 0;

    bool_t from_pipe = FALSE;
    char *unknown_message = "Unknown command. Try again.";
    char *invalid_message = "Invalid syntax. Try again.";


    char *adv_pipe_name = (char*)malloc(strlen(argv[0]) + strlen(".pipe") + 1);
    int adv_pipe;

    char client_name[BUFFER_SIZE];
    int client_pipe;


    fd_set readfds;

    printf("Welcome to CircuitRouter-AdvancedShell\n\n");

    sprintf(adv_pipe_name, "%s.pipe", argv[0]);
    unlink(adv_pipe_name);

    adv_pipe = mkfifo(adv_pipe_name, 0666);
    if(adv_pipe < 0) {
        perror("Error creating named pipe.\n");
        exit(-1);
    }


    if(argv[1] != NULL){
        MAXCHILDREN = atoi(argv[1]);
    }

    children = vector_alloc(MAXCHILDREN);

    while (1) {
        int numArgs;
        if ((adv_pipe = open(adv_pipe_name, O_RDONLY|O_NONBLOCK)) < 0) exit (-1);
        from_pipe = FALSE;

        FD_ZERO(&readfds);
        FD_SET(0, &readfds);
        FD_SET(adv_pipe, &readfds);

        select(adv_pipe+1, &readfds, 0, 0, 0);

        if(FD_ISSET(0, &readfds)){
            numArgs = readLineArguments(args, MAXARGS+1, buffer, BUFFER_SIZE);
        }
        
        if(FD_ISSET(adv_pipe, &readfds)){
            numArgs = readPipeArguments(args, MAXARGS+1, buffer, BUFFER_SIZE, adv_pipe, client_name);
            from_pipe = TRUE;
        }
            
        /* EOF (end of file) do stdin ou comando "sair" */
        if (numArgs < 0 || (numArgs > 0 && (strcmp(args[0], COMMAND_EXIT) == 0) && (!from_pipe))) {
            printf("CircuitRouter-AdvancedShell will exit.\n--\n");

            /* Espera pela terminacao de cada filho */
            while (runningChildren > 0) {
                waitForChild(children);
                runningChildren --;
            }

            printChildren(children);
            printf("--\nCircuitRouter-AdvancedShell ended.\n");
            break;
        }

        else if (numArgs > 0 && strcmp(args[0], COMMAND_RUN) == 0){
            int pid;
            if (numArgs < 2) {
                if (from_pipe) {
                    if ((client_pipe = open(client_name, O_WRONLY)) < 0) exit (-1);
                    write(client_pipe, invalid_message, strlen(invalid_message));
                    close(client_pipe);
                }
                else
                    printf("%s: invalid syntax. Try again.\n", COMMAND_RUN);
                continue;
            }
            if (MAXCHILDREN != -1 && runningChildren >= MAXCHILDREN) {
                waitForChild(children);
                runningChildren--;
            }

            pid = fork();
            if (pid < 0) {
                perror("Failed to create new process.");
                exit(EXIT_FAILURE);
            }

            if (pid > 0) {
                runningChildren++;
                printf("%s: background child started with PID %d.\n\n", COMMAND_RUN, pid);
                continue;
            } else {
                char seqsolver[] = "../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver";
                char *newArgs[4] = {seqsolver, args[1], client_name, NULL};

                execv(seqsolver, newArgs);
                perror("Error while executing child process"); // Nao deveria chegar aqui
                exit(EXIT_FAILURE);
            }
        }

        else if (numArgs == 0){
            /* Nenhum argumento; ignora e volta a pedir */
            continue;
        }
        else {
            if (from_pipe) {
                if ((client_pipe = open(client_name, O_WRONLY)) < 0) exit (-1);
                write(client_pipe, unknown_message, strlen(unknown_message));
                close(client_pipe);
            }
            else 
                printf("%s", unknown_message);
        }
        close(adv_pipe);
    
    }

    for (int i = 0; i < vector_getSize(children); i++) {
        free(vector_at(children, i));
    }
    vector_free(children);

    return EXIT_SUCCESS;
}
