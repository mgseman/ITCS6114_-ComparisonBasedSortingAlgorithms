#pragma once
#include <stdio.h>
#include <stdlib.h>

//Structure for minimum heap node
struct MinHeapNode
{
	int v;
	int distance;
};

//Structure for minimum heap
struct MinHeap
{
	//Number of heap nodes
	int size;

	//Capacity of heap nodes
	int capacity;

	//Position of heap node for decreasing key
	int* pos;
	struct MinHeapNode** array;
};

//Function to create new minimum heap node
struct MinHeapNode* newMinHeapNode(int v, int distance)
{
	struct MinHeapNode* minHeapNode = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
	minHeapNode->v = v;
	minHeapNode->distance = distance;
	return minHeapNode;
}

//Function to create a minimum heap
struct MinHeap* createMinHeap(int capacity)
{
	struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
	minHeap->pos = (int*)malloc(capacity * sizeof(int));
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array = (struct MinHeapNode**)malloc(capacity * sizeof(struct MinHeapNode*));

	return minHeap;
}

//Function to swap nodes in a minimum heap
void swapMinHeapNodes(struct MinHeapNode** a, struct MinHeapNode** b)
{
	struct MinHeapNode* temp = *a;
	*a = *b;
	*b = temp;
}

//Minimum heapify function at given index updating node positions
void minHeapify(struct MinHeap* minHeap, int index)
{
	int smallest = index;
	int left = 2 * index + 1;
	int right = 2 * index + 2;

	if (left < minHeap->size && minHeap->array[left]->distance < minHeap->array[smallest]->distance) {
		smallest = left;
	}

	if (right < minHeap->size && minHeap->array[right]->distance < minHeap->array[smallest]->distance) {
		smallest = right;
	}

	if (smallest != index)
	{
		//Nodes to be swapped in minimum heap
		MinHeapNode* smallestNode = minHeap->array[smallest];
		MinHeapNode* indexNode = minHeap->array[index];

		//Swap node positions
		minHeap->pos[smallestNode->v] = index;
		minHeap->pos[indexNode->v] = smallest;

		//Swap nodes
		swapMinHeapNodes(&minHeap->array[smallest], &minHeap->array[index]);

		//recursively minimum heapify
		minHeapify(minHeap, smallest);
	}
}

//Check if minHeap is empty
bool isEmpty(struct MinHeap* minHeap)
{
	return minHeap->size == 0;
}

//Function to extract minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
	if (isEmpty(minHeap)) {
		return NULL;
	}
	
	//Root node
	struct MinHeapNode* root = minHeap->array[0];

	//Replace root node with last node
	struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
	minHeap->array[0] = lastNode;

	//Update position of last node
	minHeap->pos[root->v] = minHeap->size - 1;
	minHeap->pos[lastNode->v] = 0;

	//Decrement heap size and heapify root node
	--minHeap->size;
	minHeapify(minHeap, 0);

	return root;
}

//Function to decrease distance value of vertex v
void decreaseDistance(struct MinHeap* minHeap, int v, int distance)
{
	//Get index of v in heap array
	int i = minHeap->pos[v];

	//Get node and update distance value
	minHeap->array[i]->distance = distance;

	//Travel up until tree is heapified
	//Complexity O(logn)
	while (i && minHeap->array[i]->distance < minHeap->array[(i - 1) / 2]->distance)
	{
		//Swap current node with parent node
		minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
		minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;

		swapMinHeapNodes(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

		//move parent to index
		i = (i - 1) / 2;
	}
}

//Check if vertex v is in minimum heap
bool isInMinHeap(struct MinHeap* minHeap, int v)
{
	if (minHeap->pos[v] < minHeap->size) {
		return true;
	}
	return false;
}