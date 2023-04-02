# compressing-decompressing-binary-tree

**Introduction

Tree compression is a process of transforming a tree into a directed acyclic graph (DAG) by merging all occurrences of non-empty repeated subtrees.

Tree decompression is the inverse transformation of tree compression.

**Command

When the program is called with the "c" argument, it reads the tree representation from the input and writes to the output the representation of the resulting compressed graph.
When the program is called with the "d" argument, it reads the graph representation, which is the result of compressing the tree, and writes to the output the representation of the tree from which the graph was created.

**Tree representation

An empty tree is represented by a row containing only the number 0.

A non-empty tree is represented by a sequence of rows, where the first row represents the root node and contains its value. The subsequent rows represent the left subtree followed by the right subtree.

**Graph representation

The representation of the graph resulting from tree compression is obtained from the tree representation by replacing the second and each subsequent occurrence of a sequence of rows representing the same non-empty subtree with a single row containing the number -N. The value of N is the sequential number assigned to the vertex that is the root of that subtree, starting from 1 in the graph representation.
