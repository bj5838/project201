#define HASHDLL_EXPORTS
#define _CRT_SECURE_NO_WARNINGS
#include "HashFunctionDLL.h"
#include <string.h>
#include <stdio.h>

extern "C" {
    HASH_API void computeHash(char* str, char* prevHash, char* outHash) {
        unsigned char temp[HASH_SIZE];
        memset(temp, 0, HASH_SIZE);

        for (int i = 0; i < HASH_SIZE; i++) {
            if (prevHash != NULL) {
                temp[i] = prevHash[i];
            }
            else {
                temp[i] = (char)(170 + i);
            }
        }

        for (int i = 0; str[i] != '\0'; i++) {
            char c = (char)str[i];
            int j = i % HASH_SIZE;

            temp[j] ^= (c << (i % 3));
            temp[j] |= (c >> 2);
            temp[j] = ~temp[j];
            temp[j] &= 255;
        }

        for (int i = 0; i < HASH_SIZE; i++) {
            outHash[i] = (temp[i] ^ (temp[(i + 1) % HASH_SIZE] << 1)) & 0xFF;
        }
    }

    HASH_API void printHash(char* hash) {
        for (int i = 0; i < HASH_SIZE; i++) {
            printf("%02X", hash[i]);
        }
    }

    HASH_API int compareHash(char* h1, char* h2) {
        return memcmp(h1, h2, HASH_SIZE) == 0;
    }
}