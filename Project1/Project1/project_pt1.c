#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <windows.h>  // For LoadLibrary, GetProcAddress
#include "project_p2_header.h"
#define MAX_INPUT 256

// Function pointer types
typedef void (*ComputeHashFunc)(char*, unsigned char*, unsigned char*);
typedef void (*PrintHashFunc)(unsigned char*);
typedef int (*CompareHashFunc)(unsigned char*, unsigned char*);

// Global pointers to DLL functions
static ComputeHashFunc pComputeHash = NULL;
static PrintHashFunc pPrintHash = NULL;
static CompareHashFunc pCompareHash = NULL;
static HINSTANCE hHashDLL = NULL;

// Load DLL function - FIXED: Use wide strings
int loadHashDLL() {
    // Try multiple locations - ALL USING WIDE STRINGS (L prefix)

    // Location 1: Current directory
    hHashDLL = LoadLibrary(L"Dll1.dll");

    // Location 2: Relative path to Debug folder
    if (hHashDLL == NULL) {
        hHashDLL = LoadLibrary(L".\\Debug\\Dll1.dll");
    }

    // Location 3: Full path
    if (hHashDLL == NULL) {
        hHashDLL = LoadLibrary(L"C:\\Users\\Student\\source\\repos\\bj5838\\project201\\Project1\\Dll1\\Debug\\Dll1.dll");
    }

    // Location 4: Try different naming
    if (hHashDLL == NULL) {
        hHashDLL = LoadLibrary(L"Dll1");
    }

    if (hHashDLL == NULL) {
        DWORD error = GetLastError();
        printf("ERROR: Failed to load Dll1.dll\n");
        printf("Error code: %d\n", error);

        // Detailed error messages
        if (error == 2) {
            printf("File not found. Check the path.\n");
        }
        else if (error == 126) {
            printf("Module found but missing dependencies or wrong architecture.\n");
            printf("Common causes:\n");
            printf("1. DLL needs Visual C++ Redistributable\n");
            printf("2. Architecture mismatch (x64 vs x86)\n");
            printf("3. DLL corrupted\n");
        }
        else if (error == 193) {
            printf("Bad EXE format. Both DLL and EXE must be Win32 (x86).\n");
        }

        printf("Tried locations:\n");
        printf("1. Dll1.dll (current folder)\n");
        printf("2. .\\Debug\\Dll1.dll\n");
        printf("3. Full path\n");
        return 0;
    }

    printf("DLL loaded successfully! Getting functions...\n");

    // Get function addresses
    pComputeHash = (ComputeHashFunc)GetProcAddress(hHashDLL, "computeHash");
    pPrintHash = (PrintHashFunc)GetProcAddress(hHashDLL, "printHash");
    pCompareHash = (CompareHashFunc)GetProcAddress(hHashDLL, "compareHash");

    // Check if all functions were found
    if (pComputeHash == NULL) {
        printf("ERROR: Could not find computeHash function in DLL\n");
        printf("Make sure your DLL exports 'computeHash' (case-sensitive)\n");
    }

    if (pPrintHash == NULL) {
        printf("ERROR: Could not find printHash function in DLL\n");
    }

    if (pCompareHash == NULL) {
        printf("ERROR: Could not find compareHash function in DLL\n");
    }

    if (pComputeHash == NULL || pPrintHash == NULL || pCompareHash == NULL) {
        FreeLibrary(hHashDLL);
        hHashDLL = NULL;
        return 0;
    }

    printf("All hash functions loaded successfully from DLL.\n");
    return 1;
}

// Unload DLL
void unloadHashDLL() {
    if (hHashDLL != NULL) {
        FreeLibrary(hHashDLL);
        hHashDLL = NULL;
        pComputeHash = NULL;
        pPrintHash = NULL;
        pCompareHash = NULL;
        printf("DLL unloaded.\n");
    }
}

// Wrapper functions that call DLL
void computeHashWrapper(char* str, unsigned char* prevHash, unsigned char* outHash) {
    if (pComputeHash != NULL) {
        pComputeHash(str, prevHash, outHash);
    }
    else {
        printf("FATAL ERROR: Hash function not available!\n");
        exit(1);
    }
}

void printHashWrapper(unsigned char* hash) {
    if (pPrintHash != NULL) {
        pPrintHash(hash);
    }
    else {
        printf("ERROR: Cannot print hash - function not available\n");
    }
}

int compareHashWrapper(unsigned char* h1, unsigned char* h2) {
    if (pCompareHash != NULL) {
        return pCompareHash(h1, h2);
    }
    else {
        printf("ERROR: Cannot compare hashes - function not available\n");
        return 0;
    }
}

struct LinkedList history;

// FIXED: Added 'static' to fix the warning
static void processCommand(char* input) {
    char inputCopy[MAX_INPUT];
    strcpy(inputCopy, input);

    char* token = strtok(input, " \t\n"); // first word
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
            printList(history);     //print before adding
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

    // Load DLL first
    printf("Loading hash function DLL...\n");
    if (!loadHashDLL()) {
        printf("CRITICAL ERROR: Cannot run without hash function DLL.\n");
        printf("Please copy Dll1.dll to this program's folder and try again.\n");
        printf("Press Enter to exit...\n");
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
        input[strcspn(input, "\n")] = 0; // Remove newline
        processCommand(input);
    }

    // Unload DLL before exit
    unloadHashDLL();
    return 0;
}