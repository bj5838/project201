#include <windows.h>
#include <string.h>
#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS

#define HASH_SIZE 8

extern "C" __declspec(dllexport)
void computeHash(char* str, unsigned char* prevHash, unsigned char* outHash) {
    unsigned char temp[HASH_SIZE];
    memset(temp, 0, HASH_SIZE);

    for (int i = 0; i < HASH_SIZE; i++) {
        if (prevHash != NULL) {
            temp[i] = prevHash[i];
        }
        else {
            temp[i] = (unsigned char)(170 + i);
        }
    }

    for (int i = 0; str[i] != '\0'; i++) {
        unsigned char c = (unsigned char)str[i];
        int j = i % HASH_SIZE;
        temp[j] ^= c;
    }

    for (int i = 0; i < HASH_SIZE; i++) {
        outHash[i] = temp[i];
    }
}

extern "C" __declspec(dllexport)
void printHash(unsigned char* hash) {
    // FIXED: Actually print the hash
    char buffer[20];
    for (int i = 0; i < HASH_SIZE; i++) {
        printf(buffer + i * 2, "%02X", hash[i]);
    }
    printf("%s", buffer);
}

extern "C" __declspec(dllexport)
int compareHash(unsigned char* h1, unsigned char* h2) {
    return memcmp(h1, h2, HASH_SIZE) == 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason, LPVOID lpReserved) {
    return TRUE;
}