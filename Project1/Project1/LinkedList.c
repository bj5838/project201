#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "project_p2_header.h"

extern void computeHashWrapper(char* str, unsigned char* prevHash, unsigned char* outHash);
extern void printHashWrapper(unsigned char* hash);
extern int compareHashWrapper(unsigned char* h1, unsigned char* h2);

void initialize(struct LinkedList* list) {
    list->size = 0;
    list->head = NULL;
}

void addNode(struct LinkedList* list, char* command) {
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    if (!newNode) return;

    strcpy(newNode->name, command);
    if (list->head) {
        computeHashWrapper(command, list->head->hash, newNode->hash);
    }
    else {
        computeHashWrapper(command, NULL, newNode->hash);
    }
    newNode->next = list->head;
    list->head = newNode;
    list->size++;
}

void printList(struct LinkedList list) {
    struct node* temp = list.head;
    if (temp == NULL) {
        printf("History is empty.\n");
        return;
    }

    int i = 0;
    while (temp != NULL) {
        printf("[%d] %s | Hash: ", i, temp->name);
        printHashWrapper(temp->hash);
        printf("\n");
        temp = temp->next;
        i++;
    }
    printf("---- End of History ----\n");
}

void validateList(struct LinkedList* list) {
    struct node* curr = list->head;
    int allValid = 1;

    while (curr && curr->next) {
        unsigned char recomputed[HASH_SIZE];
        computeHashWrapper(curr->next->name, curr->hash, recomputed);

        if (!compareHashWrapper(curr->next->hash, recomputed)) {
            printf("Node changed at: %s\n", curr->next->name);
            allValid = 0;
        }
        curr = curr->next;
    }

    if (allValid) {
        printf("Validation complete - all nodes are valid.\n");
    }
    else {
        printf("Validation complete - some nodes were modified.\n");
    }
}