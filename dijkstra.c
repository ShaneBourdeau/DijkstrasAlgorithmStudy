#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"
#include "pq.h"
/*
    Contains the implementation of Dijkstra's algorithm and building an adjacency list
    Orginal Dijsktra's algorithm implementation: Jim Teresco
    Adjacnecy List and Modifications: Tyler Knapp and Shane Bourdeau
*/

// Dijkstra's algorithm implementation
void dijkstra(vertex *start, vertex *end){
    printf("Start: %s, End: %s\n", start->label, end->label);

    struct pq *list = create_pq();
    int size = 10;
    dijkstraPath *found = malloc(size * sizeof(dijkstraPath));
    int current_found = 0;

    // Initialize start path
    dijkstraPath *strt = malloc(sizeof(dijkstraPath));
    strt->vertex = start;
    strt->length = 0.0;
    strt->edge = NULL;

    void *t = (void *)strt;
    enqueue(list, t, strt->length);

    int finding = 1;
    dijkstraPath *current;

    while (finding){
        current = (dijkstraPath *)dequeue(list);

        if (current_found >= size - 1){
            size *= 2;
            found = realloc(found, size * sizeof(dijkstraPath));
        }
        found[current_found] = *current;
        current_found++;

        if (current->vertex == end)
            finding = 0;
        current->vertex->extra = (void *)current;

        edgelist *f = current->vertex->extra; // Edgelist stored in `extra`
        while (f != NULL) {
            dijkstraPath *newP = malloc(sizeof(dijkstraPath));
            newP->length = current->length + f->edge->length;
            newP->edge = f->edge;

            if (f->edge->end1 == current->vertex){
                newP->vertex = f->edge->end2;
            }
            else{
                newP->vertex = f->edge->end1;
            }

            void *t = (void *)newP;
            if (newP->vertex->extra == NULL)
                enqueue(list, t, newP->length);
            f = f->next;
        }
    }

    printf("Paths found: %d\n", current_found);
    for (int i = 0; i < current_found; i++){
        printf("%s : %f\n", found[i].vertex->label, found[i].length);
        dijkstraPath *temp = &found[i];
        edge *f = found[i].edge;

        if (found[i].edge == NULL)
            continue;

        if (f->end1 == found[i].vertex) {
            temp = (dijkstraPath *)f->end2->extra;
        }
        else{
            temp = (dijkstraPath *)f->end1->extra;
        }

        while (temp != NULL){
            edge *f = temp->edge;
            printf("%s, ", temp->vertex->label);
            if (f == NULL)
                break;

            if (f->end1 == temp->vertex){
                temp = (dijkstraPath *)(f->end2->extra);
            }
            else{
                temp = (dijkstraPath *)(f->end1->extra);
            }
        }
        printf("\n\n");
    }

    free(found);
    free(strt);
}

// Build adjacency list for Dijkstra's algorithm
void build_adj_list(vertex vertices[], edge edges[], edgelist *adjList[], int numVertices, int numEdges){
    
    for(int i = 0; i < numVertices; i++){
        adjList[i] = NULL;
    }
    
    for (int i = 0; i < numEdges; i++){
        edgelist *temp = malloc(sizeof(edgelist));
        temp->edge = &edges[i];
        temp->next = adjList[edges[i].end1 - vertices];
        adjList[edges[i].end1 - vertices] = temp;

        temp = malloc(sizeof(edgelist));
        temp->edge = &edges[i];
        temp->next = adjList[edges[i].end2 - vertices];
        adjList[edges[i].end2 - vertices] = temp;
    }
}

    