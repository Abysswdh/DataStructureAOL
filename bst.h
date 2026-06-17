/*
    Person 1: Noel (Manuel Fides)
    BST Core Logic — Header

    E-Commerce Product Management System
    Using: Binary Search Tree (BST)

    Key = price (float)
    Tie-breaking = product ID (int)
*/

#ifndef BST_H
#define BST_H

// PRODUCT STRUCT ---------------------------------------------------------------------------

typedef struct Product {
    int    id;
    char   name[100];
    char   category[50];    // e.g. "Electronics", "Food", "Fashion"
    float  price;
    int    stock;
    float  discount;        // dalam persen, e.g. 10.0 = 10%
} Product;

// NODE STRUCT (BST) ---------------------------------------------------------------------------

typedef struct Node {
    Product       data;
    struct Node  *left;
    struct Node  *right;
} Node;


// INSERT & DELETE ---------------------------------------------------------------------------

Node *insert         (Node *root, Product p);
Node *deleteNodeById (Node *root, int id);             // hapus by ID (exact match)
Node *findMin        (Node *n);                        // helper: in-order successor

// SEARCH ---------------------------------------------------------------------------

Node *searchExact (Node *root, float price);           // cari 1 produk harga tepat
Node *searchById  (Node *root, int id);                // cari by ID (exact match)
void  searchRange (Node *root, float minPrice, float maxPrice);

// TRAVERSAL ---------------------------------------------------------------------------

void inorder        (Node *root);   // Low  -> High (ascending price)
void reverseInorder (Node *root);   // High -> Low  (descending price)

// MIN & MAX ---------------------------------------------------------------------------

Node *findCheapest      (Node *root);   // node paling kiri  = termurah
Node *findMostExpensive (Node *root);   // node paling kanan = termahal
int   getMaxId          (Node *root);   // ambil ID terbesar yang ada di tree

// UTILITY ---------------------------------------------------------------------------

void  freeTree      (Node *root);   // bebaskan semua memory (post-order)
int   countNodes    (Node *root);   // hitung total produk di tree
int   getTreeHeight (Node *root);   // tinggi tree (untuk debug/stats)

#endif // BST_H