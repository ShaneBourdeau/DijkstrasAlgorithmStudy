// Structure for a vertex in the graph
typedef struct vertex {
    char label[50];      
    double lattitude, longitude;         
    void* extra;         
} vertex;

// Structure for an edge in the graph
typedef struct edge {
    struct vertex* end1; 
    struct vertex* end2; 
    double length;       
} edge;

// Structure to represent a list of edges for a vertex
typedef struct edgelist {
    struct edge* edge;
    struct edgelist* next;
} edgelist;

// Structure for a path in Dijkstra's algorithm
typedef struct dijkstra_path {
    struct vertex* vertex;
    struct edge* edge;
    double length;
} dijkstra_path;

extern void tmg_dijkstra(tmg_vertex* start, tmg_vertex* end);
