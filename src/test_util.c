#include <assert.h>
#include <string.h>
#include "util.h"

void test_load_file(char *Filename) {
  unsigned int numberOfChars = 0;
  int *frequency = malloc(sizeof(int) * 256);

  char *ch = (char *)malloc(INIT_STRING * sizeof(char));
  Load_File(Filename, ch, &numberOfChars, frequency);
  assert(strcmp(ch, "aaa\n") == 0);
  assert(numberOfChars == 4);
  assert(frequency[97] == 3);

  free(ch);
  free(frequency);
}

void test_build_huffman_tree(char *Filename) {
  unsigned int numberOfChars = 0;
  int *frequency = NULL;
  frequency = malloc(sizeof(int) * 256);

  char *ch = (char *)malloc(INIT_STRING * sizeof(char));
  Load_File(Filename, ch, &numberOfChars, frequency);
  assert(strcmp(ch, "aaa\n") == 0);
  assert(numberOfChars == 4);
  assert(frequency[97] == 3);
  assert(frequency[10] == 1);

  PQNode *root = malloc(sizeof(PQNode));
  NewNode(root, 0, '\0', NULL, NULL);
  build_huffman_tree(root, numberOfChars, frequency);

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
  unsigned int numberOfChars = 0;
  int *frequency = NULL;
  frequency = malloc(sizeof(int) * 256);

  char *ch = (char *)malloc(INIT_STRING * sizeof(char));
  Load_File(Filename, ch, &numberOfChars, frequency);
  assert(strcmp(ch, "aaa\n") == 0);
  assert(numberOfChars == 4);
  assert(frequency[97] == 3);
  assert(frequency[10] == 1);

  PQNode *root = malloc(sizeof(PQNode));
  NewNode(root, 0, '\0', NULL, NULL);
  build_huffman_tree(root, numberOfChars, frequency);

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
  unsigned int numberOfChars = 0;
  int *frequency = NULL;
  frequency = malloc(sizeof(int) * 256);

  char *ch = (char *)malloc(INIT_STRING * sizeof(char));
  Load_File(Filename, ch, &numberOfChars, frequency);
  assert(strcmp(ch, "aaa\n") == 0);
  assert(numberOfChars == 4);
  assert(frequency[97] == 3);
  assert(frequency[10] == 1);

  PQNode *root = malloc(sizeof(PQNode));
  NewNode(root, 0, '\0', NULL, NULL);
  build_huffman_tree(root, numberOfChars, frequency);

  assert(root != NULL);
  assert(root->left->priority == 1);
  assert(root->left->value == '\n');
  assert(root->right->priority == 3);
  assert(root->right->value == 'a');

  free(root);
  free(ch);
  free(frequency);
}

int main(int Argc, char *Argv[]) {
  // test_load_file("/tmp/gg.txt");
  // test_build_huffman_tree("/tmp/gg.txt");
  test_huffman_tree("/tmp/gg.txt");
  //test_build_codebook("/tmp/gg.txt");
  return 0;
}