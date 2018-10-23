#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

#include "util.h"

#define MAXFILELEN (50)
int main(int Argc, char **Argv) {
  // 1. Parse given parameters
  if (Argc < 2) {
    fprintf(stderr, "Must pass an argument!\n");
    exit(1);
  }

  // 2. Read file
  unsigned int fileSize = 0;
  unsigned numUniqueChar = 0;
  unsigned char *numUniqueCharList =
      (unsigned char *)malloc(sizeof(unsigned char));
  int *frequency = (int *)malloc(sizeof(int));
  char *ch = malloc(sizeof(char) * INIT_STRING);

  // no need using pseudo eof
  ch = Load_File(Argv[1], &fileSize, &numUniqueChar, &numUniqueCharList,
                 &frequency);

  if (fileSize == 0) {
    assert(fileSize != 0);
  }

  // 4. Count the frequency of each character and push characters
  //    with value of frequency onto Priority Queue (building Trie).
  // Build huffman tree
  PQNode *root = (PQNode *)malloc(sizeof(PQNode));
  root = build_huffman_trie(numUniqueChar, numUniqueCharList, frequency);

  char **codebook = (char **)malloc(sizeof(char *) * numUniqueChar);
  int i = 0;
  for (i = 0; i < numUniqueChar; i++) {
    codebook[i] = (char *)malloc(sizeof(char));
  }

  build_codebook(codebook, numUniqueCharList, numUniqueChar, root);

  //for (i=0; i < numUniqueChar; i++) {
  //  printf("%c: %s\n", numUniqueCharList[i], codebook[numUniqueCharList[i]]);
  //}

  // 6. Output the trie to xxx.huff
  /*
   * newFilename = Filename + ".huff" + '\0'
   */
  char *newFilename = malloc(strlen(Argv[1]) + strlen(".huff") + 1);
  assert(newFilename != NULL);
  newFilename[0] = '\0';
  strcpy(newFilename, Argv[1]);
  strcat(newFilename, ".huff");

  WriteFile(newFilename, ch, fileSize, codebook, *numUniqueCharList, root);

  // FIXME free allocated memory
  free(newFilename);
  free(ch);
  free(frequency);
  for (i = 0; i < numUniqueChar; i++) {
    free(codebook[i]);
  }
  free(codebook);
  free(numUniqueCharList);
  free(root);

  return 1;
}
