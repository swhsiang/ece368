#include "pq.h"

PQNode *newNode(unsigned int value, int freq) {
  PQNode *temp = (PQNode *)malloc(sizeof(PQNode));
  // FIXME refactor temp->value, unsigned int is too big since we don't need
  // pseudo eof anymore.
  temp->value = value;
  temp->priority = freq;
  temp->left = NULL;
  temp->right = NULL;
  return temp;
}

void BuildMinHeap(PQueue *pq) {
  int n = pq->heap_size - 1;
  int i = 0;
  for (i = Parent(n); i >= 0; --i) {
    MinHeapify(pq, i);
  }
}

PQueue *createPriorityQueue(int capacity) {
  PQueue *pq = (PQueue *)malloc(sizeof(PQueue));
  pq->heap_size = 0;
  pq->capacity = capacity;
  pq->heap_arr = (PQNode **)malloc(capacity * sizeof(PQNode));
  return pq;
}

PQueue *CreateAndBuildPriorityQueue(unsigned int size, unsigned char *value,
                                    int *frequency) {
  PQueue *pq = createPriorityQueue(size);
  int i = 0;
  for (i = 0; i < size; i++) {
    pq->heap_arr[i] = newNode(value[i], frequency[i]);
  }
  pq->heap_size = size;
  BuildMinHeap(pq);
  return pq;
}

void SwapPQNode(PQNode **a, PQNode **b) {
  PQNode *temp = *a;
  *a = *b;
  *b = temp;
}

void MinHeapify(PQueue *pq, int index) {
  int smallest = index;
  int left = LeftChild(index);
  int right = RightChild(index);

  if (left < pq->heap_size &&
      pq->heap_arr[left]->priority < pq->heap_arr[smallest]->priority) {
    smallest = left;
  }

  if (right < pq->heap_size &&
      pq->heap_arr[right]->priority < pq->heap_arr[smallest]->priority) {
    smallest = right;
  }

  if (smallest != index) {
    SwapPQNode(&pq->heap_arr[smallest], &pq->heap_arr[index]);
    MinHeapify(pq, smallest);
  }
};

bool IsLeaf(PQNode *node) {
  // assert(((node->left == NULL) && (node->right == NULL)) ||
  //       ((node->left != NULL) && (node->right != NULL)));

  // FIXME when jumpping into this function, even the node's left and right
  // fields contain null pointer, they become arbitary value. In other words,
  // the null pointers are polluted.
  return !(node->left) && !(node->right);
}

// Argument index should not be zero.
int Parent(int index) { return (index - 1) / 2; }

int LeftChild(int index) { return (2 * index + 1); };

int RightChild(int index) { return (2 * index + 2); };

PQNode *popMin(PQueue *pqueue) {
  assert(pqueue->heap_size > 0);
  PQNode *temp = pqueue->heap_arr[0];
  pqueue->heap_arr[0] = pqueue->heap_arr[pqueue->heap_size - 1];
  --pqueue->heap_size;
  MinHeapify(pqueue, 0);
  return temp;
}

void InsertNodeOnHeap(PQueue *pq, PQNode *node) {
  ++pq->heap_size;
  int i = pq->heap_size - 1;

  while (i && node->priority < pq->heap_arr[Parent(i)]->priority) {
    pq->heap_arr[i] = pq->heap_arr[Parent(i)];
    i = Parent(i);
  }

  pq->heap_arr[i] = node;
}
