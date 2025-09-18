#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define MAX_INPUT 256

void processCommand(char* input) {
    char* token = strtok(input, " \t\n"); // first word
    if (token == NULL) return;

    if (strcmp(token, "upload") == 0) {
        char* local = strtok(NULL, " \t\n");
        char* remote = strtok(NULL, " \t\n");
        if (!local || !remote) {
            printf("Syntax error: upload <local filename> <remote filename>\n");
        }else if (strtok(NULL, " \t\n") != NULL) {
            printf("Syntax error: upload <local filename> <remote filename>\n");
        }else {
            printf("Valid command: upload\n");
        }
    }
    else if (strcmp(token, "download") == 0) {
        char* remote = strtok(NULL, " \t\n");
        char* local = strtok(NULL, " \t\n");
        if (!remote || !local) {
            printf("Syntax error: download <remote filename> <local filename>\n");
        }
        else if (strtok(NULL, " \t\n") != NULL) {
            printf("Syntax error: download <remote filename> <local filename>\n");
        }else {
            printf("Valid command: download\n");
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
        }
    }
    else {
        printf("%s is not a valid FML command.\n", token);
    }
}

int main() {
    char input[MAX_INPUT];

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
