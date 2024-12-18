/*
    Header file for dijkstra.c
    
    Tyler Knapp and Shane Bourdeau
    Nov 1 2024
*/
#ifndef HIGHWAY_GRAPH_H
#define HIGHWAY_GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <limits.h>

typedef struct HighwayEdge {
    char label[50];
    double length;
    int dest;
    int source;
    struct HighwayEdge *next;
} HighwayEdge;

typedef struct HighwayVertex {
    char label[50];
    int vNum;
    int visited;
    HighwayEdge *head;
} HighwayVertex;

typedef struct {
    double totalDist;
    HighwayEdge *lastEdge;
} PQEntry;

void Dijkstra(HighwayVertex* vertices, int numVertices, int startIdx, int destIdx);

#endif
