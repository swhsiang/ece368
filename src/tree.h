#include "pq.h"

void printGivenLevel(PQNode* root, int level); 
int height(PQNode* node); 
void printLevelOrder(PQNode *root); 
int leaf_counter(PQNode *root);
void print2D(PQNode* root);
void print2DUtil(PQNode* root, int space);