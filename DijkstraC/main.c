#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    HighwayVertex *vertices = (HighwayVertex*)malloc(numVertices * sizeof(HighwayVertex));
    for (int i = 0; i < numVertices; i++){
        vertices[i].vNum = i;
        vertices[i].visited = 0;
        vertices[i].head = NULL;
    }

    // read in vertices
    for (int i = 0; i < numVertices; i++){
        fscanf(file, "%s", vertices[i].label);
    }

    // // read in edges
    // for (int i = 0; i < numEdges; i++){
    //     int source, dest;
    //     double length;
    //     char label[50];
    //     fscanf(file, "%d %d %lf %s", &source, &dest, &length, label);

    //     // create new edge
    //     HighwayEdge *newEdge = (HighwayEdge*)malloc(sizeof(HighwayEdge));
    //     newEdge->source = source;
    //     newEdge->dest = dest;
    //     newEdge->length = length;
    //     strcpy(newEdge->label, label);
    //     newEdge->next = vertices[source].head;
    //     vertices[source].head = newEdge;
    // }

    // close file
    fclose(file);

    // run Dijkstra's algorithm
    Dijkstra(vertices, numVertices, 0, numVertices - 1);


}
