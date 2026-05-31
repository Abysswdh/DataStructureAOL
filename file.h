/*
    Person 2: Putra Abyasa Wedha | 2902583635
    File Handling & Product Management — Header

    E-Commerce Product Management System

    Using:
    - File Processing (pipe-delimited text)
    - AVL Tree integration
*/

#ifndef FILE_H
#define FILE_H

#include "bst.h"

#define DB_FILE "products.txt"

// FILE I/O ---------------------------------------------------------------------------

void  saveToFile   (Node *root);
Node *loadFromFile (Node *root);

// PRODUCT MANAGEMENT ---------------------------------------------------------------------------

Node *updateProduct  (Node *root, int id);
Node *restockProduct (Node *root, int id, int addStock);

// STATISTICS ---------------------------------------------------------------------------

void  showStatistics         (Node *root);
float calcTotalInventoryValue(Node *root);

// helper: count total stock across all products
int calcTotalStock(Node *root);

// helper: count how many products have discount > 0
int countDiscountedProducts(Node *root);

// VALIDATION ---------------------------------------------------------------------------

int isValidPrice   (float price);
int isValidStock   (int stock);
int isValidDiscount(float discount);
int isValidName    (char name[]);
int isValidCategory(char category[]);
int isUniqueId     (Node *root, int id);

#endif // FILE_H
