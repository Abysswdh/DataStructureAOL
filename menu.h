/*
    Person 3: Damario | [Student ID]
    UI/UX & Filtering Features — Header

    E-Commerce Product Management System

    Using:
    - Console-based UI with formatted table display
    - AVL Tree integration (Person 1 & 2)
    - Category, discount, stock, and budget filtering
*/

#ifndef MENU_H
#define MENU_H

#include "bst.h"

// MENU SCREENS ---------------------------------------------------------------------------

void showHomeScreen  (void);
void customerMenu    (Node **rootPtr);   // pointer-to-pointer so root can change
void adminMenu       (Node **rootPtr);

// FILTERING ---------------------------------------------------------------------------

void filterByCategory (Node *root, char category[]);
void filterByDiscount (Node *root, float minDiscount);
void filterByStock    (Node *root, int minStock);
void budgetRecommend  (Node *root, float budget);

// DISPLAY ---------------------------------------------------------------------------

void printTableHeader (void);
void printProductRow  (Product p);
void printTableFooter (void);

#endif // MENU_H
