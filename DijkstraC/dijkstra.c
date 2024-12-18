/*
    Computes Dijkstra's alogrthim to find the shortest path between two vertices in a graph.

    Tyler Knapp and Shane Bourdeau
    Nov 1 2024
*/
#include <stdio.h>
#include <stdlib.h>

void Dijkstra(HighwayVertex* vertices, int numVertices, int startIdx, int destIdx) {
    int* visited = (int*)malloc(numVertices * sizeof(int));
    HighwayEdge** result = (HighwayEdge**)malloc(numVertices * sizeof(HighwayEdge*));
    for (int i = 0; i < numVertices; i++) {
        visited[i] = 0;
        result[i] = NULL;
    }
    
    PriorityQueue* pq = createPQ(100);
    visited[startIdx] = 1;

    HighwayEdge* e = vertices[startIdx].head;
    while (e) {
        PQEntry entry = {e->length, e};
        add(pq, entry);
        e = e->next;
    }
    
    int reached = startIdx;
    while (reached != destIdx) {
        PQEntry nextPQ;
        HighwayEdge* nextEdge;
        do {
            nextPQ = removeMin(pq);
            nextEdge = nextPQ.lastEdge;
        } while (visited[nextEdge->dest]);
        
        result[nextEdge->dest] = nextEdge;
        visited[nextEdge->dest] = 1;
        reached = nextEdge->dest;
        
        e = vertices[reached].head;
        while (e) {
            if (!visited[e->dest]) {
                PQEntry newEntry = {nextPQ.totalDist + e->length, e};
                add(pq, newEntry);
            }
            e = e->next;
        }
    }

    // Displaying the path
    int current = destIdx;
    double totalLength = 0.0;
    while (current != startIdx) {
        HighwayEdge* hop = result[current];
        printf("Travel from %s to %s for %.2f along %s\n", vertices[hop->source].label, vertices[hop->dest].label, hop->length, hop->label);
        totalLength += hop->length;
        current = hop->source;
    }
    printf("Total distance: %.2f\n", totalLength);

    free(pq->data);
    free(pq);
    free(visited);
    free(result);
}