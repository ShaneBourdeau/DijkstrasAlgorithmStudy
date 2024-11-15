/*
	Implementation file for a min priority queue using singly linked list structure that holds ratio values

	Jim Teresco, Siena College, CSIS 330, Spring 2012
	edited by Michael Plekan
  	Initial implementation:
  	Fri Feb  3 11:02:04 EST 2012
	Second implementation:
	Fall 2021
*/

#include <stdlib.h>
#include <stdio.h>
#include "pq.h"
/* create a new, empty, pq structure */
pq * create_pq() {

  	pq *l = (pq *)malloc(sizeof(pq));
	l->tail = NULL;
	l->head= NULL;
  	return l;
}

/* check if the list is empty.  This is the case if the tail pointer is NULL */
int pq_isempty(pq *l) {
  	return l->tail == NULL;
}

/*Searches through the list to put the new ratio in the correct spot for a min priority queue */
extern void enqueue(pq *l, void *value, double priority){

	pq_node *newnode = (pq_node *)malloc(sizeof(pq_node));
  	newnode->value = value;
	newnode->priority = priority;
	if(l->head==NULL){
  		l->tail = newnode;
		l->head = newnode;
	}
	else{
		pq_node *f=newnode;
		double newVal,current;
		newVal = newnode->priority;
		if(l->head !=NULL){
			current = l->head->priority;
			if(newVal<current){
					newnode->next=l->head;
					l->head = newnode;
					return;
			}
			f = l->head;
  			while (f != NULL) {
				if (f->next != NULL)current=f->next->priority;
				else{
						f->next = newnode;
						l->tail=newnode;
						break;
				}
				if(newVal<current){
					newnode->next=f->next;
					f->next = newnode;
					break;
				}
    				f = f->next;
  			}
		}
	}
}

/* remove from the head of the list, return value removed */
void *dequeue(pq *l) {

  	if (l->tail == NULL) {
    		fprintf(stderr, "(dequeue) Attempt to remove from empty list!\n");
    		return 0;
  	}

  	void *retval =l->head->value;
	struct pq_node *temp;
	temp=l->head;
	l->head=l->head->next;
	
	if(l->head==NULL)l->tail=NULL;
	free(temp);
  	return retval;
}

/* destroy a possibly non-empty list */
void pq_destroy(pq *l) {
  	pq_node *ptr = l->head;
  	while (ptr) {
    		pq_node *rmme = ptr;
    		ptr = ptr->next;
    		free(rmme);
  	}
  	free(l);
}

/* print the contents of a list */
void pq_print_contents(pq *l) {

  pq_node *f;
	if(l->head !=NULL){
		f = l->head;
  		printf("<");
  		while (f != NULL) {
    			printf("%f",f->priority);
    			f = f->next;
    			if (f != NULL) printf(", ");
  		}
  		printf(">\n");
	}
}
