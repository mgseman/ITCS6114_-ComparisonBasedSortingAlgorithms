#pragma once
#include <stdio.h>
#include <stdlib.h>

//Structure for nodes within an adjacency list
struct AdjListNode {
	int destination;
	int weight;
	struct AdjListNode* next;
};

//Structure for adjacency list
struct AdjList
{
	//Point to head node
	struct AdjListNode* head;
};

//Structure to represent the graph
//Array of adjacency lists
//Size = V (number of vertices)
struct Graph {
	//Number of vertices
	int V;
	struct AdjList* array;
};

//Function to create new adjacency list node
struct AdjListNode* newAdjListNode (int destination, int weight)
{
	struct AdjListNode* newNode = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
	newNode->destination = destination;
	newNode->weight = weight;
	newNode->next = NULL;
	return newNode;
}

//Function to create graph of V vertices
struct Graph* createGraph(int V)
{
	struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
	graph->V = V;

	//Create array of adjacency lists
	graph->array = (struct AdjList*)malloc(V * sizeof(struct AdjList));

	//Initialize heads of each adjacency list as empty(NULL)
	for (int i = 0; i < V; i++) {
		graph->array[i].head = NULL;
	}

	return graph;
}

//Function to add edge to a directed graph
void addEdgeDirected(struct Graph* graph, int source, int destination, int weight)
{
	//Add an edge from source to destination
	//New node is added to the adjacency list
	struct AdjListNode* newNode = newAdjListNode(destination, weight);
	newNode->next = graph->array[source].head;
	graph->array[source].head = newNode;
}

//Function to add edge to an undirected graph
void addEdgeUndirected(struct Graph* graph, int source, int destination, int weight)
{
	//Add an edge from source to destination
	//New node is added to the adjacency list
	struct AdjListNode* newNode = newAdjListNode(destination, weight);
	newNode->next = graph->array[source].head;
	graph->array[source].head = newNode;

	//Since graph is undirected add an edge from
	//destination to source too
	newNode = newAdjListNode(source, weight);
	newNode->next = graph->array[destination].head;
	graph->array[destination].head = newNode;
}