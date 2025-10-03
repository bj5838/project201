#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "project_p2_header.h"
#include "LinkedList.c"
#define MAX_INPUT 256

struct LinkedList history; // global history list

void processCommand(char* input) {
    char inputCopy[MAX_INPUT];
    strcpy(inputCopy, input);

    char* token = strtok(input, " \t\n"); // first word
    if (token == NULL) return;

    if (strcmp(token, "upload") == 0) {
        char* local = strtok(NULL, " \t\n");
        char* remote = strtok(NULL, " \t\n");
        if (!local || !remote || strtok(NULL, " \t\n")) {
            printf("Syntax error: upload <local filename> <remote filename>\n");
        }else if (strtok(NULL, " \t\n") != NULL) {
            printf("Syntax error: upload <local filename> <remote filename>\n");
        }else {
            printf("Valid command: upload\n");
            addNode(&history, inputCopy);
        }
    }
    else if (strcmp(token, "download") == 0) {
        char* remote = strtok(NULL, " \t\n");
        char* local = strtok(NULL, " \t\n");
        if (!remote || !local || strtok(NULL, " \t\n")) {
            printf("Syntax error: download <remote filename> <local filename>\n");
        }
        else if (strtok(NULL, " \t\n") != NULL) {
            printf("Syntax error: download <remote filename> <local filename>\n");
        }else {
            printf("Valid command: download\n");
            addNode(&history, inputCopy);
        }
    }
    else if (strcmp(token, "delete") == 0) {
        char* scope = strtok(NULL, " \t\n");
        char* file = strtok(NULL, " \t\n");
        if (!scope || !file) {
            printf("Syntax error: delete <local|remote> <filename>\n");
        }else if (strcmp(scope, "local") != 0 && strcmp(scope, "remote") != 0) {
            printf("'%s' is not recognized. Valid options are 'local' and 'remote'.\n", scope);
        }
        else if (strtok(NULL, " \t\n") != NULL) {
            printf("Syntax error: delete <local|remote> <filename>\n");
        }
        else {
            printf("Valid command: delete\n");
            addNode(&history, inputCopy);
        }
    }
    else if (strcmp(token, "change") == 0) {
        char* scope = strtok(NULL, " \t\n");
        char* path = strtok(NULL, " \t\n");
        if (!scope || !path) {
            printf("Syntax error: change <local|remote> <filepath>\n");
        }
        else if (strcmp(scope, "local") != 0 && strcmp(scope, "remote") != 0) {
            printf("'%s' is not recognized. Valid options are 'local' and 'remote'.\n", scope);
        }
        else {
            printf("Valid command: change\n");
            addNode(&history, inputCopy);
        }
    }
    else if (strcmp(token, "show") == 0) {
        char* scope = strtok(NULL, " \t\n");
        char* what = strtok(NULL, " \t\n");
        if (!scope || !what) {
            printf("Syntax error: show <local|remote> <path|files|folders>\n");
        }
        else if (strcmp(scope, "local") != 0 && strcmp(scope, "remote") != 0) {
            printf("'%s' is not recognized. Valid options for show are 'local' and 'remote'.\n", scope);
        }
        else if (strcmp(what, "path") != 0 && strcmp(what, "files") != 0 && strcmp(what, "folders") != 0) {
            printf("'%s' is not recognized. Valid options are 'path', 'files', and 'folders'.\n", what);
        }
        else if (strtok(NULL, " \t\n") != NULL) {
            printf("Syntax error: show <local|remote> <path|files|folders>\n");
        }
        else {
            printf("Valid command: show\n");
            addNode(&history, inputCopy);
        }
    }
    else if (strcmp(token, "history") == 0 ||
        strcmp(token, "validate") == 0 ||
        strcmp(token, "quit") == 0) {
        if (strtok(NULL, " \t\n") != NULL) {
            printf("Syntax error: '%s' takes no parameters.\n", token);
        }
        else {
            printf("Valid command: %s\n", token);
            printList(history);     //print before adding
            addNode(&history, inputCopy);
        }
    }
    else {
        printf("%s is not a valid FML command.\n", token);
    }
}

int main() {
    char input[MAX_INPUT];

    initialize(&history);

    printf("Welcome to FML Parser. Type commands below.\n");
    while (1) {
        printf("FML> ");
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            break;
        }
        processCommand(input);
    }
    return 0;
}
