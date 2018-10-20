#include <assert.h>
#include <string.h>
#include "tree.h"
#include "util.h"

void test_load_file(char *Filename) {
  unsigned int distinctChars = 0, Size =0;
  int *frequency = malloc(sizeof(int) * 256);

  char *ch = (char *)malloc(INIT_STRING * sizeof(char));
  Load_File(Filename, ch, &Size, &distinctChars, frequency);
  assert(strcmp(ch, "aaa\n") == 0);
  assert(Size == 4);
  assert(frequency[97] == 3);

  free(ch);
  free(frequency);
}

void test_build_huffman_tree(char *Filename) {
  unsigned int distinctChars = 0, Size = 0;
  int *frequency = NULL;
  frequency = malloc(sizeof(int) * 256);

  char *ch = (char *)malloc(INIT_STRING * sizeof(char));
  Load_File(Filename, ch, &Size, &distinctChars, frequency);
  assert(strcmp(ch, "aaa\n") == 0);
  assert(Size == 4);
  assert(frequency[97] == 3);
  assert(frequency[10] == 1);

  PQNode *root = malloc(sizeof(PQNode));
  NewNode(root, 0, '\0', NULL, NULL);
  build_huffman_tree(root, distinctChars, frequency);

  assert(root != NULL);
  assert(root->left->priority == 1);
  assert(root->left->value == '\n');
  assert(root->right->priority == 3);
  assert(root->right->value == 'a');
  free(root);
  free(ch);
  free(frequency);
}

void test_huffman_tree(char *Filename) {
  unsigned int distinctChars = 0, Size = 0;
  int *frequency = NULL;
  frequency = malloc(sizeof(int) * 256);

  char *ch = (char *)malloc(INIT_STRING * sizeof(char));
  Load_File(Filename, ch, &Size, &distinctChars, frequency);
  assert(strcmp(ch, "aaa\n") == 0);
  assert(Size == 4);
  assert(frequency[97] == 3);
  assert(frequency[10] == 1);

  PQNode *root = malloc(sizeof(PQNode));
  NewNode(root, 0, '\0', NULL, NULL);
  build_huffman_tree(root, distinctChars, frequency);

  assert(root != NULL);
  assert(root->left->priority == 1);
  assert(root->left->value == '\n');
  assert(root->right->priority == 3);
  assert(root->right->value == 'a');

  print_huffman_tree(root);
  free(root);
  free(ch);
  free(frequency);
}

void test_build_codebook(char *Filename) {
  unsigned int distinctChars = 0;
  unsigned int Size = 0;
  int *frequency = NULL;
  frequency = malloc(sizeof(int) * 256);

  char *ch = (char *)malloc(INIT_STRING * sizeof(char));
  Load_File(Filename, ch, &Size, &distinctChars, frequency);
  assert(strcmp(ch, "aaabbzz\n") == 0);
  assert(Size == 8);
  assert(frequency[97] == 3);
  assert(frequency[10] == 1);

  PQNode *root = malloc(sizeof(PQNode));
  NewNode(root, 0, '\0', NULL, NULL);
  build_huffman_tree(root, distinctChars, frequency);

  printLevelOrder(root);

  assert(root != NULL);

  char **codebook = (char **)malloc(sizeof(char *) * ASCII_NUM);
  int i = 0;
  for (i = 0; i < ASCII_NUM; i++) {
    codebook[i] = malloc(sizeof(char *));
  }
  build_codebook(codebook, root, "");
  for (i = 0; i < ASCII_NUM; i++) {
    printf("ASCII char: frequency: %d binary %20s\n", frequency[i],
           codebook[i]);
  }
  free(root);
  free(ch);
  free(frequency);
}

void test_leaf_node_huff() {
  unsigned int distinctChars = ASCII_NUM;
  int *frequency = NULL;
  frequency = malloc(sizeof(int) * MAX_CHAR);
  int i = 0;
  for (i = 0; i < MAX_CHAR; i++) {
    frequency[i] = 1;
  }

  char *ch = (char *)malloc(256 * sizeof(char));

  PQNode *root = malloc(sizeof(PQNode));
  NewNode(root, 0, '\0', NULL, NULL);

  frequency[PSUEDO_EOF - 1] = 1;
  build_huffman_tree(root, distinctChars + PSUEDO_EOF_NUM, frequency);
  printf("\nHOW many leaves of huffman tree %d\n", leaf_counter(root));

  // print2D(root);

  // assert(root != NULL);

  char **codebook = (char **)malloc(sizeof(char *) * (MAX_CHAR));
  int j = 0;
  for (j = 0; j < MAX_CHAR; j++) {
    codebook[j] = malloc(sizeof(char *));
  }
  build_codebook(codebook, root, "");
  for (j = MAX_CHAR - 1; j >= 0; j--) {
    printf("ASCII char: (%4d, ) frequency: %3d, binary: %-10s\n", j,
           frequency[j], codebook[j]);
  }

  free(root);
  free(ch);
  free(frequency);
  for (i = 0; i < MAX_CHAR; i++) {
    free(codebook[i]);
  }
  free(codebook);
}

int main(int Argc, char *Argv[]) {
  // test_load_file("/tmp/gg.txt");
  // test_build_huffman_tree("/tmp/gg.txt");
  /*
   * Handling issue of null pointer
   * https://stackoverflow.com/questions/42536419/why-do-we-need-pointer-to-a-structure-pointer-to-change-members-value
   */
  // test_huffman_tree("/tmp/gg.txt");
  // test_build_codebook("/tmp/gg.txt");
  test_leaf_node_huff();
  return 0;
}