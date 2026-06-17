/*
    Person 3: Damario
    UI/UX & Filtering Features — Header

    E-Commerce Product Management System

    Using:
    - Console-based UI with formatted table display
    - BST integration (Person 1 & 2)
    - Category, discount, stock, and budget filtering
*/

#ifndef MENU_H
#define MENU_H

#include "bst.h"

// ANSI Color Codes
#define COLOR_RESET   "\033[0m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_RED     "\033[31m"


// MENU SCREENS ---------------------------------------------------------------------------

void printMargin(void);
void printVerticalPadding(int lines);
void printTableMargin(void);
void printBudgetMargin(void);

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
