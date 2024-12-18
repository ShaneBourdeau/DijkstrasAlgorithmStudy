import java.io.File;
import java.io.IOException;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.locks.ReentrantLock;

/**
 * non-public class to encapsulate a distance and an edge for use
 * as elements of the priority queue underlying Dijkstra's algorithm.
 *
 * @author Jim Teresco
 */
class PQEntry implements Comparable<PQEntry> {

    // the cumulative distance to the destination of the edge
    protected double totalDist;

    // the edge to be considered for addition to the collection of shortest paths
    protected HighwayEdge lastEdge;

    /**
     * Construct a PQEntry
     *
     * @param totalDist the total distance should this edge be added
     * @param lastEdge  the candidate edge for inclusion in the collection of
     *                  shortest paths
     */
    public PQEntry(double totalDist, HighwayEdge lastEdge) {
        this.totalDist = totalDist;
        this.lastEdge = lastEdge;
    }

    /**
     * Compare this PQEntry to another for priority queue ordering.
     * Here, we want higher priority given to the smaller cumulative
     * distance.
     *
     * @param other the PQEntry to which this PQEntry is to be compared
     *
     * @return -1 if this PQEntry's cumulative distance is smaller that other's<br>
     *         1 if this PQEntry's cumulative distance is greater that other's<br>
     *         0 if the cumulative distances are the same
     */
    @Override
    public int compareTo(PQEntry other) {
        if (totalDist < other.totalDist)
            return -1;
        if (totalDist > other.totalDist)
            return 1;
        return 0;
    }
}

/**
 * Computes shortest paths in a graph using Dijkstra's algorithm and Java threads.
 *
 * @author Orginal Verison: Jim Teresco
 * @author Modified for parallel version: Tyler Knapp and Shane Bourdeau
 */
public class Dijkstra {

	// global variables
    public static int numThreads;
    public static Thread threads[];
    public static ConcurrentLinkedQueue<PQEntry> pq = new ConcurrentLinkedQueue<>();
    public static Map<String, HighwayEdge> result = new HashMap<>();
    public static AtomicBoolean found = new AtomicBoolean(false);
    public static HighwayGraph g;
    public static HighwayVertex dest;
	public static ReentrantLock resultLock = new ReentrantLock();

    /**
     * The main method for the Dijkstra's algorithm driver program,
     * which will take command-line parameters of a graph to read,
     * starting and ending points for driving directions
     *
     * @param args command-line parameters, which include<
     *             args[0]: number of threads<br>
     *             args[1]: name of graph file<br>
     *             args[2]: starting waypoint label<br>
     *             args[3]: destination waypoint label<br>
     */
    public static void main(String args[]) throws IOException {

        // check for command-line parameters
        if (args.length != 4) {
            System.err.println("Usage: java Dijkstra numThreads graphfile start destination");
            System.exit(1);
        }

		// start timer
        long startTime = System.currentTimeMillis();
        
		// initialize threads
		numThreads = Integer.parseInt(args[0]);

		// read the graph
        Scanner s = new Scanner(new File(args[1]));
        g = new HighwayGraph(s);
        s.close();

        // find the vertex objects for the starting and destination points
        HighwayVertex start = g.getVertexByName(args[2]);
        if (start == null) {
            System.err.println("No vertex found with label " + args[2]);
            System.exit(1);
        }
        dest = g.getVertexByName(args[3]);
        if (dest == null) {
            System.err.println("No vertex found with label " + args[3]);
            System.exit(1);
        }

        // mark all vertices as unvisited
        g.markAllUnvisited();

        // entry for the start vertex in the result map
        result.put(start.label, null);

        // mark as visited
        start.visited = true;

        // initialize PQ with the places we can get to directly from the start vertex
        HighwayEdge e = start.head;
        while (e != null) {
            if (g.vertices[e.dest] != null) { 
                pq.add(new PQEntry(e.length, e));
            }
            e = e.next;
        }

        // create and start worker threads
        threads = new Thread[numThreads];
        for (int i = 0; i < numThreads; i++) {
            threads[i] = new WorkerThread(i);
            threads[i].start();
        }

        // wait for all threads to finish
        for (int i = 0; i < numThreads; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException ex) {
                ex.printStackTrace();
            }
        }

