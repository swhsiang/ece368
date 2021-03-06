#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pq.h"

#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define log_error(M, ...)                                                   \
  fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, \
          clean_errno(), ##__VA_ARGS__)
#define assertf(A, M, ...)       \
  if (!(A)) {                    \
    log_error(M, ##__VA_ARGS__); \
    assert(A);                   \
  }

#define INIT_STRING 1000
#define CHAR_GROW_FACTOR 0.3
#define ASCII_NUM 256
#define PSUEDO_EOF 257
#define PSUEDO_EOF_NUM 1
#define MAX_CHAR (PSUEDO_EOF_NUM + ASCII_NUM)

/*
 * Loading file based on the parameters. Count how many characters in
 * the file and assign the number of characters to parameter Size.
 */
char *Load_File(char *Filename, unsigned int *Size, unsigned *NumDistinctChar,
                unsigned char **DistinctChars, int **Frequency);

char *LoadFile(char *Filename, unsigned int *Size, unsigned *NumDistinctChar,
               unsigned int **DistinctChars, int **Frequency);

char *Decode(char *Filename);

void NewHNode(PQNode *node, char ch, unsigned int freq, PQNode *left,
              PQNode *right);

void build_huffman_tree(PQNode *root, unsigned int Size, int *Frequency);

PQNode *build_huffman_trie(unsigned int NumDistinctChar,
                           unsigned char *DistincChars, int *Frequency);

void build_codebook(char **arr_string, unsigned char *UniqueCharList,
                    unsigned int NumUniqueChar, PQNode *node);

void add_pseudo_eof(char **arr_string, PQNode *node, char *binary);

void print_huffman_tree(PQNode *node);

/*
 * Generate binary of original file
 * Input: Size == number of allocated space include terminator '\0';
 * Should store the codebook in the begining of the binary.
 */
void GenerateBinary(FILE *fptr, char *source, unsigned int sourceSize,
                    char **codebook, unsigned int codebookSize, PQNode *root);

void WriteFile(char *Filename, char *source, unsigned int sourceSize,
               char **codebook, unsigned int codebookSize, PQNode *root);