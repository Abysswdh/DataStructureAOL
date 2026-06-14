/*
    Person 1: Manuel Fides Waluyo (2902572096)
    BST / AVL Core Logic — Implementation

    E-Commerce Product Management System
    Using: AVL Tree (self-balancing BST)

    System uses AVL Tree for efficient product filtering
    with automatic balancing for optimized searching.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"
#include "menu.h"



// INSERT ---------------------------------------------------------------------------

// create a new AVL node with given product data
static Node *createNode(Product p) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode) {
        printf("[ERROR] Memory allocation failed!\n");
        return NULL;
    }
    newNode->data   = p;
    newNode->left   = NULL;
    newNode->right  = NULL;
    return newNode;
}

// insert a product into the AVL tree
// primary key = price, secondary key = id (for tie-breaking)
// duplicate prices go to the right subtree
Node *insert(Node *root, Product p) {
    if (root == NULL)
        return createNode(p);

    if (p.price < root->data.price)
        root->left = insert(root->left, p);
    else if (p.price > root->data.price)
        root->right = insert(root->right, p);
    else {
        // same price — use ID as tie-breaker
        if (p.id < root->data.id)
            root->left = insert(root->left, p);
        else
            root->right = insert(root->right, p);
    }

    return root;
}

// DELETE ---------------------------------------------------------------------------

// find the node with the smallest price in a subtree (leftmost node)
// used as in-order successor during deletion
Node *findMin(Node *n) {
    if (n == NULL)       return NULL;
    if (n->left == NULL) return n;
    return findMin(n->left);
}

// delete a product by its unique ID
// traverses the entire tree because ID is not the BST key
Node *deleteNodeById(Node *root, int id) {
    if (root == NULL) return NULL;

    if (root->data.id == id) {
        // node found — handle 3 deletion cases:

        // Case 1: leaf node (no children)
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        // Case 2a: only has right child
        else if (root->left == NULL) {
            Node *temp = root->right;
            free(root);
            return temp;
        }
        // Case 2b: only has left child
        else if (root->right == NULL) {
            Node *temp = root->left;
            free(root);
            return temp;
        }
        // Case 3: has two children
        // replace with in-order successor (smallest in right subtree)
        else {
            Node *successor = findMin(root->right);
            root->data  = successor->data;
            root->right = deleteNodeById(root->right, successor->data.id);
        }
    } else {
        // ID doesn't match — search both subtrees
        // (because ID is not the BST ordering key)
        root->left  = deleteNodeById(root->left,  id);
        root->right = deleteNodeById(root->right, id);
    }

    return root;
}

// SEARCH ---------------------------------------------------------------------------

// search for a product by exact price match
// returns the first node found with matching price, or NULL
Node *searchExact(Node *root, float price) {
    if (root == NULL) return NULL;

    if (price < root->data.price)
        return searchExact(root->left, price);
    else if (price > root->data.price)
        return searchExact(root->right, price);
    else
        return root;   // exact match found
}

// search for a product by its unique ID
// must traverse entire tree since ID is not the BST key
Node *searchById(Node *root, int id) {
    if (root == NULL) return NULL;

    // check current node
    if (root->data.id == id) return root;

    // search left subtree first
    Node *found = searchById(root->left, id);
    if (found != NULL) return found;

    // if not found in left, search right subtree
    return searchById(root->right, id);
}

// display all products within price range [minPrice, maxPrice]
// uses in-order traversal to show results sorted by price (ascending)
void searchRange(Node *root, float minPrice, float maxPrice) {
    if (root == NULL) return;

    // only go left if there could be nodes >= minPrice
    if (root->data.price > minPrice)
        searchRange(root->left, minPrice, maxPrice);

    // print if current node is within range
    if (root->data.price >= minPrice && root->data.price <= maxPrice) {
        printProductRow(root->data);
    }

    // only go right if there could be nodes <= maxPrice
    if (root->data.price < maxPrice)
        searchRange(root->right, minPrice, maxPrice);
}

// TRAVERSAL ---------------------------------------------------------------------------

// inorder traversal: Left -> Root -> Right
// displays products sorted by price from LOW to HIGH (ascending)
void inorder(Node *root) {
    if (root == NULL) return;
    inorder(root->left);
    printProductRow(root->data);
    inorder(root->right);
}

// reverse inorder traversal: Right -> Root -> Left
// displays products sorted by price from HIGH to LOW (descending)
void reverseInorder(Node *root) {
    if (root == NULL) return;
    reverseInorder(root->right);
    printProductRow(root->data);
    reverseInorder(root->left);
}

// MIN & MAX ---------------------------------------------------------------------------

// find the cheapest product = leftmost node in BST
Node *findCheapest(Node *root) {
    if (root == NULL)        return NULL;
    if (root->left == NULL)  return root;
    return findCheapest(root->left);
}

// get the maximum product ID in the tree
int getMaxId(Node *root) {
    if (root == NULL) return 0;
    int leftMax = getMaxId(root->left);
    int rightMax = getMaxId(root->right);
    int max = root->data.id;
    if (leftMax > max) max = leftMax;
    if (rightMax > max) max = rightMax;
    return max;
}

// find the most expensive product = rightmost node in BST
Node *findMostExpensive(Node *root) {
    if (root == NULL)        return NULL;
    if (root->right == NULL) return root;
    return findMostExpensive(root->right);
}

// UTILITY ---------------------------------------------------------------------------

// free all nodes from memory using post-order traversal
// (free children first, then parent)
void freeTree(Node *root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// count total number of products/nodes in the tree
int countNodes(Node *root) {
    if (root == NULL) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

// helper: return the bigger of two integers
static int max(int a, int b) {
    return (a > b) ? a : b;
}

// return the height of the tree
// useful for Person 2 statistics
int getTreeHeight(Node *root) {
    if (root == NULL) return -1;
    return 1 + max(getTreeHeight(root->left), getTreeHeight(root->right));
}