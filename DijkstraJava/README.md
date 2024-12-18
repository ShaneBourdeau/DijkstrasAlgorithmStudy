# DijkstraJava
<p>Using a non-parallelized version of Dijkstra's algorthim from the METAL project as a baseline, this directory contains a completed parallelized version of the algorithm using Java threads</p>

<ul>
    <li>Dijkstra.java: Main implementation of Dijkstras algorthim </li>
    <li>HighwayEdge.java: Constructs edges</li>
    <li>HighwayVertex.java: Constructs vertexs</li>
    <li>HighwayGraph.java: An undirected, adjacency-list based graph data structure developed specifically for METAL highway mapping graphs.</li>
    <li>LatLng.java: Small data structure representing a latitude-longitude pair.</li>
    <li>.tmg files: Input files consisting of the road name and latitude-longitude pair needed to compute the algorthim</li>
</ul>

<p>To run program:</p>  

1) run `javac Dijkstra.java`  
2) run `java Dijkstra numThreads graphfile start destination`  

`numThreads`: Number of threads  
`graphfile`: name of graph file (.tmg)  
`start`: Starting vertex label  
`destination`: Ending vertex label  

