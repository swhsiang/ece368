#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pq.h"

#define INIT_STRING 1000
#define CHAR_GROW_FACTOR 0.3
#define ASCII_NUM 256

/*
 * Loading file based on the parameters. Count how many characters in
 * the file and assign the number of characters to parameter Size.
 */
char *Load_File(char *Filename, char *ch, unsigned int *Size, int *Frequency);

void NewHNode(PQNode *node, char ch, unsigned int freq, PQNode *left,
              PQNode *right);


void build_huffman_tree(PQNode *root, unsigned int Size, int *Frequency);

void build_codebook(char **arr_string, PQNode *node, char *binary);

void print_huffman_tree(PQNode *node);