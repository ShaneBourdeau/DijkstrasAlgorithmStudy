#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdio.h>
#include <stdlib.h> 

typedef struct {
    double totalDist; 
    void *lastEdge; 
} PQEntry;

typedef struct {
    PQEntry *data; 
    int size;      
    int capacity;  
} PriorityQueue;


PriorityQueue* createPQ(int capacity);              
void swap(PQEntry* a, PQEntry* b);                  
void heapifyUp(PriorityQueue* pq, int index);      
void heapifyDown(PriorityQueue* pq, int index);     
void add(PriorityQueue* pq, PQEntry entry);         
PQEntry removeMin(PriorityQueue* pq);               

#endif