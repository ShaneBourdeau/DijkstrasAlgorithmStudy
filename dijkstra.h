/*
    Header file for dijkstra.c
    Tyler Knapp and Shane Bourdeau
*/
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

// Structure for a vertex in the graph
typedef struct vertex {
    char label [50];
    double lattitude;
    double longitude;
  double *extra;
} vertex;

// Structure for an edge in the graph
typedef struct edge {
    struct vertex *end1;
    struct vertex *end2;
    char location;
    double length;
} edge;

// Structure to represent a list of edges for a vertex
typedef struct edgelist{
    struct edge *edge;
    struct edgelist *next;
} edgelist;

// Structure for a path in Dijkstra's algorithm
typedef struct dijkstraPath{
    struct vertex *vertex;
    struct edge *edge;
    double length;
} dijkstraPath;

void dijkstra(vertex *start, vertex *end);
void build_adj_list(vertex vertices[], edge edges[], edgelist *adjList[], int numVertices, int numEdges);

#endif
