#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "AdjacencyList.h"
#include "MinimumHeap.h"

char alphabet[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
				   'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };

//Print path for dijkstra's algorithm
void printPath(int parent[], int j)
{
	if (parent[j] == -1) {
		return;
	}
	printPath(parent, parent[j]);
	std::cout << alphabet[j] << " ";
}

//Print solution to dijkstra's algorithm
void printSolDij(int distance[], int V, int parent[])
{
	int source = 0;
	std::cout << "Vertex\tCost\tPath";
	for (int i = 1; i < V; i++) {
		std::cout << "\n" << alphabet[source] << "->" << alphabet[i]
			<< "\t" << distance[i] << "\t" << alphabet[source] << " ";
		printPath(parent, i);
	}
	std::cout << "\n" << std::endl;
}

//Print solution to Prim MST
void printSolPrim(int distance[], int V, int parent[])
{
	int source = 0;
	int totCost = 0;
	std::cout << "Edges\tCost";
	for (int i = 1; i < V; i++) {
		std::cout << "\n" << alphabet[parent[i]] << "->" << alphabet[i]
			<< "\t" << distance[i] << " ";
		totCost += distance[i];
	}
	std::cout << "\nTotal Cost of Minimum Spanning Tree: "<< totCost << std::endl;
	std::cout << std::endl;
}

//Dijkstra's Shortest Path Algorithm
//Calculates shortest path from source to all other vertices
//Complexity O(ElogV)
void dijkstra(struct Graph* graph, int source)
{
	//Extract number of vertices in graph
	int V = graph->V;

	const int size = V;

	//Array of distance values
	int* distance = new int[V];

	//Parent array, Stores shortest path
	int* parent = new int[V];

	//minHeap of edges set
	struct MinHeap* minHeap = createMinHeap(V);

	//Initialize minimum heap with vertices and their distance values
	for (int i = 0; i < V; ++i) {
		parent[i] = -1;
		distance[i] = INT_MAX;
		minHeap->array[i] = newMinHeapNode(i, distance[i]);
		minHeap->pos[i] = i;
	}

	//Set distance value of source vertex to zero
	minHeap->array[source] = newMinHeapNode(source, distance[source]);
	minHeap->pos[source] = source;
	distance[source] = 0;
	decreaseDistance(minHeap, source, distance[source]);

	//Initialize minimum heap size to number of vertices V
	minHeap->size = V;

	//Loop finds minimum distance of all nodes removing them
	//from minimum heap once calculated
	//Loop ends once all shortest distance are calculated
	while (!isEmpty(minHeap))
	{
		//Extract vertex with smallest distance and store in u
		struct MinHeapNode* minHeapNode = extractMin(minHeap);
		int u = minHeapNode->v;

		//Traverse adjacent vertices of u and update distance values
		struct AdjListNode* posTrav = graph->array[u].head;

		while (posTrav != NULL)
		{
			int v = posTrav->destination;

			//Check is distance to v is finalized and distance to v
			//through u is less than previous distance
			if (isInMinHeap(minHeap, v) && distance[u] != INT_MAX && posTrav->weight + distance[u] < distance[v])
			{
				parent[v] = u;
				distance[v] = distance[u] + posTrav->weight;

				//Update distance value in minHeap
				decreaseDistance(minHeap, v, distance[v]);
			}
			posTrav = posTrav->next;
		}
	}

	//Print shortest distance
	printSolDij(distance, V, parent);

}

//Prim's Minimum Spanning Tree Algorithm
//Complexity O(ElogV)
void PrimMST(struct Graph* graph, int source)
{
	//Number of vertices from graph
	int V = graph->V;

	//Array to store MST
	int* arrMST = new int[V];

	//Array to store minimum weight values for each vertex
	int* minWeights = new int[V];

	//minHeap of edges set
	struct MinHeap* minHeap = createMinHeap(V);

	//Initialize minHeap with vertices
	//minWeights of vertices set to INT_MAX
	for (int i = 1; i < V; i++) {
		arrMST[i] = -1;
		minWeights[i] = INT_MAX;
		minHeap->array[i] = newMinHeapNode(i, minWeights[i]);
		minHeap->pos[i] = i;
	}

	//Set source vertex minWeight to zero
	minWeights[source] = 0;
	minHeap->array[source] = newMinHeapNode(source, minWeights[source]);
	minHeap->pos[source] = source;

	//Set size of minHeap equal to number of vertices
	minHeap->size = V;

	//minHeap removes nodes once they are added to the MST
	//while loop continues until minHeap is empty
	while (!isEmpty(minHeap))
	{
		//Find vertex with minimum weight value and store in u
		struct MinHeapNode* minHeapNode = extractMin(minHeap);
		int u = minHeapNode->v;

		//Traverse through adjacent vertices of u updating minimum weights
		struct AdjListNode* posTrav = graph->array[u].head;
		while (posTrav != NULL) {
			int v = posTrav->destination;

			//If v is not included in MST and weight from u->v is less than
			//the minimum weight of v then update v's min weight and parent
			if (isInMinHeap(minHeap, v) && posTrav->weight < minWeights[v]) {
				minWeights[v] = posTrav->weight;
				arrMST[v] = u;
				decreaseDistance(minHeap, v, minWeights[v]);
			}
			posTrav = posTrav->next;
		}
	}

	//Print solution MST
	printSolPrim(minWeights, V, arrMST);
}

void dijkstraFromFile(std::string fname)
{
	std::ifstream file(fname);
	std::string line;

	if (!file.is_open()) {
		std::cerr << "Could not open the file - '" << fname << "'. Please ensure it is in the project directory." << std::endl;
		return;
	}

	int lineCount = 0;

	int vertices, edges, weight;
	char graphType;
	char source, destination;
	std::vector<int> sources, destinations, weights;

	while (getline(file, line))
	{
		std::istringstream ss(line);
		if (lineCount == 0) {
			while (ss >> vertices >> edges >> graphType) {
				//std::cout << vertices << edges << graphType;
			}
		}
		else if (lineCount == int(edges) + 1) {
			while (ss >> source) {
				//std::cout << source;
			}
		}
		else {
			while (ss >> source >> destination >> weight) {
				//std::cout << source << destination << weight;
				sources.push_back(int(source) - 65);
				destinations.push_back(int(destination) - 65);
				weights.push_back(weight);
			}
		}
		lineCount++;
		//std::cout << '\n';
	}

	std::cout << "Shortest Path for Graph: " << fname << '\n';

	//Create the graph
	struct Graph* graph = createGraph(vertices);
	if (graphType == 'D') {
		for (int i = 0; i < edges; i++) {
			addEdgeDirected(graph, sources[i], destinations[i], weights[i]);
		}
	}
	else if (graphType == 'U') {
		for (int i = 0; i < edges; i++) {
			addEdgeUndirected(graph, sources[i], destinations[i], weights[i]);
		}
	}
	
	dijkstra(graph, int(source) - 65);

}

void primMSTFromFile(std::string fname)
{
	std::ifstream file(fname);
	std::string line;

	if (!file.is_open()) {
		std::cerr << "Could not open the file - '" << fname << "'. Please ensure it is in the project directory." << std::endl;
		return;
	}

	int lineCount = 0;

	int vertices, edges, weight;
	char graphType;
	char source, destination;
	std::vector<int> sources, destinations, weights;

	while (getline(file, line))
	{
		std::istringstream ss(line);
		if (lineCount == 0) {
			while (ss >> vertices >> edges >> graphType) {
				//std::cout << vertices << edges << graphType;
			}
		}
		else if (lineCount == int(edges) + 1) {
			while (ss >> source) {
				//std::cout << source;
			}
		}
		else {
			while (ss >> source >> destination >> weight) {
				//std::cout << source << destination << weight;
				sources.push_back(int(source) - 65);
				destinations.push_back(int(destination) - 65);
				weights.push_back(weight);
			}
		}
		lineCount++;
		//std::cout << '\n';
	}

	std::cout << "Prim Minimum Spanning Tree for Graph: " << fname << '\n';

	//Create the graph
	struct Graph* graph = createGraph(vertices);
	if (graphType == 'D') {
		std::cout << "ERROR: Prim's algorithm fails for directed graphs.\nPlease load an undirected graph to use Prim's MST.\n";
	}
	else if (graphType == 'U') {
		for (int i = 0; i < edges; i++) {
			addEdgeUndirected(graph, sources[i], destinations[i], weights[i]);
		}

		PrimMST(graph, int(source) - 65);
	}

}

int main()
{
	dijkstraFromFile("projectExample.txt");
	dijkstraFromFile("undirected1.txt");
	dijkstraFromFile("directed1.txt");
	dijkstraFromFile("undirected2.txt");
	dijkstraFromFile("directed2.txt");

	primMSTFromFile("projectExample.txt");
	primMSTFromFile("undirected1.txt");
	primMSTFromFile("undirected2.txt");
	primMSTFromFile("undirected3.txt");
	primMSTFromFile("undirected4.txt");

	char choice;
	std::string fname;
	std::cout << "Please choose which algorithm to run (p - Prim MST, d - Dijkstra).\n";
	while (std::cin >> choice) {
		if (choice == 'd') {
			std::cout << "Please input the file name (name.txt) to run Dijkstra on.\n";
			std::cin >> fname;
			dijkstraFromFile(fname);
		}
		else if (choice == 'p') {
			std::cout << "Please input the file name (name.txt) to run Prim MST on.\n";
			std::cin >> fname;
			primMSTFromFile(fname);
		}
		else {
			std::cout << "Please input a valid choice: p - Prim MST, d - Dijkstra.\n";
		}
	}

	return 0;
}