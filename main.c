#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"
/*
    Reads in a TMG file to create a graph and runs Dijkstra's algorithm on it
    Tyler Knapp and Shane Bourdeau
*/

int main(int argc, char *argv[]){
    // check for correct number of arguments
    if (argc != 2){
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // open file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL){
        printf("Error: File not found\n");
        return 1;
    }

    // read in first line, first number is number of vertices, second number is number of edges
    int numVertices, numEdges;
    fscanf(file, "%d %d", &numVertices, &numEdges);

    // create array of vertices
    vertex vertices[numVertices];
    for (int i = 0; i < numVertices; i++){
        fscanf(file, "%s %lf %lf", vertices[i].label, &vertices[i].lattitude, &vertices[i].longitude);
    }

    // create array of edges
    edge edges[numEdges];
    for (int i = 1; i < numEdges; i++){
        int end1, end2;
        char location;
        fscanf(file, "%d %d %c", &end1, &end2, &location);
        edges[i].end1 = &vertices[end1];
        edges[i].end2 = &vertices[end2];
        edges[i].location = location;
    }

    // create adjacency list
    edgelist *adjList[numVertices];
    build_adj_list(vertices, edges, adjList, numVertices, numEdges);

    // close file
    fclose(file);

    // run Dijkstra's algorithm
    dijkstra(&vertices[0], &vertices[1]);

    return 0;
}
