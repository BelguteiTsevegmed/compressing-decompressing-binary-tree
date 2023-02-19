/**
 * Program to compress a binary tree into a directed acyclic
 * graph (DAG) and to decompress.
 *
 * Program:
 *  - When called with the "c" argument, reads the tree representation
 * from input and writes the compressed graph representation to output.
 *  - When called with the "d" argument, reads the compressed graph
 * representation from input and writes the tree representation that
 * the graph was compressed from to output.
 *
 * author: Belgutei Tsevegmed
 * date: January 24, 2023
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Data Structures */
typedef struct tree {
  int val;
  struct tree *left;
  struct tree *right;
} tree;

/**
 * Using an array to store the elements of the tree.
 * This will be an array of pointers to tree elements.
 */
tree **array = NULL;
int count = 0;

/**
 * Function to read a tree
 */
tree *read_tree() {
  int val;
  scanf("%d", &val);
  if (val == 0)
    return NULL;
  tree *root = (tree *)malloc(sizeof(tree));
  root->val = val;
  root->left = read_tree();
  root->right = read_tree();
  return root;
}

/**
 * Function to read a directed, acyclic graph
 */
tree *read_dag() {
  int val;
  scanf("%d", &val);
  if (val == 0)
    return NULL;
  tree *root = (tree *)malloc(sizeof(tree));
  root->val = val;
  if (val < 0) {
    root->left = NULL;
    root->right = NULL;
  } else {
    root->left = read_dag();
    root->right = read_dag();
  }
  return root;
}

/**
 * Function to print a tree
 */
void print_tree(tree *root) {
  if (root == NULL) {
    printf("0\n");
    return;
  }
  printf("%d\n", root->val);
  print_tree(root->left);
  print_tree(root->right);
}

/**
 * Function to print a directed, acyclic graph
 */
void print_dag(tree *root) {
  if (root == NULL) {
    printf("0\n");
    return;
  }
  printf("%d\n", root->val);
  if (root->val > 0) {
    print_dag(root->left);
    print_dag(root->right);
  }
}

/**
 * Helper function for the compression function.
 * Determines if two trees are isomorphic.
 */
bool iso(tree *d1, tree *d2) {
  if (d1 == d2) {
    return true;
  } else if (d1 == NULL || d2 == NULL) {
    return false;
  } else {
    return (d1->val == d2->val && iso(d1->left, d2->left) &&
            iso(d1->right, d2->right));
  }
}

/**
 * Function to compress a tree
 */
tree *compression(tree *root) {
  if (root == NULL)
    return NULL;
  tree *dag_root = NULL;
  int index = -1;
  /* Checks if the current tree isn't already in the tree array */
  for (int i = 0; i < count; i++) {
    if (iso(root, array[i])) {
      index = i;
      break;
    }
  }
  /* If the current tree isn't already in the tree array,
   * it makes a new tree */
  if (index == -1) {
    dag_root = (tree *)malloc(sizeof(tree));
    dag_root->val = root->val;
    /* Increases the size of the array and adds a new tree */
    array =
        (tree **)realloc(array, sizeof(tree *) * (size_t)(count + 1));
    array[count++] = root;
    dag_root->left = compression(root->left);
    dag_root->right = compression(root->right);
  } else {
    /* If tree is already in the array, it adds a new tree with value
     * equal to -(index+1), index is the index in the array */
    dag_root = (tree *)malloc(sizeof(tree));
    dag_root->val = -(index + 1);
    dag_root->left = NULL;
    dag_root->right = NULL;
  }
  return dag_root;
}

/**
 * Helper function for the decompression function.
 *
 * If a negative value is encountered in the graph, this function fills the 
 * corresponding node by copying the appropriate elements from the original tree.
 */
tree *fill_a_place(tree *orig_root) {
  if (orig_root == NULL) {
    return NULL;
  }
  tree *new = malloc(sizeof(tree));
  new->val = orig_root->val;
  new->left = fill_a_place(orig_root->left);
  new->right = fill_a_place(orig_root->right);
  return new;
}

/**
 * Function for decompressing the graph
 *
 * This function remembers the elements it has already encountered in the 
 * array, and when it encounters a negative value, it searches the array for 
 * the corresponding element and copies it to that location.
 */
tree *decompression(tree *root, tree *orig_root) {
  if (root == NULL)
    return NULL;
  if (root->val < 0) {
    return fill_a_place(array[-(root->val) - 1]);
  } else {
    tree *tree_root = (tree *)malloc(sizeof(tree));
    tree_root->val = root->val;
    /* Increases the size of the array and adds a new tree */
    array =
        (tree **)realloc(array, sizeof(tree *) * (size_t)(count + 1));
    array[count++] = tree_root;
    tree_root->left = decompression(root->left, orig_root);
    tree_root->right = decompression(root->right, orig_root);
    return tree_root;
  }
}

/**
 * Frees the memory occupied by a tree or graph
 */
void free_memory(tree *root) {
  if (root == NULL) {
    return;
  }
  free_memory(root->left);
  free_memory(root->right);
  free(root);
}

/**
 * Runs the program
 */
int main(int argc, char *argv[]) {
  if (argc < 2)
    return 0;
  char arg = argv[1][0];
  if (arg == 'c') {
    tree *tree_root = read_tree();
    tree *dag_root = compression(tree_root);
    print_dag(dag_root);
    free_memory(tree_root);
    free_memory(dag_root);
    free(array);
  } else if (arg == 'd') {
    tree *dag_root = read_dag();
    tree *tree_root = decompression(dag_root, dag_root);
    print_tree(tree_root);
    free_memory(dag_root);
    free_memory(tree_root);
    free(array);
  }
  return 0;
}
