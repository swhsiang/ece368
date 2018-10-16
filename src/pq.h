#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/* This Priority Queue is implemented based on Binary Heap (Min Heap) */

typedef struct PQNode {
  char value;
  // Lower values indicate higher priority
  int priority;

  struct PQNode *left, *right;
} PQNode;

typedef struct PriorityQueue {
  int capacity;
  int heap_size;
  PQNode **heap_arr;
} PQueue;

PQueue *NewPriorityQueue(int capacity);

// Function to Create A New Node
PQNode *NewNode(int priority, char value, PQNode *left, PQNode *right);

bool IsLeaf(PQNode *node);

void MinHeapify(PQueue *pqueue, int index);

int Parent(int index);

int LeftChild(int index);

int RightChild(int index);

void ExtractMin(PQueue *pqueue, PQNode *node);

void DecreaseKey(PQueue *pqueue, int index, int newVal);

// Returns the minimum key (key at root) from Min heap
PQNode *GetMin(PQueue *pqueue);

// Deletes a key stored at index i
void DeleteKey(PQueue *pqueue, int index);

void InsertKey(PQueue *pqueue, PQNode *node);