#pragma once
#include <stdio.h>

#define HASH_SIZE 8
//define node
struct node {
	char name[256];		//Store full command string
	unsigned char hash[HASH_SIZE];
	struct node* next;	//pointer to next node
};

//linked list wrapper
struct LinkedList {
	int size;
	struct noide* head;
};

//prototypes
void initialize(struct LinkedList* list);
void addNode(struct LinkedList* list, char* command);
void printList(struct LinkedList list);
void validateList(struct LinkedList* list);