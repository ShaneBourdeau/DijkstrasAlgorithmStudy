import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * non-public class to encapsulate a distance and an edge for use 
 * as elements of the priority queue underlying Dijkstra's algorithm.
 *
 * @author Jim Teresco
 */
class PQEntry implements Comparable<PQEntry> {

	/** the cumulative distance to the destination of the edge */
	protected double totalDist;
	/** the edge to be considered for addition to the collection
	of shortest paths */
	protected HighwayEdge lastEdge;

	/**
	 * Construct a PQEntry
	 *
	 * @param totalDist the total distance should this edge be added
	 * @param lastEdge the candidate edge for inclusion in the collection of
	 *        shortest paths
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
	 * 1 if this PQEntry's cumulative distance is greater that other's<br>
	 * 0 if the cumulative distances are the same
	 */
	@Override
	public int compareTo(PQEntry other) {

	if (totalDist < other.totalDist) return -1;
	if (totalDist > other.totalDist) return 1;
	return 0;
	}
}

/**
 * Sample solution for Dijkstra's algorithm using METAL graph data.
 *
 * Please do not place this code in any publically accessible location,
 * as students will be required to write this for assignments.
 *
 * @author Jim Teresco
 */
public class Dijkstra {

	public static final boolean DEBUG = true;
	public static int numThreads;
	public static Thread threads[];
	public static ConcurrentLinkedQueue<PQEntry> pq = new ConcurrentLinkedQueue<>();
	public static Map<String, HighwayEdge> result = new HashMap<>();
	public static AtomicBoolean found = new AtomicBoolean(false);
	public static HighwayGraph g;
	public static HighwayVertex dest;

	/**
	 * The main method for the Dijkstra's algorithm driver program,
	 * which will take command-line parameters of a graph to read,
	 * starting and ending points for driving directions, and an
	 * optional filename to write a METAL ".pth" file to display
	 * the directions in HDX.
	 *
	 * @param args command-line parameters, which include<br>
	 * args[0]: name of graph file<br>
	 * args[1]: starting waypoint label<br>
	 * args[2]: destination waypoint label<br>
	 * args[3]: (optional) .pth file name for mappable route
	 */
	public static void main(String args[]) throws IOException {

	// check for command-line parameters
	if (args.length != 4) {
		System.err.println("Usage: java Dijkstra numThreads graphfile start destination");
		System.exit(1);
	}
	
	numThreads = Integer.parseInt(args[0]);
		// read in the file to construct the graph
		Scanner s = new Scanner(new File(args[1]));
		g = new HighwayGraph(s);
		s.close();
	if (DEBUG) {
		System.out.println("Successfully read graph " + args[1] + " with |V|=" + g.vertices.length + " |E|=" + g.numEdges);
	}

	// find the vertex objects for the starting and destination points
	HighwayVertex start = g.getVertexByName(args[2]);
	if (start == null) {
		System.err.println("No vertex found with label " + args[2]);
		System.exit(1);
	}
	if (DEBUG) {
		System.out.println("Start: " + start.label + ", vertexNum=" + start.vNum);
	}

	dest = g.getVertexByName(args[3]);
	if (dest == null) {
		System.err.println("No vertex found with label " + args[3]);
		System.exit(1);
	}
	if (DEBUG) {
		System.out.println("Dest: " + dest.label + ", vertexNum=" + dest.vNum);
	}

	// mark all vertices as unvisited
	g.markAllUnvisited();

	// entry for the start vertex in the result map
	result.put(start.label, null);

	// mark as visited
	start.visited = true;

	// initialize PQ with the places we can get to directly from
	// the start vertex
	HighwayEdge e = start.head;
	while (e != null) {
		if (DEBUG) {
		System.out.println("pq.add(" + e.length + ", edge to " + g.vertices[e.dest].label + " via " + e.label + ")");
		}
		pq.add(new PQEntry(e.length, e));
		e = e.next;
	}

	// create and start worker threads

	//start timer
	long startTime = System.currentTimeMillis();
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
	//stop timer
	long endTime = System.currentTimeMillis();

	System.out.println("Time taken: " + ((endTime - startTime) / 1000.0) + " seconds");
	
	// map is populated, find the sequence of edges to traverse
	// for the shortest path
	// build ArrayList that we will traverse in reverse to get
	// forward driving directions
	ArrayList<HighwayEdge> path = new ArrayList<HighwayEdge>();
	String current = dest.label;
	while (!current.equals(start.label)) {
		HighwayEdge hop = result.get(current);
		path.add(hop);
		current = g.vertices[hop.source].label;
	}
	
	DecimalFormat df = new DecimalFormat("0.00");

	System.out.println("Detailed directions:");
	// report results
	double totalLength = 0.0;
	for (int i = path.size() - 1; i >= 0; i--) {
		HighwayEdge hop = path.get(i);
		totalLength += hop.length;
		System.out.println("Travel from " + g.vertices[hop.source].label + " to " + g.vertices[hop.dest].label + " for " + df.format(hop.length) + " along " + hop.label + ", total " + df.format(totalLength));
	}

	// condense driving directions to ignore intersections where
	// we do not change roads
	System.out.println("Simplified directions:");
	int hopNum = path.size() - 1;
	totalLength = 0.0;
	while (hopNum >= 0) {
		HighwayEdge hop = path.get(hopNum);
		System.out.print("Travel from " + g.vertices[hop.source].label + " to ");
		String currentRoute = hop.label;
		double sameRouteLength = hop.length;
		while (hopNum > 0 && path.get(hopNum-1).label.equals(currentRoute)) {
		sameRouteLength += path.get(hopNum-1).length;
		hopNum--;
		}
		totalLength += sameRouteLength;

		System.out.println(g.vertices[path.get(hopNum).dest].label +
				   " for " + df.format(sameRouteLength) +
				   " miles along " + currentRoute + ", total "
				   + df.format(totalLength));
		hopNum--;
	}
	}
}

class WorkerThread extends Thread {

	protected int threadId;

	public WorkerThread(int threadId) {
	this.threadId = threadId;
	}

	@Override
	public void run() {
	while (!Dijkstra.found.get()) {
		PQEntry nextPQ = Dijkstra.pq.poll();
		if (nextPQ == null) {
		continue;
		}
		HighwayEdge nextEdge = nextPQ.lastEdge;
		HighwayVertex destVertex = Dijkstra.g.vertices[nextEdge.dest];
		synchronized (destVertex) {
		if (destVertex.visited) {
			continue;
		}
		destVertex.visited = true;
		}
		Dijkstra.result.put(destVertex.label, nextEdge);
		if (destVertex.vNum == Dijkstra.dest.vNum) {
		Dijkstra.found.set(true);
		return;
		}
		HighwayEdge e = destVertex.head;
		while (e != null) {
		if (!Dijkstra.g.vertices[e.dest].visited) {
			synchronized (Dijkstra.pq) {
				Dijkstra.pq.add(new PQEntry(nextPQ.totalDist + e.length, e));
			}
			System.out.println("Thread " + threadId + " added " + e.length + " to " + Dijkstra.g.vertices[e.dest].label + " via " + e.label);
		}
		e = e.next;
		}
	}
	}
}
