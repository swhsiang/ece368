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
  unsigned int numberOfChars = 0;
  unsigned numDistinctChar = 0;
  unsigned int *distinctChars =
      (unsigned int *)malloc(sizeof(unsigned int));
  int *frequency = (int *)malloc(sizeof(int));
  char *ch = malloc(sizeof(char) * INIT_STRING);

  ch = LoadFile(Argv[1], &numberOfChars, &numDistinctChar, &distinctChars,
                &frequency);
 
  if (numberOfChars == 0) {
    assert(numberOfChars != 0);
  }

  // 4. Count the frequency of each character and push characters
  //    with value of frequency onto Priority Queue (building Trie).


  // FIXME remove me. The following block created bug
  // set pseudo EOF as the last character
  // add pseudo EOF at the end
  /*
  numDistinctChar++;
  int *temp_freq = (int *) malloc(numDistinctChar * sizeof(int));
  assert(temp_freq != NULL);
  memcpy(temp_freq, frequency, numDistinctChar - 1);
  temp_freq[numDistinctChar - 1] = 1;
  free(frequency);
  frequency = temp_freq;

  unsigned int *temp_chars =
      (unsigned int *) malloc(sizeof(unsigned int) * numDistinctChar);
  assert(temp_chars != NULL);
  memcpy(temp_chars, distinctChars, numDistinctChar - 1);
  temp_chars[numDistinctChar - 1] = '$';
  free(distinctChars);
  distinctChars = temp_chars;
  */

  PQNode *root = (PQNode *) malloc(sizeof(PQNode));
  // Build huffman tree
  root = build_huffman_trie(numDistinctChar, distinctChars, frequency);

  char **codebook = (char **)malloc(sizeof(char *) * numDistinctChar);
  int i = 0;
  for (i = 0; i < numDistinctChar; i++) {
    codebook[i] = (char *)malloc(sizeof(char *));
  }

  // build_codebook(codebook, root, "");
  build_codebook(codebook, distinctChars, numDistinctChar, root);

  // FIXME remove me
  int j = 0;
  for (j = 0; j < numDistinctChar; j++) {
    printf("ASCII char: (%4d, %3d) frequency: %3d, binary: %-10s\n", j,
           distinctChars[j], frequency[j], codebook[j]);
  }

  // 6. Output the trie to xxx.huff
  // NOTE think about terminator \0
  numberOfChars = numberOfChars + 1;
  char *bCh = malloc(sizeof(char) * numberOfChars);
  unsigned int sizeofBinary = 0;
  // GenerateBinary(bCh, ch, numberOfChars, &sizeofBinary, codebook);

  /*
   * newFilename = Filename + ".huff" + '\0'
   */
  char *newFilename = malloc(strlen(Argv[1]) + strlen(".huff") + 1);
  assert(newFilename != NULL);
  newFilename[0] = '\0';
  strcpy(newFilename, Argv[1]);
  strcat(newFilename, ".huff");

  // WriteFIle(newFilename, bCh, numberOfChars);

  // FIXME free allocated memory
  return 1;
}
