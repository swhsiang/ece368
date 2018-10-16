#include <assert.h>
#include "pq.h"

void test_pq_insert() {
  PQueue *q = NewPriorityQueue(10);
  InsertKey(q, NewNode(10, 'a', NULL, NULL));
  InsertKey(q, NewNode(12, 'c', NULL, NULL));
  InsertKey(q, NewNode(3, 'z', NULL, NULL));
  assert(q->heap_size == 3);

  PQNode *temp = (PQNode *)malloc(sizeof(PQNode *));

  temp = GetMin(q);
  assert(temp != NULL);
  assert(temp->priority == 3);
  assert(temp->value == 'z');
  DeleteKey(q, 0);
  assert(q->heap_size == 2);

  ExtractMin(q, temp);
  assert(temp != NULL);
  assert(temp->priority == 10);
  assert(temp->value == 'a');
  assert(q->heap_size == 1);

  temp = GetMin(q);
  assert(temp->priority == 12);
  assert(temp->value == 'c');
}

int main(void) {
  test_pq_insert();
  return 0;
}