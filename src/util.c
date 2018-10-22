#include "util.h"

/*
 * Count how many characters appear in the given file include EOF.
 */
char *Load_File(char *Filename, unsigned int *Size, unsigned *DistinctChars,
                int *Frequency) {
  // FIXME should accept larger input!
  int ascii = 0;
  unsigned int counter = 0;
  long int capacity = INIT_STRING;
  unsigned distinct = 0;
  char *ch = malloc(sizeof(char) * INIT_STRING);
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
char *LoadFile(char *Filename, unsigned int *Size, unsigned *NumDistinctChar,
               unsigned int **DistinctChars, int **Frequency) {
  // FIXME should accept larger input!
  int ascii = 0;
  unsigned int counter = 0;
  long int capacity = INIT_STRING;
  unsigned distinct = 0;
  char *ch = malloc(sizeof(char) * INIT_STRING);

  int *freq = (int *)malloc(sizeof(int) * ASCII_NUM);
  int i = 0;
  for (i = 0; i < ASCII_NUM; i++) {
    freq[i] = 0;
  }

  FILE *fptr;

  fptr = fopen(Filename, "r");

  if (fptr == NULL) {
    printf("Cannot open the file %s\n", Filename);
    exit(1);
  }

  ascii = getc(fptr);
  while (ascii != EOF) {
    if (counter >= capacity) {
      capacity = (long int)(((float)capacity) * (1.0 + CHAR_GROW_FACTOR));
      char *temp = (char *)realloc(ch, capacity * sizeof(char));
      assert(temp != NULL);
      ch = temp;
    }
    ch[counter] = ascii;
    if (freq[ascii] == 0) distinct++;
    freq[ascii] = freq[ascii] + 1;
    counter++;
    ascii = getc(fptr);
  }

  // NOTE Take pseudo EOF into account
  int *temp_freq = (int *)malloc(sizeof(int) * (distinct + 1));
  unsigned int *temp_ch =
      (unsigned int *)malloc(sizeof(unsigned int) * (distinct + 1));
  int j = 0;
  for (i = 0, j = 0; i < ASCII_NUM; i++) {
    if (freq[i] > 0) {
      temp_ch[j] = i;
      temp_freq[j] = freq[i];
      j++;
    }
  }

  // NOTE add pseudo EOF
  temp_ch[j] = 256;
  temp_freq[j] = 1;

  ch[counter] = '\0';
  *Size = counter;
  *NumDistinctChar = distinct + 1;
  *DistinctChars = temp_ch;
  *Frequency = temp_freq;

  fclose(fptr);
  return ch;
}
// NOTE root can be NULL, make sure we handle it.
// Size is exact size + 1 (pseudo EOF);
/*
void build_huffman_tree(PQNode *root, unsigned int DistinctChars,
                        int *Frequency) {
  PQueue *pq = (PQueue *)malloc(sizeof(PQueue));
  NewPriorityQueue(pq, DistinctChars);
  int i;
  int char_counter;

  for (i = MAX_CHAR - 1, char_counter = 0;
       i >= 0 && char_counter < DistinctChars; i--) {
    if (Frequency[i] > 0) {
      PQNode *temp = malloc(sizeof(PQNode));
      NewNode(temp, Frequency[i], i, NULL, NULL);
      InsertKey(pq, temp);
      char_counter++;
    }
  }

  if ((pq->heap_size) == 1) {
    // FIXME think about adding pseudo EOF.
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
*/

PQNode *build_huffman_trie(unsigned int NumDistinctChar,
                           unsigned int *DistincChars, int *Frequency) {
  PQueue *pq = CreateAndBuildPriorityQueue(NumDistinctChar, DistincChars, Frequency);

  PQNode *left, *right, *temp;

  while (pq->heap_size != 1) {
    left = popMin(pq);
    right = popMin(pq);
    temp = newNode('\0', left->priority + right->priority);
    temp->left = left;
    temp->right = right;
    InsertNodeOnHeap(pq, temp);
  }

  return popMin(pq);
};

// FIXME we can use fast heap construction to optimize this function
// the complexity will be O(n) instead of O(n * log n);
void _build_codebook(char **arr_string, PQNode *node, char *binary) {
  if (node == NULL) {
    return;
  }
  if (node->right == NULL && node->left == NULL) {
    free(arr_string[node->value]);
    arr_string[node->value] = binary;
    assert(arr_string[node->value] != NULL);
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

  assert(node->left->value != 4294967259);
  _build_codebook(arr_string, node->left, left_str);
  assert(node->right->value != 4294967259);
  _build_codebook(arr_string, node->right, right_str);
};

void build_codebook(char **arr_string, unsigned int *DistinctChars,
                    unsigned int NumDistinctChar, PQNode *node) {
  char **temp_codebook = (char **)malloc(sizeof(char *) * ASCII_NUM);
  int i = 0;
  for (i = 0; i < ASCII_NUM; i++) {
    temp_codebook[i] = (char *)malloc(sizeof(char));
  }
  _build_codebook(temp_codebook, node, "");

  for (i = 0; i < NumDistinctChar; i++) {
    assert(temp_codebook[DistinctChars[i]] != NULL);
    arr_string[i] = temp_codebook[DistinctChars[i]];
  }
  free(temp_codebook);
}

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

void GenerateBinaryHeader(char *dest, unsigned int *BinarySize,
                          char **codebook){
    // FIXME store the info of codebook in the begining of binary

};

// https://stackoverflow.com/questions
// /759707/efficient-way-of-storing-huffman-tree
// tree_path has been allocated. ex 9 bit + 1 terminator + 1 char itself
void encodeNode(PQNode *node, char *tree_path) {
  if (!(node->left) && !(node->right)) {
    strcat(tree_path, "1");
    // ??????
    // strcat(tree_path, node->value);
  } else {
  }
}

// Generate binary of original file
char *GenerateBinaryBody(char *source, unsigned int sourceSize,
                         unsigned int *BinarySize, char **codebook,
                         unsigned *paddingLen) {
  int i = 0, j;
  int lenOfCode = 0;
  char buffer = 0;
  unsigned buffer_len = 0;
  unsigned int bytesOfBinary = 0;
  unsigned int capacity = INIT_STRING;
  char *dest = malloc(sizeof(char) * capacity);

  for (; i < sourceSize; i++) {
    lenOfCode = strlen(codebook[source[i]]);
    for (j = 0; j < lenOfCode; j++) {
      assert((codebook[source[i]][j] == '0' || codebook[source[i]][j] == '1'));
      buffer = codebook[source[i]][j] | (buffer << 1);

      buffer_len++;
      if (buffer_len >= 8) {
        // examine the boundary of dest
        if (bytesOfBinary >= capacity - 1) {
          capacity = (unsigned int)(((float)(capacity - 1)) *
                                        (1.0 + CHAR_GROW_FACTOR) +
                                    1.0);
          char *temp = realloc(dest, sizeof(char) * capacity);
          assert(temp != NULL);
          dest = temp;
          dest[capacity - 1] = '\0';
        }

        dest[bytesOfBinary] = buffer;
        bytesOfBinary++;
        buffer = 0;
      }
    }
  }

  // FIXME
  // do padding if the buffer is not full
  if (buffer_len > 0) {
    if (bytesOfBinary >= capacity - 1) {
      capacity =
          (unsigned int)(((float)(capacity - 1)) * (1.0 + CHAR_GROW_FACTOR) +
                         1.0);
      char *temp = realloc(dest, sizeof(char) * capacity);
      assert(temp != NULL);
      dest = temp;
      dest[capacity - 1] = '\0';
    }
    dest[bytesOfBinary] = (buffer << (8 - buffer_len));
    bytesOfBinary++;
  }
  dest[bytesOfBinary] = '\0';

  *BinarySize = bytesOfBinary;
  return dest;
};

void GenerateBinary(char *dest, char *source, unsigned int sourceSize,
                    unsigned int *BinarySize, char **codebook) {
  // pseudo EOF or add padding info at the front of the file
  unsigned int headerSize = 0;
  GenerateBinaryHeader(dest, &headerSize, codebook);

  // FIXME size ?? binary size?? if there already exist some content in the
  // dest?? don't overwrite it.
  char *body = malloc(sizeof(char) * INIT_STRING);
  unsigned int bodyBinarySize = 0;
  unsigned paddingLength = 0;
  body = GenerateBinaryBody(source, sourceSize, &bodyBinarySize, codebook,
                            &paddingLength);

  char *buffer = realloc(dest, headerSize + bodyBinarySize + 1);
  assert(buffer != NULL);
  dest = buffer;

  memcpy(dest + headerSize, body, bodyBinarySize + 1);
};

void WriteFile(char *Filename, char *bCh, unsigned int Size) {}