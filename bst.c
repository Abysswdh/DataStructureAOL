#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"

// insert

// Buat node baru
static Node *createNode(Product p) {
    Node *newNode   = (Node *)malloc(sizeof(Node));
    if (!newNode) {
        printf("[ERROR] Gagal alokasi memori!\n");
        return NULL;
    }
    newNode->data   = p;
    newNode->left   = NULL;
    newNode->right  = NULL;
    newNode->height = 0;
    return newNode;
}

// Insert produk ke BST (key = price)
// Jika harga sama, masuk ke kanan
Node *insert(Node *root, Product p) {
    if (root == NULL)
        return createNode(p);

    if (p.price < root->data.price)
        root->left  = insert(root->left,  p);
    else
        root->right = insert(root->right, p);

    return rebalance(root);
}

// delete

Node *findMin(Node *n) {
    if (n == NULL)       return NULL;
    if (n->left == NULL) return n;
    return findMin(n->left);
}

// Hapus node berdasarkan harga (price)
Node *deleteNode(Node *root, float price) {
    if (root == NULL) {
        printf("[INFO] Produk dengan harga %.2f tidak ditemukan.\n", price);
        return NULL;
    }

    if (price < root->data.price) {
        root->left  = deleteNode(root->left,  price);
    } else if (price > root->data.price) {
        root->right = deleteNode(root->right, price);
    } else {
        // Node ditemukan — 3 kasus:

        // Kasus 1: leaf node (tidak punya anak)
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        // Kasus 2a: hanya punya anak kanan
        else if (root->left == NULL) {
            Node *temp = root->right;
            free(root);
            return temp;
        }
        // Kasus 2b: hanya punya anak kiri
        else if (root->right == NULL) {
            Node *temp = root->left;
            free(root);
            return temp;
        }
        // Kasus 3: punya dua anak
        // Ganti dengan in-order successor (minimum di subtree kanan)
        else {
            Node *successor = findMin(root->right);
            root->data      = successor->data;
            root->right     = deleteNode(root->right, successor->data.price);
        }
    }

    return rebalance(root);
}


// search



// Cari produk dengan harga TEPAT
Node *searchExact(Node *root, float price) {
    if (root == NULL) return NULL;

    if (price == root->data.price) return root;

    if (price < root->data.price)
        return searchExact(root->left,  price);
    else
        return searchExact(root->right, price);
}

// Cari & tampilkan semua produk dalam rentang harga [minPrice, maxPrice]
void searchRange(Node *root, float minPrice, float maxPrice) {
    if (root == NULL) return;

    if (root->data.price > minPrice)
        searchRange(root->left, minPrice, maxPrice);

    if (root->data.price >= minPrice && root->data.price <= maxPrice) {
        printf("  [ID:%d] %-25s | Rp %10.2f | Stok: %d | Kategori: %s\n",
               root->data.id,
               root->data.name,
               root->data.price,
               root->data.stock,
               root->data.category);
    }

    if (root->data.price < maxPrice)
        searchRange(root->right, minPrice, maxPrice);
}

//traversal

// Inorder  ->  Low to High (kiri - root - kanan)
void inorder(Node *root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("  [ID:%d] %-25s | Rp %10.2f | Stok: %3d | Kat: %-15s | Diskon: %.0f%%\n",
           root->data.id,
           root->data.name,
           root->data.price,
           root->data.stock,
           root->data.category,
           root->data.discount);
    inorder(root->right);
}

// Reverse Inorder  ->  High to Low (kanan - root - kiri)
void reverseInorder(Node *root) {
    if (root == NULL) return;
    reverseInorder(root->right);
    printf("  [ID:%d] %-25s | Rp %10.2f | Stok: %3d | Kat: %-15s | Diskon: %.0f%%\n",
           root->data.id,
           root->data.name,
           root->data.price,
           root->data.stock,
           root->data.category,
           root->data.discount);
    reverseInorder(root->left);
}

// min dan max


// Produk TERMURAH = node paling kiri
Node *findCheapest(Node *root) {
    if (root == NULL)        return NULL;
    if (root->left == NULL)  return root;
    return findCheapest(root->left);
}

// Produk TERMAHAL = node paling kanan
Node *findMostExpensive(Node *root) {
    if (root == NULL)        return NULL;
    if (root->right == NULL) return root;
    return findMostExpensive(root->right);
}

// utility

// Bebaskan semua node dari memori (post-order)
void freeTree(Node *root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// Hitung total jumlah node / produk
int countNodes(Node *root) {
    if (root == NULL) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}