#include "util.h"

/*
 * Count how many characters appear in the given file include EOF.
 */
char *Load_File(char *Filename, unsigned int *Size, unsigned *NumDistinctChar,
                unsigned char **DistinctChars, int **Frequency) {
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

  int *temp_freq = (int *)malloc(sizeof(int) * distinct);
  unsigned char *temp_ch =
      (unsigned char *)malloc(sizeof(unsigned char) * distinct);
  int j = 0;
  for (i = 0, j = 0; i < ASCII_NUM; i++) {
    if (freq[i] > 0) {
      temp_ch[j] = i;
      temp_freq[j] = freq[i];
      j++;
    }
  }

  ch[counter] = '\0';
  *Size = counter;
  *NumDistinctChar = distinct;
  *DistinctChars = temp_ch;
  *Frequency = temp_freq;

  fclose(fptr);
  return ch;
}

char *Decode(char *Filename, unsigned int *Size, unsigned *LeaveSize) {
  int byte = 0;
  unsigned int counter = 0;
  long int capacity = INIT_STRING;
  char *ch = malloc(sizeof(char) * INIT_STRING);

  FILE *fptr;

  fptr = fopen(Filename, "rb");
  if (fptr == NULL) {
    printf("Cannot open the file %s\n", Filename);
    exit(1);
  }

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

PQNode *build_huffman_trie(unsigned char NumDistinctChar,
                           unsigned char *DistincChars, int *Frequency) {
  PQueue *pq =
      CreateAndBuildPriorityQueue(NumDistinctChar, DistincChars, Frequency);

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
    arr_string[node->value] = binary;
    assert(arr_string[node->value] != NULL);
    return;
  }

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

/*
void char_build_codebook(char **arr_string, PQNode *node, char *binary) {
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
*/

void build_codebook(char **arr_string, unsigned char *UniqueCharList,
                    unsigned char NumUniqueChar, PQNode *node) {
  _build_codebook(arr_string, node, "");
  /*
    FIXME if the above approach doesn't work, try the old method.
    char **temp_codebook = (char **)malloc(sizeof(char *) * ASCII_NUM);
    int i = 0;
    for (i = 0; i < ASCII_NUM; i++) {
      temp_codebook[i] = (char *)malloc(sizeof(char));
    }
    _build_codebook(temp_codebook, node, "");

    for (i = 0; i < NumUniqueChar; i++) {
      assert(temp_codebook[UniqueCharList[i]] != NULL);
      arr_string[i] = temp_codebook[UniqueCharList[i]];
    }
    free(temp_codebook);
    */
}

/*
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
*/

int WriteBit(FILE *fptr, unsigned char *buffer, unsigned char *buffer_len,
             unsigned char bit) {
  int status = 0;
  if (*buffer_len == 0) {
    *buffer = 0;
  }
  if (*buffer_len >= 8) {
    // buffer is full, flush buffer
    status = fwrite(buffer, sizeof(unsigned char), 1, fptr);
    *buffer = 0;
    *buffer_len = 0;
    if (status == 1)
      status = 1;
    else
      status = -1;
  }

  *buffer = (*buffer << 1) | bit;
  *buffer_len += 1;

  if (*buffer_len >= 8) {
    // buffer is full, flush buffer
    status = fwrite(buffer, sizeof(unsigned char), 1, fptr);
    *buffer = 0;
    *buffer_len = 0;
    if (status == 1)
      status = 1;
    else
      status = -1;
  }
  return status;
};

int WriteByte(FILE *fptr, unsigned char *buffer, unsigned char *buffer_len,
              unsigned char value) {
  unsigned char mask = 0x80;  // 8 bits
  while (mask > 0) {
    WriteBit(fptr, buffer, buffer_len, (value & mask) == mask);
    mask >>= 1;
  }
  // FIXME return value should be meaningful
  return 1;
};

// https://stackoverflow.com/questions
// /759707/efficient-way-of-storing-huffman-tree
// tree_path has been allocated. ex 9 bit + 1 terminator + 1 char itself
void encodeNode(FILE *fptr, PQNode *node, unsigned char *buffer,
                unsigned char *buffer_len) {
  if (node == NULL) return;
  if (!(node->left) && !(node->right)) {
    char bit = '1';
    WriteBit(fptr, buffer, buffer_len, bit);
    WriteByte(fptr, buffer, buffer_len, node->value & 0xFF);
  }
  char bit = '0';
  WriteBit(fptr, buffer, buffer_len, bit);
  encodeNode(fptr, node->left, buffer, buffer_len);
  encodeNode(fptr, node->right, buffer, buffer_len);
}

// return how many zeros are used to complete padding
unsigned padding(FILE *fptr, unsigned char *buffer, unsigned char *buffer_len) {
  unsigned counter = 0, i = 0;
  assert(*buffer_len != 8);
  if (*buffer_len == 0) {
    *buffer = 0;
    return counter;
  }

  i = 8 - *buffer_len;
  while (i > 0) {
    WriteBit(fptr, buffer, buffer_len, '0');
    counter++;
    i--;
  }
  return counter;
}

/*
 * Assumption:
 * 3 bytes of file
 * --------------------------------------------------------------------------------
 * | number of leaves, 8bits | file end pading info 8bits | tree xbtis | empty 1
 * byte | content / binary
 * --------------------------------------------------------------------------------
 * 1. first 8 bits: how many leaf nodes in the tree (in hexidecimal) (should
 * exclude EOF).
 * 2. the begin of tree.
 * 3. content
 */
void GenerateBinaryHeader(FILE *fptr, unsigned int *BinarySize, char **codebook,
                          unsigned int leaf_counter, PQNode *root,
                          unsigned *paddingNum) {
  assert(leaf_counter < 257 && leaf_counter >= 2);

  // Generate header info
  unsigned char firstByte = (unsigned char)(leaf_counter & 0xFF);
  // FIXME remember to update this field after compression done.
  unsigned char secondByte;
  secondByte = 0;
  int i = 0;

  fwrite(&firstByte, sizeof(unsigned char), 1, fptr);
  fwrite(&secondByte, sizeof(unsigned char), 1, fptr);

  unsigned char buffer = 0;
  unsigned char buffer_len = 0;
  encodeNode(fptr, root, &buffer, &buffer_len);
  *paddingNum = padding(fptr, &buffer, &buffer_len);
};

// Generate binary of original file
void GenerateBinaryBody(FILE *fptr, char *source, unsigned int sourceSize,
                        unsigned int *bodyBinarySize, char **codebook,
                        unsigned *bodyPaddingLength) {
  unsigned int i = 0, counter = 0;
  unsigned j = 0;
  unsigned char buffer = 0, buffer_len = 0;
  for (i = 0; i < sourceSize; i++) {
    for (j = 0; j < strlen(codebook[source[i]]); j++) {
      WriteBit(fptr, &buffer, &buffer_len, codebook[source[i]][j]);
      counter++;
    }
  }
  *bodyBinarySize = counter / 8;
  *bodyPaddingLength = padding(fptr, &buffer, &buffer_len);
};

void GenerateBinary(FILE *fptr, char *source, unsigned int sourceSize,
                    char **codebook, unsigned int codebookSize, PQNode *root) {
  unsigned int headerSize = 0;
  unsigned int leaf_counter = codebookSize;
  unsigned headerPaddingLength = 0;
  GenerateBinaryHeader(fptr, &headerSize, codebook, leaf_counter, root,
                       &headerPaddingLength);
  assert(headerPaddingLength < 8);

  unsigned int bodyBinarySize = 0;
  unsigned bodyPaddingLength = 0;
  GenerateBinaryBody(fptr, source, sourceSize, &bodyBinarySize, codebook,
                     &bodyPaddingLength);
  assert(bodyPaddingLength < 8);

  unsigned char totalPadding = (headerPaddingLength << 4) | (bodyPaddingLength);
  fseek(fptr, 1, SEEK_SET);
  unsigned char buffer = 0, buffer_len = 0;
  WriteByte(fptr, &buffer, &buffer_len, totalPadding);
};

void WriteFile(char *Filename, char *source, unsigned int sourceSize,
               char **codebook, unsigned char codebookSize, PQNode *root) {
  FILE *fptr;
  fptr = fopen(Filename, "wb");

  if (fptr == NULL) {
    printf("Cannot open the file %s\n", Filename);
    exit(1);
  }

  GenerateBinary(fptr, source, sourceSize, codebook, codebookSize, root);

  fclose(fptr);
}