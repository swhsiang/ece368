#include <assert.h>
#include "pq.h"

void test_pq_insert() {
  PQueue *q = NewPriorityQueue(10);
  PQNode *temp, *temp2, *temp3;
  NewNode(&temp, 10, 'a', NULL, NULL);
  InsertKey(q, temp);
  NewNode(&temp2, 12, 'c', NULL, NULL);
  InsertKey(q, temp2);
  NewNode(&temp3, 3, 'z', NULL, NULL);
  InsertKey(q, temp3);
  assert(q->heap_size == 3);

  PQNode *temp4;

  temp4 = GetMin(q);
  assert(temp4 != NULL);
  assert(temp4->priority == 3);
  assert(temp4->value == 'z');
  DeleteKey(q, 0);
  assert(q->heap_size == 2);

  ExtractMin(q, temp4);
  assert(temp4 != NULL);
  assert(temp4->priority == 10);
  assert(temp4->value == 'a');
  assert(q->heap_size == 1);

  temp4 = GetMin(q);
  assert(temp4->priority == 12);
  assert(temp4->value == 'c');
}

int main(void) {
  test_pq_insert();
  return 0;
}