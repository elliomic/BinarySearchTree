/*
  File: bst.c
  Implementation of the binary search tree data structure.
 
*/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "bst.h"
#include "structs.h"


struct Node {
    TYPE         val;
    struct Node *left;
    struct Node *right;
};

struct BSTree {
    struct Node *root;
    int          cnt;
};

/*----------------------------------------------------------------------------*/
/*
  function to initialize the binary search tree.
  param tree
  pre: tree is not null
  post:	tree size is 0
  root is null
*/

void initBSTree(struct BSTree *tree)
{
    tree->cnt  = 0;
    tree->root = 0;
}

/*
  function to create a binary search tree.
  param: none
  pre: none
  post: tree->count = 0
  tree->root = 0;
*/

struct BSTree*  newBSTree()
{
    struct BSTree *tree = (struct BSTree *)malloc(sizeof(struct BSTree));
    assert(tree != 0);

    initBSTree(tree);
    return tree;
}

/*----------------------------------------------------------------------------*/
/*
  function to free the nodes of a binary search tree
  param: node  the root node of the tree to be freed
  pre: none
  post: node and all descendants are deallocated
*/

void _freeBST(struct Node *node)
{
    if (node != 0) {
	_freeBST(node->left);
	_freeBST(node->right);
	free(node);
    }
}

/*
  function to clear the nodes of a binary search tree
  param: tree    a binary search tree
  pre: tree ! = null
  post: the nodes of the tree are deallocated
  tree->root = 0;
  tree->cnt = 0
*/
void clearBSTree(struct BSTree *tree)
{
    _freeBST(tree->root);
    tree->root = 0;
    tree->cnt  = 0;
}

/*
  function to deallocate a dynamically allocated binary search tree
  param: tree   the binary search tree
  pre: tree != null;
  post: all nodes and the tree structure itself are deallocated.
*/
void deleteBSTree(struct BSTree *tree)
{
    clearBSTree(tree);
    free(tree);
}

/*----------------------------------------------------------------------------*/
/*
  function to determine if  a binary search tree is empty.

  param: tree    the binary search tree
  pre:  tree is not null
*/
int isEmptyBSTree(struct BSTree *tree) { return (tree->cnt == 0); }

/*
  function to determine the size of a binary search tree

  param: tree    the binary search tree
  pre:  tree is not null
*/
int sizeBSTree(struct BSTree *tree) { return tree->cnt; }

/*----------------------------------------------------------------------------*/

struct Node * constructNode(TYPE val) {
	struct Node * node = malloc(sizeof(struct Node));
	node->val = val;
	node->right = node->left = NULL;
	return node;
}

/*
  recursive helper function to add a node to the binary search tree.
  HINT: You have to use the compare() function to compare values.
  param:  cur	the current root node
  val	the value to be added to the binary search tree
  pre:	val is not null
*/
struct Node *_addNode(struct Node *cur, TYPE val)
{
	assert(cur != NULL);
	if (compare(val, cur->val) > 0 && cur->right == NULL) {
		cur->right = constructNode(val);
		return cur->right;
	} else if (compare(val, cur->val) > 0){
		return _addNode(cur->right, val);
	}
	if (compare(val, cur->val) < 0 && cur->left == NULL) {
		cur->left = constructNode(val);
		return cur->left;
	} else if (compare(val, cur->val) < 0){
		return _addNode(cur->left, val);
	}
	return NULL;
}

/*
  function to add a value to the binary search tree
  param: tree   the binary search tree
  val		the value to be added to the tree

  pre:	tree is not null
  val is not null
  pose:  tree size increased by 1
  tree now contains the value, val
*/
void addBSTree(struct BSTree *tree, TYPE val)
{
	if (isEmptyBSTree(tree)) {
		tree->root = constructNode(val);
	} else {
		_addNode(tree->root, val);
	}
	tree->cnt++;
}


/*----------------------------------------------------------------------------*/

int containsNode(struct Node * cur, TYPE val) {
	if (cur != NULL) {
		if (compare(val, cur->val) > 0) {
			return containsNode(cur->right, val);
		}
		if (compare(val, cur->val) < 0) {
			return containsNode(cur->left, val);
		}
		return 1;
	} else {
		return 0;
	}
}

/*
  function to determine if the binary search tree contains a particular element
  HINT: You have to use the compare() function to compare values.
  param:	tree	the binary search tree
  val		the value to search for in the tree
  pre:	tree is not null
  val is not null
  post:	none
*/
int containsBSTree(struct BSTree *tree, TYPE val)
{
	assert(tree != NULL);
	return containsNode(tree->root, val);
}

/*
  helper function to find the left most child of a node
  return the value of the left most child of cur
  param: cur		the current node
  pre:	cur is not null
  post: none
*/

/*----------------------------------------------------------------------------*/
TYPE _leftMost(struct Node *cur)
{
	assert(cur != NULL);
	while (cur->left != NULL) {
		cur = cur->left;
	}
	return cur->val;
}


/*
  recursive helper function to remove the left most child of a node
  HINT: this function returns cur if its left child is NOT NULL. Otherwise,
  it returns the right child of cur and free cur.

  Note:  If you do this iteratively, the above hint does not apply.

  param: cur	the current node
  pre:	cur is not null
  post:	the left most node of cur is not in the tree
*/
/*----------------------------------------------------------------------------*/
struct Node *_removeLeftMost(struct Node *cur)
{
	assert(cur != NULL);
	if (cur->left != NULL) {
		cur->left = _removeLeftMost(cur->left);
		return cur;
	} else if (cur->right != NULL) {
		struct Node * ret = cur->right;
		free(cur);
		return ret;
	} else {
		free(cur);
		return NULL;
	}
}
/*
  recursive helper function to remove a node from the tree
  HINT: You have to use the compare() function to compare values.
  param:	cur	the current node
  val	the value to be removed from the tree
  pre:	val is in the tree
  cur is not null
  val is not null
*/
/*----------------------------------------------------------------------------*/
struct Node *_removeNode(struct Node *cur, TYPE val)
{
	assert(cur != NULL);
	assert(containsNode(cur, val));
	if (compare(val, cur->val) > 0) {
		cur->right = _removeNode(cur->right, val);
	} else if (compare(val, cur->val) < 0) {
		cur->left = _removeNode(cur->left, val);
	} else if (cur->right == NULL) {
		struct Node * mate;
		mate = cur->left;
		free(cur);
		return mate;
	} else {
		cur->val = _leftMost(cur->right);
		cur->right = _removeLeftMost(cur->right);
	}
	return cur;
}
/*
  function to remove a value from the binary search tree
  param: tree   the binary search tree
  val		the value to be removed from the tree
  pre:	tree is not null
  val is not null
  val is in the tree
  pose:	tree size is reduced by 1
*/
void removeBSTree(struct BSTree *tree, TYPE val)
{
    if (containsBSTree(tree, val)) {
	tree->root = _removeNode(tree->root, val);
	tree->cnt--;
    }
}

/*----------------------------------------------------------------------------*/

/* The following is used only for debugging, set to "#if 0" when used 
   in other applications */
#if 1
#include <stdio.h>

/*----------------------------------------------------------------------------*/
void printNode(struct Node *cur) {
    if (cur == 0)
	return;
    printf("(");
    printNode(cur->left);	 
    /*Call print_type which prints the value of the TYPE*/
    print_type(cur->val);
    printNode(cur->right);
    printf(")");
}

void printTree(struct BSTree *tree) {
    if (tree == 0)
	return;	 
    printNode(tree->root);	 
}
/*----------------------------------------------------------------------------*/

#endif