        // map is populated, find the sequence of edges to traverse
        // for the shortest path
        // build ArrayList that we will traverse in reverse to get
        // forward driving directions
        ArrayList<HighwayEdge> path = new ArrayList<HighwayEdge>();
        String current = dest.label;
        while (!current.equals(start.label)) {
            HighwayEdge hop = result.get(current);
            if (hop == null) {
				//if no path is found, print error and exit
                System.err.println("Graph is disconnected, no path from " + start.label + " to " + dest.label);
                System.exit(1);
            }
            path.add(hop);
            current = g.vertices[hop.source].label;
        }

		// format the output
        DecimalFormat df = new DecimalFormat("0.00");
		double totalLength = 0.0;
        
		// print out the shortest path
        System.out.println("Shortest Path:");
        int hopNum = path.size() - 1;
        totalLength = 0.0;
        while (hopNum >= 0) {
            HighwayEdge hop = path.get(hopNum);
            if (hop == null) {
                hopNum--;
                continue;
            }
            System.out.print("Travel from " + g.vertices[hop.source].label + " to ");
            String currentRoute = hop.label;
            double sameRouteLength = hop.length;
            while (hopNum > 0 && path.get(hopNum - 1) != null && path.get(hopNum - 1).label.equals(currentRoute)) {
                hopNum--;
                hop = path.get(hopNum);
                sameRouteLength += hop.length;
            }
            System.out.println(g.vertices[hop.dest].label + " for " + df.format(sameRouteLength) + " miles along " + currentRoute);
            totalLength += sameRouteLength;
            hopNum--;
            if (hopNum < 0) {
                System.out.println("Total length: " + df.format(totalLength) + " miles");
            }
        }

		//end timer and print out the time taken to compute
        long endTime = System.currentTimeMillis();
        System.out.println("Time taken to compute: " + (endTime - startTime) + "ms");
    }
}

/**
 * WorkerThread class that extends Thread and processes edges and adds them to the priority queue
 * @author Tyler Knapp and Shane Bourdeau
 */
class WorkerThread extends Thread {

	//thread id
    protected int threadId;

	/**
	 * Constructor for WorkerThread
	 * @param threadId the id of the thread
	 */
    public WorkerThread(int threadId) {
		//set the thread id
        this.threadId = threadId;
    }

	/**
	 * Process edges and add them to the priority queue
	 */
    @Override
    public void run() {
        while (!Dijkstra.found.get()) {
			//if there are no more edges to process, continue
            PQEntry nextPQ = Dijkstra.pq.poll();
            if (nextPQ == null) {
                continue;
            }
			//if the edge is null, continue
            HighwayEdge nextEdge = nextPQ.lastEdge;
            if (nextEdge == null) {
                continue;
            }
			//if the destination vertex is null, continue
            HighwayVertex destVertex = Dijkstra.g.vertices[nextEdge.dest];
            if (destVertex == null) {
                continue;
            }
			//if the destination vertex is already visited, continue and synchronize
			synchronized (destVertex) {
                if (destVertex.visited) {
                    continue;
                }
                destVertex.visited = true;
            }
			//add the edge to the result map and lock map to prevent thread racing
            Dijkstra.resultLock.lock();
            try {
                Dijkstra.result.put(destVertex.label, nextEdge);
            } finally {
				//unlock the map
                Dijkstra.resultLock.unlock();
            }
			//if the destination vertex is the destination, set found to true and return
            if (destVertex.vNum == Dijkstra.dest.vNum) {
                Dijkstra.found.set(true);
                return;
            }
			//add the edges of the destination vertex to the priority queue
            HighwayEdge e = destVertex.head;
            while (e != null) {
                if (Dijkstra.g.vertices[e.dest] != null && !Dijkstra.g.vertices[e.dest].visited) {
					//syncrhonize the priority queue to prevent thread racing, and print out what each thread added to the queue
                    synchronized (Dijkstra.pq) {
                        Dijkstra.pq.add(new PQEntry(nextPQ.totalDist + e.length, e));
						System.out.println("Thread " + threadId +" to " + Dijkstra.g.vertices[e.dest].label + " via " + e.label);
                    }
                }
				//move to the next edge
                e = e.next;
            }
        }
    }
}