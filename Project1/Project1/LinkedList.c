#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "project_p2_header.h"

void initialize(struct LinkledList* list) {
	list->size = 0;
	list->head = NULL;
}

void addNode(struct LinkedList* list, char* command) {
	struct node* newNode = (struct node*)malloc(sizeof(struct node));
	strcpy(newNode->name, command);
	newNode->next = list->head;		//add at front
	list->head = newNode;
	list->size++;
}

void printList(struct LinkedList list) {
	struct node* temp = list.head;
	if (temp == NULL) {
		printf("Historyt is empty.\n");
		return;
	}

	while (temp!= NULL) {
		printf("%s", temp->name);
		temp = temp->next;
	}
	printf("---- End of History ----\n");
}
