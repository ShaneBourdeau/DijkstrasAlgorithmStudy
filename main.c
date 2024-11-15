#include <stdio.h>
#include <mpi.h>
// Dijkstra's algorithm implementation

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);//starts MPI
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
  FILE *f = fopen(argv[1], "r");
  if (f == NULL) { // returns an error if the file can't be opened
    perror("Cannot open file");
    return EXIT_FAILURE;
    exit(1);
  }
  while (1) {
    waypoint *currentWaypoint = malloc(sizeof(waypoint));
    int result = fscanf(f, "%255s %lf %lf", vertex->label, vertex->latitude, &vertext->longitude);
    if (result == 3) {
      /*
        northernMost = findNorthernMost(northernMost,currentWaypoint->label, currentWaypoint->latitude, currentWaypoint->longitude);
        southernMost = findSouthernMost(southernMost, currentWaypoint->label, currentWaypoint->latitude, currentWaypoint->longitude);
        easternMost = findEasternMost(easternMost, currentWaypoint->label, currentWaypoint->latitude, currentWaypoint->longitude);
        westernMost = findWesternMost(westernMost, currentWaypoint->label, currentWaypoint->latitude, currentWaypoint->longitude);  
      */
      return 0;
    }
  }
  MPI_Finalize();
}
