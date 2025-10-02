#include <string.h>
#include "project_p2_header.h"

void computeHash(char* str, unsigned char* prevHash, unsigned char* outHash) {
	unsigned char temp[HASH_SIZE];
	memset(temp, 0, HASH_SIZE);

	for (int i = 0; str[i] != '\0'; i++) {
		unsigned char c = (unsigned char)str[i];
		int j = i % HASH_SIZE;

		temp[j] ^= (c << (i % 3));
		temp[j] |= (c >> 2);
		temp[j] = ~temp[j];
		temp[j] &= 0xFF;
	}

	for (int i = 0; i < HASH_SIZE; i++) {
		outHash[i] = (temp[i] ^ (temp[(i + 1) % HASH_SIZE] << 1)) & 0xFF;
	}
}

void printHash(unsigned char* hash) {
	for (int i = 0; i < HASH_SIZE; i++) {
		printf("%02X", hash[i]);
	}
}

int compareHash(unsigned char* h1, unsigned char* h2) {
	return memcmp(h1, h2, HASH_SIZE) == 0;
}