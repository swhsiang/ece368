#include "tree.h"
#define COUNT 5 

int height(PQNode* node) {
  if (node == NULL)
    return 0;
  else {
    /* compute the height of each subtree */
    int lheight = height(node->left);
    int rheight = height(node->right);

    /* use the larger one */
    if (lheight > rheight)
      return (lheight + 1);
    else
      return (rheight + 1);
  }
}

void printGivenLevel(PQNode* root, int level) {
  if (root == NULL) {
    printf("_l:%d_", level);
    return;
  }
  if (level == 1)
    printf("_l:%d.%d_", level, root->value);
  else if (level > 1) {
    printGivenLevel(root->left, level - 1);
    printGivenLevel(root->right, level - 1);
  }
}

void printLevelOrder(PQNode* root) {
  int h = height(root);
  int i;
  for (i = 1; i <= h; i++) printGivenLevel(root, i);
}

int leaf_counter(PQNode* root) {
  if (!root) return 0;
  if ((!root->left) && (!root->right)) {
    printf("%c ", root->value);
    return 1;
  };
  return leaf_counter(root->left) + leaf_counter(root->right);
}

void print2DUtil(PQNode* root, int space) {
  // Base case
  if (root == NULL) return;

  // Increase distance between levels
  space += COUNT;

  // Process right child first
  print2DUtil(root->right, space);

  // Print current node after space
  // count
  printf("\n");
  for (int i = COUNT; i < space; i++) printf(" ");

  printf("%3d\n", root->value);

  // Process left child
  print2DUtil(root->left, space);
}

// Wrapper over print2DUtil()
void print2D(PQNode* root) {
  // Pass initial space count as 0
  print2DUtil(root, 0);
}