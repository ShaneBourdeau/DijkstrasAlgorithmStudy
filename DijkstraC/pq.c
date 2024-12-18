/*
	Implementation file for a min priority queue using singly linked list structure that holds ratio values

	Jim Teresco, Siena College, CSIS 330, Spring 2012
	edited by Michael Plekan
  	Initial implementation:
  	Fri Feb  3 11:02:04 EST 2012
	Second implementation:
	Fall 2021
*/

#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"

typedef struct {
    PQEntry *data;
    int size;
    int capacity;
} PriorityQueue;

PriorityQueue* createPQ(int capacity) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->data = (PQEntry*)malloc(capacity * sizeof(PQEntry));
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

void swap(PQEntry* a, PQEntry* b) {
    PQEntry temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(PriorityQueue* pq, int index) {
    int parent = (index - 1) / 2;
    if (index > 0 && pq->data[index].totalDist < pq->data[parent].totalDist) {
        swap(&pq->data[index], &pq->data[parent]);
        heapifyUp(pq, parent);
    }
}

void heapifyDown(PriorityQueue* pq, int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int smallest = index;
    
    if (left < pq->size && pq->data[left].totalDist < pq->data[smallest].totalDist) {
        smallest = left;
    }
    if (right < pq->size && pq->data[right].totalDist < pq->data[smallest].totalDist) {
        smallest = right;
    }
    if (smallest != index) {
        swap(&pq->data[index], &pq->data[smallest]);
        heapifyDown(pq, smallest);
    }
}

void add(PriorityQueue* pq, PQEntry entry) {
    if (pq->size == pq->capacity) {
        pq->capacity *= 2;
        pq->data = (PQEntry*)realloc(pq->data, pq->capacity * sizeof(PQEntry));
    }
    pq->data[pq->size] = entry;
    heapifyUp(pq, pq->size);
    pq->size++;
}

PQEntry removeMin(PriorityQueue* pq) {
    PQEntry min = pq->data[0];
    pq->data[0] = pq->data[pq->size - 1];
    pq->size--;
    heapifyDown(pq, 0);
    return min;
}