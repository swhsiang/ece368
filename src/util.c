#include "util.h"

/*
 * Count how many characters appear in the given file include EOF.
 */
char *Load_File(char *Filename, char *ch, unsigned int *Size,
                unsigned *DistinctChars, int *Frequency) {
  // FIXME should accept larger input!
  int ascii = 0;
  unsigned int counter = 0;
  long int capacity = INIT_STRING;
  unsigned distinct = 0;
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
      assert(temp != NULL);
      ch = temp;
    }
    ch[counter] = ascii;
    if (Frequency[ascii] == 0) distinct++;
    Frequency[ascii] = Frequency[ascii] + 1;
    counter++;
    ascii = getc(fptr);
  }

  // NOTE remember remove this termination character.
  ch[counter] = '\0';
  // FIXME count the EOF character ?
  //(*Frequency)[(int)'\0'] = (*Frequency)[(int)'\0'] + 1;
  *Size = counter;
  *DistinctChars = distinct;

  fclose(fptr);
  return ch;
}

// NOTE root can be NULL, make sure we handle it.
// Size is exact size + 1 (pseudo EOF);
void build_huffman_tree(PQNode *root, unsigned int DistinctChars,
                        int *Frequency) {
  PQueue *pq = malloc(sizeof(PQueue));
  NewPriorityQueue(pq, DistinctChars);
  int i, char_counter;



  for (i = MAX_CHAR - 1, char_counter = 0;
       i >= 0 && char_counter <= DistinctChars; i--) {
    if (Frequency[i] > 0) {
      PQNode *temp = malloc(sizeof(PQNode));
      NewNode(temp, Frequency[i], i, NULL, NULL);
      InsertKey(pq, temp);
      char_counter++;
    }
  }

  if ((pq->heap_size) == 1) {
    PQNode *temp = malloc(sizeof(PQNode));
    if (Frequency['\0'] == 0) {
      NewNode(temp, 0, '\0', NULL, NULL);
    } else {
      NewNode(temp, 0, '\1', NULL, NULL);
    }
    InsertKey(pq, temp);
  }

  while (pq->heap_size > 1) {
    // FIXME will the following declaration causes memory leak?
    // ex: left or right??
    PQNode *left = malloc(sizeof(PQNode)), *right = malloc(sizeof(PQNode)),
           *temp = malloc(sizeof(PQNode));
    ExtractMin(pq, left);
    ExtractMin(pq, right);
    NewNode(temp, left->priority + right->priority, '\0', left, right);
    InsertKey(pq, temp);
  }

  ExtractMin(pq, root);
};

// FIXME we can use fast heap construction to optimize this function
// the complexity will be O(n) instead of O(n * log n);
void build_codebook(char **arr_string, PQNode *node, char *binary) {
  if (node == NULL) {
    return;
  }
  if (node->right == NULL && node->left == NULL) {
    arr_string[node->value] = binary;
    return;
  }
  // FIXME memory allocation can be optimized
  int len = strlen(binary);
  char *left_str = malloc(len + 1 + 1);
  strcpy(left_str, binary);
  strcat(left_str, "0");

  char *right_str = malloc(len + 1 + 1);
  strcpy(right_str, binary);
  strcat(right_str, "1");

  if (!(node->left == NULL)) {
    assert(node->left->value != 4294967259);
    build_codebook(arr_string, node->left, left_str);
  }
  if (!(node->right == NULL)) {
    assert(node->right->value != 4294967259);
    build_codebook(arr_string, node->right, right_str);
  }
};

void print_huffman_tree(PQNode *node) {
  if (node == NULL) {
    return;
  }

  if ((node->left == NULL) && (node->right == NULL)) {
    printf("true ");
    printf("%d:%c ", node->value, node->value);
    return;
  }
  printf("false ");
  if (node->left != NULL) print_huffman_tree(node->left);
  if (node->right != NULL) print_huffman_tree(node->right);
};

void Generate_Binary(char *dest, char *source, unsigned int *Size,
                     char **codebook) {
  // FIXME store the info of codebook in the begining of binary
  int i = 0, j;
  int lenOfCode = 0;
  char buffer = 0;
  unsigned buffer_len = 0;
  unsigned int bytesOfBinary = 0;
  // capacity == number of allocated space include terminator \0
  unsigned int capacity = *Size;
  for (; i < *Size; i++) {
    lenOfCode = strlen(codebook[source[i]]);
    for (j = 0; j < lenOfCode; j++) {
      assert((codebook[source[i]][j] == '0' || codebook[source[i]][j] == '1'));
      if (codebook[source[i]][j] == '0') {
        buffer = buffer | (0 << buffer_len);
      } else if (codebook[source[i]][j] == '1') {
        buffer = buffer | (1 << buffer_len);
      }

      buffer_len++;
      if (buffer_len >= 8) {
        // increase the Size of dest by 1 (byte)
        // check the overflow of dest?

        if (bytesOfBinary > capacity - 1) {
          // realloc
          capacity = (unsigned int)(((float)(capacity - 1)) *
                                        (1.0 + CHAR_GROW_FACTOR) +
                                    1.0);
          char *temp = realloc(dest, sizeof(char) * capacity);
          assert(temp != NULL);
          dest = temp;
          // FIXME Should I use \0?
          dest[capacity - 1] = '\0';
        }
        dest[bytesOfBinary - 1] = buffer;
        bytesOfBinary++;
        buffer = 0;
      }
      // do padding if the buffer is not full
      if (buffer_len > 0) {
        dest[bytesOfBinary - 1] = (buffer << (8 - buffer_len));
      }
      dest[bytesOfBinary] = '\0';
    }
  }
};
