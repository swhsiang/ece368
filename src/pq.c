#include "pq.h"

void SWAP(PQNode *x, PQNode *y) {
  PQNode temp = *x;
  *x = *y;
  *y = temp;
}

// PQueue *q = (PQueue *)malloc(sizeof(PQueue));
void NewPriorityQueue(PQueue *pq, int capacity) {
  pq->capacity = capacity;
  pq->heap_arr = (PQNode **)malloc(sizeof(PQNode *) * capacity);
  int i = 0;
  for (i = 0; i < capacity; i++) {
    PQNode *temp = (PQNode *)malloc(sizeof(PQNode));
    temp->left = NULL;
    temp->right = NULL;
    pq->heap_arr[i] = temp;
  }
  pq->heap_arr[0]->value = 0;
  pq->heap_size = 0;
}

PQNode *newNode(unsigned int value, int freq) {
  PQNode *temp = (PQNode *)malloc(sizeof(PQNode));
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

PQueue *CreateAndBuildPriorityQueue(int capacity, unsigned int *value,
                                    int *frequency) {
  PQueue *pq = (PQueue *)malloc(sizeof(PQueue));
  pq->capacity = capacity;
  pq->heap_arr = (PQNode **)malloc(sizeof(PQNode *) * capacity);
  int i = 0;
  for (i = 0; i < capacity; i++) {
    pq->heap_arr[i] = newNode(value[i], frequency[i]);
  }
  pq->heap_size = capacity;
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
      pq->heap_arr[left]->priority < pq->heap_arr[smallest]->priority)
    smallest = left;

  if (right < pq->heap_size &&
      pq->heap_arr[right]->priority < pq->heap_arr[smallest]->priority)
    smallest = right;

  if (smallest != index) {
    SwapPQNode(&pq->heap_arr[smallest], &pq->heap_arr[index]);
    MinHeapify(pq, smallest);
  }
};

void NewNode(PQNode *node, int priority, unsigned int value, PQNode *left,
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

void ExtractMin(PQueue *pqueue, PQNode *node) {
  assert(pqueue->heap_size > 0);

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
  // FIXME will the temp variable be deleted when the life cycle of this
  // function comes to its end?
  PQNode temp;
  ExtractMin(pqueue, &temp);
};

void InsertNodeOnHeap(PQueue *pq, PQNode *node) {
  ++pq->heap_size;
  int i = pq->heap_size - 1;

  while (i && node->priority < pq->heap_arr[Parent(i)]->priority) {
    pq->heap_arr[i] = pq->heap_arr[Parent(i)];
    i = Parent(i);
  }

  pq->heap_arr[i] = node;
}

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
  while (i > 0) {
    // The formula of sift_up affects how the tree is going to be built
    // (i - 1) / 2 will make the heap tree more in-balanced
    // i / 2 makes the tree more balanced
    if (pqueue->heap_arr[i / 2]->priority > pqueue->heap_arr[i]->priority) {
      SWAP(pqueue->heap_arr[i], pqueue->heap_arr[i / 2]);
    }
    i = i / 2;
  }
};