#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "project_p2_header.h"


void initialize(struct LinkedList* list) {
	list->size = 0;
	list->head = NULL;
}

void addNode(struct LinkedList* list, char* command) {
	struct node* newNode = (struct node*)malloc(sizeof(struct node));
	if (!newNode) return;

	strcpy(newNode->name, command);
	if (list->head) {
		computeHash(command, list->head->hash, newNode->hash);
	}
	else {
		computeHash(command, NULL, newNode->hash);
	}
	newNode->next = list->head;		//add at front
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
		printHash(temp->hash);
		printf("\n");
		temp = temp->next;
		i++;
	}
	printf("---- End of History ----\n");
}

void validateList(struct LinkedList* list) {
	struct node* curr = list->head;
	while (curr && curr->next) {
		unsigned char recomputed[HASH_SIZE];
		computeHash(curr->next->name, curr->hash, recomputed);

		if (!compareHash(curr->next->hash, recomputed)) {
			printf("Node changed: %s\n", curr->next->name);
		}
		curr = curr->next;
	}

	printf("Validatation complete.\n");
}
