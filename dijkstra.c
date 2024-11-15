#include <stdio.h>
#include <stdlib.h>
#include "pq.h"

// Dijkstra's algorithm implementation
void dijkstra(vertex* start, vertex* end) {
    printf("Start: %s, End: %s\n", start->label, end->label);

    struct pq* list = create_pq();
    int size = 10;
    dijkstra_path* found = malloc(size * sizeof(dijkstra_path));
    int current_found = 0;

    // Initialize start path
    dijkstra_path* strt = malloc(sizeof(dijkstra_path));
    strt->vertex = start;
    strt->length = 0.0;
    strt->edge = NULL;

    void* t = (void*)strt;
    enqueue(list, t, strt->length);

    int finding = 1;
    dijkstra_path* current;

    while (finding) {
        current = (dijkstra_path*)dequeue(list);

        if (current_found >= size - 1) {
            size *= 2;
            found = realloc(found, size * sizeof(dijkstra_path));
        }
        found[current_found] = *current;
        current_found++;

        if (current->vertex == end) finding = 0;
        current->vertex->extra = (void*)current;

        edgelist* f = current->vertex->extra;  // Edgelist stored in `extra`
        while (f != NULL) {
            dijkstra_path* newP = malloc(sizeof(dijkstra_path));
            newP->length = current->length + f->edge->length;
            newP->edge = f->edge;

            if (f->edge->end1 == current->vertex) {
                newP->vertex = f->edge->end2;
            } else {
                newP->vertex = f->edge->end1;
            }

            void* t = (void*)newP;
            if (newP->vertex->extra == NULL) enqueue(list, t, newP->length);
            f = f->next;
        }
    }

    printf("Paths found: %d\n", current_found);
    for (int i = 0; i < current_found; i++) {
        printf("%s : %f\n", found[i].vertex->label, found[i].length);
        dijkstra_path* temp = &found[i];
        edge* f = found[i].edge;

        if (found[i].edge == NULL) continue;

        if (f->end1 == found[i].vertex) {
            temp = (dijkstra_path*)f->end2->extra;
        } else {
            temp = (dijkstra_path*)f->end1->extra;
        }

        while (temp != NULL) {
            edge* f = temp->edge;
            printf("%s, ", temp->vertex->label);
            if (f == NULL) break;

            if (f->end1 == temp->vertex) {
                temp = (dijkstra_path*)(f->end2->extra);
            } else {
                temp = (dijkstra_path*)(f->end1->extra);
            }
        }
        printf("\n\n");
    }

    free(found);
    free(strt);
}
