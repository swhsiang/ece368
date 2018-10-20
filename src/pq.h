#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/* This Priority Queue is implemented based on Binary Heap (Min Heap) */

#ifndef PQ_H_
#define PQ_H_
typedef struct PQNode {
  // In order to fit the requirement of pseudo EOF, we need more space
  // than ASCII code requires
  unsigned int value;
  // Lower values indicate higher priority
  int priority;

  struct PQNode *left, *right;
} PQNode;

typedef struct PriorityQueue {
  int capacity;
  int heap_size;
  PQNode **heap_arr;
} PQueue;

void NewPriorityQueue(PQueue *pq, int capacity);

// Function to Create A New Node
void NewNode(PQNode *node, int priority, unsigned int value, PQNode *left, PQNode *right);

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
#endif