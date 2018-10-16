#include "pq.h"

void SWAP(PQNode *x, PQNode *y) {
  PQNode temp = *x;
  *x = *y;
  *y = temp;
}

// PQueue *q = (PQueue *)malloc(sizeof(PQueue));
void NewPriorityQueue(PQueue *pq, int capacity) {
  pq->capacity = capacity;
  pq->heap_arr = malloc(sizeof(PQNode *) * capacity);
  int i = 0;
  for (i = 0; i < capacity; i++) {
    PQNode *temp = malloc(sizeof(PQNode));
    temp->left = NULL;
    temp->right = NULL;
    pq->heap_arr[i] = temp;
  }
  pq->heap_size = 0;
}

void NewNode(PQNode *node, int priority, char value, PQNode *left,
             PQNode *right) {
  node->value = value;
  node->priority = priority;
  if (left != NULL) {
    node->left = left;
  } else {
    node->left = NULL;
  }
  if (right != NULL) {
    node->right = right;
  } else {
    node->right = NULL;
  }
}

bool IsLeaf(PQNode *node) {
  // assert(((node->left == NULL) && (node->right == NULL)) ||
  //       ((node->left != NULL) && (node->right != NULL)));

  // FIXME when jumpping into this function, even the node's left and right
  // fields contain null pointer, they become arbitary value. In other words,
  // the null pointers are polluted.
  return (node->left == NULL) && (node->right == NULL);
}

// Argument index should not be zero.
int Parent(int index) { return (index - 1) / 2; }

int LeftChild(int index) { return (2 * index + 1); };

int RightChild(int index) { return (2 * index + 2); };

void MinHeapify(PQueue *pqueue, int index) {
  int l = LeftChild(index);
  int r = LeftChild(index);
  int biggest = index;
  if (l < pqueue->heap_size &&
      pqueue->heap_arr[l]->priority < pqueue->heap_arr[index]->priority)
    biggest = l;
  if (r < pqueue->heap_size &&
      pqueue->heap_arr[r]->priority < pqueue->heap_arr[biggest]->priority)
    biggest = r;
  if (biggest != index) {
    SWAP(pqueue->heap_arr[index], pqueue->heap_arr[biggest]);
    MinHeapify(pqueue, biggest);
  }
};

void ExtractMin(PQueue *pqueue, PQNode *node) {
  if (pqueue->heap_size <= 0) {
    node = NULL;
    return;
  };
  if (pqueue->heap_size == 1) {
    pqueue->heap_size--;
    *node = *pqueue->heap_arr[0];
    return;
  }

  // Store the maximum value, and remove it from heap
  *node = *pqueue->heap_arr[0];
  pqueue->heap_arr[0] = pqueue->heap_arr[pqueue->heap_size - 1];
  pqueue->heap_size = pqueue->heap_size - 1;
  MinHeapify(pqueue, 0);
  return;
};

// Returns the maximum key (key at root) from Max heap
PQNode *GetMin(PQueue *pqueue) {
  if (pqueue->heap_size > 0) {
    return pqueue->heap_arr[0];
  }
  return NULL;
};

void DecreaseKey(PQueue *pqueue, int index, int newVal) {
  pqueue->heap_arr[index]->priority = newVal;
  while (index != 0 && pqueue->heap_arr[Parent(index)]->priority >
                           pqueue->heap_arr[index]->priority) {
    SWAP(pqueue->heap_arr[index], pqueue->heap_arr[Parent(index)]);
    index = Parent(index);
  }
}

// Deletes a key stored at index i
void DeleteKey(PQueue *pqueue, int index) {
  DecreaseKey(pqueue, index, INT_MIN);
  PQNode temp;
  ExtractMin(pqueue, &temp);
};

void InsertKey(PQueue *pqueue, PQNode *node) {
  if (pqueue->heap_size == pqueue->capacity) {
    printf("\nOverflow: Could not insertKey\n");
    return;
  }

  // First insert the new key at the end
  pqueue->heap_size = pqueue->heap_size + 1;
  unsigned int i = pqueue->heap_size - 1;
  pqueue->heap_arr[i] = node;

  // Fix the max heap property if it is violated
  while (i != 0 && pqueue->heap_arr[Parent(i)]->priority >
                       pqueue->heap_arr[i]->priority) {
    SWAP(pqueue->heap_arr[i], pqueue->heap_arr[Parent(i)]);
    i = Parent(i);
  }
};