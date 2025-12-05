#pragma once
#include <stdio.h>

#define HASH_SIZE 8

struct node {
    char name[256];
    unsigned char hash[HASH_SIZE];
    struct node* next;
};

struct LinkedList {
    int size;
    struct node* head;
};

// Function prototypes
void initialize(struct LinkedList* list);
void addNode(struct LinkedList* list, char* command);
void printList(struct LinkedList list);
void validateList(struct LinkedList* list);

// Hash function declarations - these will be implemented via DLL
void computeHashWrapper(char* str, unsigned char* prevHash, unsigned char* outHash);
void printHashWrapper(unsigned char* hash);
int compareHashWrapper(unsigned char* h1, unsigned char* h2);

// DLL loading function
int loadHashDLL(void);
void unloadHashDLL(void);