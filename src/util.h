#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pq.h"

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
char *Load_File(char *Filename, char *ch, unsigned int *Size, unsigned *DistinctChars, int *Frequency);

void NewHNode(PQNode *node, char ch, unsigned int freq, PQNode *left,
              PQNode *right);


void build_huffman_tree(PQNode *root, unsigned int Size, int *Frequency);

void build_codebook(char **arr_string, PQNode *node, char *binary);

void add_pseudo_eof(char **arr_string, PQNode *node, char *binary);

void print_huffman_tree(PQNode *node);

/*
* Input: Size == number of allocated space include terminator '\0';
* Should store the codebook in the begining of the binary.
*/
void Generate_Binary(char *dest, char *source, unsigned int *Size, char **codebook);

void Write_File(char *Filename, char *ch, unsigned int Size);