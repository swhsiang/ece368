#include "util.h"

/*
 * Count how many characters appear in the given file include EOF.
 */
char *Load_File(char *Filename, char *ch, unsigned int *Size, int *Frequency) {
  // FIXME should accept larger input!
  int ascii = 0;
  int counter = 0;
  long int capacity = INIT_STRING;
  FILE *fptr;

  fptr = fopen(Filename, "r");

  if (fptr == NULL) {
    printf("Cannot open the file %s\n", Filename);
    exit(1);
  }

  ascii = getc(fptr);
  while (ascii != EOF) {
    if (counter >= capacity) {
      capacity = (long int)(((float)capacity) * CHAR_GROW_FACTOR);
      char *temp = (char *)realloc(ch, capacity * sizeof(char));
      if (temp != NULL) {
        ch = temp;
      }
    }
    ch[counter] = (char)ascii;
    Frequency[ascii] = Frequency[ascii] + 1;
    counter++;
    ascii = getc(fptr);
  }

  // NOTE remember remove this termination character.
  ch[counter] = '\0';
  // FIXME count the EOF character ?
  //(*Frequency)[(int)'\0'] = (*Frequency)[(int)'\0'] + 1;
  *Size = counter;

  fclose(fptr);
  return ch;
}

// NOTE root can be NULL, make sure we handle it.
void build_huffman_tree(PQNode *root, unsigned int Size, int *Frequency) {
  PQueue *pq = malloc(sizeof(PQueue));
  NewPriorityQueue(pq, Size);
  unsigned i;
  int size_counter = 0;
  for (i = 0; i < ASCII_NUM; i++) {
    if (Frequency[i] > 0) {
      PQNode *temp = malloc(sizeof(PQNode));
      NewNode(temp, Frequency[i], (char)i, NULL, NULL);
      InsertKey(pq, temp);
      size_counter = Frequency[i] + size_counter;
    }
    if (size_counter >= Size) {
      break;
    }
  }

  if ((pq->heap_size) == 1) {
    PQNode *temp = malloc(sizeof(PQNode));
    if (Frequency['\0'] == 0) {
      NewNode(temp, 0, '\0', NULL, NULL);
    } else {
      NewNode(temp, 0, '\0', NULL, NULL);
    }
    InsertKey(pq, temp);
  }

  while (pq->heap_size > 1) {
    PQNode *left = malloc(sizeof(PQNode)), *right = malloc(sizeof(PQNode)),
           *temp = malloc(sizeof(PQNode));
    ExtractMin(pq, left);
    ExtractMin(pq, right);
    NewNode(temp, left->priority + right->priority, '\0', left, right);
    InsertKey(pq, temp);
  }

  ExtractMin(pq, root);
};

void build_codebook(char **arr_string, PQNode *node, char *binary) {
  if (!IsLeaf(node)) {
    // FIXME memory allocation can be optimized
    int len = strlen(binary);
    char *left_str = malloc(len + 1 + 1);
    strcpy(left_str, binary);
    left_str[len] = '0';
    left_str[len + 1] = '\0';

    char *right_str = malloc(len + 1 + 1);
    strcpy(right_str, binary);
    right_str[len] = '1';
    right_str[len + 1] = '\0';
    build_codebook(arr_string, node->left, left_str);
    build_codebook(arr_string, node->right, right_str);
  } else {
    char *temp = realloc(arr_string[node->value], strlen(binary));
    assert(temp != NULL);
    arr_string[node->value] = binary;
    // FIXME remove me
    printf("node %c binary %s\n", node->value, binary);
  }
};

void print_huffman_tree(PQNode *node) {
  if (node == NULL) {
    return;
  }

  if ((node->left == NULL) && (node->right == NULL)) {
    printf("true ");
    printf("%c ", node->value);
    return;
  }
  printf("false ");
  if (node->left != NULL) print_huffman_tree(node->left);
  if (node->right != NULL) print_huffman_tree(node->right);
};