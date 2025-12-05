#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "project_p2_header.h"
#define MAX_INPUT 256

// Function pointer types for DLL functions - use correct types
typedef void (*ComputeHashFunc)(char*, unsigned char*, unsigned char*);
typedef void (*PrintHashFunc)(unsigned char*);
typedef int (*CompareHashFunc)(unsigned char*, unsigned char*);

// Global function pointers - use different names
ComputeHashFunc pComputeHash = NULL;
PrintHashFunc pPrintHash = NULL;
CompareHashFunc pCompareHash = NULL;

// DLL handle
HMODULE hashDllHandle = NULL;

// Function to load the DLL
int loadHashDLL() {
    // Try to load the DLL - use TEXT macro for Unicode compatibility
    hashDllHandle = LoadLibrary(TEXT("HashFunctionDLL.dll"));

    if (hashDllHandle == NULL) {
        DWORD error = GetLastError();
        printf("Error: Failed to load HashFunctionDLL.dll. Error code: %lu\n", error);
        printf("Make sure HashFunctionDLL.dll is in the same directory as this executable.\n");
        return 0;
    }

    // Get function addresses
    pComputeHash = (ComputeHashFunc)GetProcAddress(hashDllHandle, "computeHash");
    pPrintHash = (PrintHashFunc)GetProcAddress(hashDllHandle, "printHash");
    pCompareHash = (CompareHashFunc)GetProcAddress(hashDllHandle, "compareHash");

    if (pComputeHash == NULL || pPrintHash == NULL || pCompareHash == NULL) {
        printf("Error: Failed to find required functions in HashFunctionDLL.dll\n");
        FreeLibrary(hashDllHandle);
        hashDllHandle = NULL;
        return 0;
    }

    printf("Hash function DLL loaded successfully.\n");
    return 1;
}

// Unload the DLL when done
void unloadHashDLL() {
    if (hashDllHandle != NULL) {
        FreeLibrary(hashDllHandle);
        hashDllHandle = NULL;
        printf("Hash function DLL unloaded.\n");
    }
}

struct LinkedList history; // global history list

// Wrapper functions that use the DLL
void computeHashWrapper(char* str, unsigned char* prevHash, unsigned char* outHash) {
    if (pComputeHash != NULL) {
        pComputeHash(str, prevHash, outHash);
    }
    else {
        printf("Error: Hash function not available!\n");
        exit(1);
    }
}

void printHashWrapper(unsigned char* hash) {
    if (pPrintHash != NULL) {
        pPrintHash(hash);
    }
    else {
        printf("Error: Print hash function not available!\n");
    }
}

int compareHashWrapper(unsigned char* h1, unsigned char* h2) {
    if (pCompareHash != NULL) {
        return pCompareHash(h1, h2);
    }
    else {
        printf("Error: Compare hash function not available!\n");
        return 0;
    }
}

void processCommand(char* input) {
    char inputCopy[MAX_INPUT];
    strcpy(inputCopy, input);

    char* token = strtok(input, " \t\n");
    if (token == NULL) return;

    if (strcmp(token, "upload") == 0) {
        char* local = strtok(NULL, " \t\n");
        char* remote = strtok(NULL, " \t\n");
        if (!local || !remote || strtok(NULL, " \t\n")) {
            printf("Syntax error: upload <local filename> <remote filename>\n");
        }
        else if (strtok(NULL, " \t\n") != NULL) {
            printf("Syntax error: upload <local filename> <remote filename>\n");
        }
        else {
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
        }
        else {
            printf("Valid command: download\n");
            addNode(&history, inputCopy);
        }
    }
    else if (strcmp(token, "delete") == 0) {
        char* scope = strtok(NULL, " \t\n");
        char* file = strtok(NULL, " \t\n");
        if (!scope || !file) {
            printf("Syntax error: delete <local|remote> <filename>\n");
        }
        else if (strcmp(scope, "local") != 0 && strcmp(scope, "remote") != 0) {
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
    else if (strcmp(token, "history") == 0) {
        if (strtok(NULL, " \t\n") != NULL) {
            printf("Syntax error: 'history' takes no parameters.\n");
        }
        else {
            printf("Valid command: history\n");
            validateList(&history);
            printList(history);
            addNode(&history, inputCopy);
        }
    }
    else if (strcmp(token, "validate") == 0) {
        if (strtok(NULL, " \t\n") != NULL) {
            printf("Syntax error: 'validate' takes no parameters.\n");
        }
        else {
            printf("Valid command: validate\n");
            validateList(&history);
            addNode(&history, inputCopy);
        }
    }
    else if (strcmp(token, "quit") == 0) {
        printf("Exiting Program.\n");
        exit(0);
    }
    else {
        printf("%s is not a valid FML command.\n", token);
    }
}

int main() {
    char input[MAX_INPUT];

    // Load the DLL
    if (!loadHashDLL()) {
        printf("Critical error: Cannot run without hash function DLL.\n");
        printf("Press any key to exit...\n");
        getchar();
        return 1;
    }

    initialize(&history);

    printf("Welcome to FML Parser. Type commands below.\n");
    while (1) {
        printf("FML> ");
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            break;
        }
        processCommand(input);
    }

    // Unload DLL before exit
    unloadHashDLL();
    return 0;
}