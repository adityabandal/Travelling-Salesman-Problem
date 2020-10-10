/* The code is for demonstrating A* search with MST heuristics on the
 * travelling salesman problem.
 * The MST(Minimum Spanning Tree) is implemented using Prim's algorithm.
 * CS683 Artificial Intelligence
 * Author: Wei Hong, Liu Yang
 * UMass Amherst, School of Computer Science
 * Copyright 2014
 */

package cs683.hw1.TSP;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.PriorityQueue;

public class TSPWithMST {	
	static double[][] distanceMap;
	//problem generator
	//Randomly generate a couple of unique (x,y) coordinates, each is in between (0, 1)
	public static ArrayList<Node> probGenerator(int sizeOfProblem){
		//use a hashmap as a buffer to store randomly generated nodes
		HashMap<Node,Integer> nodeCache = new HashMap<Node,Integer>();
		//terminate when the number of nodes meets demand
	    int count = 0;
		while(nodeCache.size()<sizeOfProblem){
			double x = Math.random();
			double y = Math.random();
			nodeCache.put(new Node(x,y), 1);
		}
		//convert the keyset into an arraylist
	    ArrayList<Node> nodes = new ArrayList<Node>(nodeCache.keySet());
		return nodes;		
	}
	//Generator an Minimum Spanning Tree (MST) over a set of nodes
	//Use Prim's algorithm
	
	public static double genMST(ArrayList<Node> nodes){
		//adjacent matrix to implement the fully connected graph
		int size = nodes.size();
		//initialize 
		final double[] dist = new double[size];
		final int[] pred = new int[size];
		final boolean[] visited = new boolean[size];
		//set distance to infinity
		for(int i = 0; i < dist.length; i++){
			dist[i] = Double.MAX_VALUE;
		}
		//starting from the first node, can be changed.
		dist[0] = 0;
		
		for(int i = 0; i < dist.length; i++){			
			final int next = minDist(dist, visited);
		//	System.out.println(nodes.get(next).x + " " + nodes.get(next).y);
			visited[next] = true;
			//find all unvisited neighbors of the current node
			for(int j = 0; j < size; j++){
				if(j != next && !visited[j]){
					double d = distanceMap[next][j];
					//update distance and pred array
					if(dist[j] > d ){
						dist[j] = d;
						pred[j] = next;
					}							
				}
			}
		}
		
	
		//return pred;	
		//calculate the sum of edge values of the minimum spanning tree
		double score = 0.0;
		for(int i = 0; i < pred.length; i++){
			score = distanceMap[i][pred[i]] + score;
		}
		
		return score;
	}
	//Traverse the distance array to obtain the nearest node to the explored set.
	//Can be optimized, e.g. use heap. 
	private static int minDist(double[] dist, boolean[] v){
		double x = Double.MAX_VALUE;
		int y = -1;
		for(int i = 0; i < dist.length; i++){
			if(!v[i] && dist[i] < x){
				y = i;
				x = dist[i];
			}
		}
		return y;
	}
	
	//calculate the Euclidean distance between two nodes
	private static double distance(Node n1, Node n2){
		return Math.sqrt(Math.pow((n1.x - n2.x),2) + Math.pow((n1.y - n2.y),2));		
	}
	
	//A* Search use MST heuristics
	public static int[] AStar(ArrayList<Node> nodes){	
		double tic = System.currentTimeMillis();
		
		int numOfNodes = 0;
		//Initialization
		//Use an integer array to represent state, if the ith element is 1, it means the ith node has been expanded.
		//otherwise the ith node has not been expanded yet.
		ArrayList<ANode> closedList = new ArrayList<ANode>();
		PriorityQueue<ANode> openList = new PriorityQueue<ANode>();
		int size = nodes.size();
		//int[] visited = new int[size];
		int[] initialPath = {0};
		ANode start = new ANode(initialPath,nodes.get(0),0,0,0);
		openList.add(start);
		while(openList.size()>0){
			ANode current = openList.remove();
			numOfNodes ++;
			int[] path = current.path;
			if(path!=null){
			int plen = path.length;
			if(plen == size){
				double toc = (System.currentTimeMillis() - tic); 
			//	System.out.println(Integer.toString(plen) + "\t" + Integer.toString(numOfNodes) + "\t" + toc);
				return current.path;}// a TSP path has been found
		    //find all neighbors of current node
            int[] neighbors = new int[size];
            Arrays.fill(neighbors, 1);
		    //neighbors: nodes not in the path			
		    for(int j : path){
			   neighbors[j] = 0;
			}
		    for(int i = 0; i < size; i++){
		    	if(neighbors[i] != 0){// is a neighbor
		    		//calculate h value
		    		ArrayList<Node> remainingNodes = new ArrayList<Node>();
                    for(int k = 0; k < size; k++){
                    	if(neighbors[k] == 1 && k != i) remainingNodes.add(nodes.get(k));
                    }
                    double hValue;
                    if(remainingNodes.size() == 0){ 
                    	hValue = 0.0;
                    }else{
                        hValue = genMST(remainingNodes);
                    }
		    		//calculate g value
                    double gValue = current.gValue + distanceMap[current.index][i];
                    double fValue = gValue + hValue;	
                    //extend the path
		    		int[] newPath = new int[plen+1];
                    for(int j = 0; j < plen; j++){
                    	newPath[j] = path[j];
                    }
                    newPath[plen] = i;

                    //generate new node
                    ANode an = new ANode(newPath,nodes.get(i),i,fValue,gValue);
                    //put into openlist
                    openList.add(an);
                    
		    	}
		      }
		    closedList.add(current);
           }
		}
		
		return null;
	}
	
	
	public static void main(String[] args) throws IOException{
		/*for(int i = 1; i < 25; i++){
	    ArrayList<Node> nodes = probGenerator(i);
	    for(Node n:nodes){
	  //  	System.out.println(n.x + " " + n.y);	    	
	    }
		int[] path = AStar(nodes);
		if(path != null){
		 
			
		 for(int i : path){
			System.out.println(i);
		 }
		}
	  }*/
		InputStream    fis;
		BufferedReader br;
		String         line;
        ArrayList<Node> cities = new ArrayList<Node>();
		fis = new FileInputStream("WS_cities.txt");
		br = new BufferedReader(new InputStreamReader(fis, Charset.forName("UTF-8")));
		while ((line = br.readLine()) != null) {
		    // Deal with the line
			String[] tokens = line.split("\\s");
			double x = Double.parseDouble(tokens[1]);
			double y = Double.parseDouble(tokens[2]);
			Node city = new Node(x,y);
			cities.add(city);
			
		}
		distanceMap = new double[cities.size()][cities.size()];
		for(int i = 0; i < cities.size(); i++ ){
			for(int j = 0; j < i; j++){
				distanceMap[i][j] = distance(cities.get(i), cities.get(j));
				distanceMap[j][i] = distanceMap[i][j];
			}
		}
		for(int i = 0; i < cities.size(); i++ ){
			distanceMap[i][i] = 0 ;			
		}
	    
		// Done with the file
		br.close();
		br = null;
		fis = null;
		int path[] = AStar(cities);
		try {
	          File file = new File("path.txt");
	          BufferedWriter output = new BufferedWriter(new FileWriter(file));
	          for(int i : path){
	        	  output.write((i+1) + "\t"+ cities.get(i).x + "\t" + cities.get(i).y +"\n");
	          }	          
	          output.close();
	        } catch ( IOException e ) {
	           e.printStackTrace();
	        }		
	}
}