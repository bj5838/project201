#pragma once

#ifdef HASHDLL_EXPORTS
#define HASH_API __declspec(dllexport)
#else
#define HASH_API __declspec(dllimport)
#endif

#include <windows.h>

#define HASH_SIZE 8

extern "C" {
    HASH_API void computeHash(char* str, unsigned char* prevHash, unsigned char* outHash);
    HASH_API void printHash(unsigned char* hash);
    HASH_API int compareHash(unsigned char* hash1, unsigned char* hash2);
}