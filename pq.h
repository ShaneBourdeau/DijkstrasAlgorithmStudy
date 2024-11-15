/*
  Header file for a singly-linked list priority queue structure that holds ratio values
  	Jim Teresco, Siena College, CSIS 330, Spring 2012
	edited by Michael Plekan
  	Initial implementation:
  	Fri Feb  3 11:02:04 EST 2012
	Second implementation:
	Fall 2021
*/

typedef struct pq_node {
  	void * value;
	double priority;
  	struct pq_node *next;
} pq_node;

typedef struct pq {
	struct pq_node * head;
	struct pq_node * tail;
} pq;

extern pq * create_pq();
extern void enqueue(pq *l, void *value,double priority);
extern void * dequeue(pq *l);
extern void pq_print_contents(pq *l);
extern int pq_isempty(pq *q);
extern void pq_destroy(pq *q);
