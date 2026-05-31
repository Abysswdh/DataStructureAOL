#ifndef BST_H
#define BST_H



// -----------------------------------------------------------
// 1. PRODUCT struct
//    (koordinasi dengan Aby – harus sama dengan product.h)
// -----------------------------------------------------------
typedef struct Product {
    int    id;
    char   name[100];
    char   category[50];   // e.g. "Electronics", "Food", dll
    float  price;
    int    stock;
    float  discount;       // dalam persen, e.g. 10.0 = 10%
} Product;

// -----------------------------------------------------------
// 2. NODE struct  (AVL node, key = price)
// -----------------------------------------------------------
typedef struct Node {
    Product       data;
    struct Node  *left;
    struct Node  *right;
    int           height;  // untuk AVL balancing
} Node;

// -----------------------------------------------------------
// 3. AVL HELPER
// -----------------------------------------------------------
int   getHeight   (Node *n);
int   getBalance  (Node *n);
Node *rotateRight (Node *y);
Node *rotateLeft  (Node *x);
Node *rebalance   (Node *n);

// -----------------------------------------------------------
// 4. INSERT & DELETE
// -----------------------------------------------------------
Node *insert     (Node *root, Product p);
Node *deleteNode (Node *root, float price);   // hapus by harga
Node *findMin    (Node *n);                   // helper delete

// -----------------------------------------------------------
// 5. SEARCH
// -----------------------------------------------------------
Node *searchExact (Node *root, float price);
void  searchRange (Node *root, float minPrice, float maxPrice);

// -----------------------------------------------------------
// 6. TRAVERSAL
// -----------------------------------------------------------
void inorder        (Node *root);  // Low  -> High
void reverseInorder (Node *root);  // High -> Low

// -----------------------------------------------------------
// 7. MIN & MAX
// -----------------------------------------------------------
Node *findCheapest      (Node *root);  // node paling kiri
Node *findMostExpensive (Node *root);  // node paling kanan

// -----------------------------------------------------------
// 8. UTILITY
// -----------------------------------------------------------
void  freeTree     (Node *root);   // bebaskan semua memory
int   countNodes   (Node *root);   // hitung total produk

#endif // BST_H