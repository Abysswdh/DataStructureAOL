/*
    Person 2: Putra Abyasa Wedha | 2902583635
    File Handling & Product Management — Implementation

    E-Commerce Product Management System

    Using:
    - File Processing (pipe-delimited text)
    - AVL Tree integration (insert, delete, search from Person 1)
    - Auto-save after every data modification
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

// FILE I/O ---------------------------------------------------------------------------

// helper: traverse tree and write each node to file (in-order)
static void saveHelper(Node *node, FILE *file) {
    if (node == NULL) return;
    saveHelper(node->left, file);

    // format: id|name|category|price|stock|discount
    fprintf(file, "%d|%s|%s|%.2f|%d|%.2f\n",
            node->data.id,
            node->data.name,
            node->data.category,
            node->data.price,
            node->data.stock,
            node->data.discount);

    saveHelper(node->right, file);
}

// save all products to file (overwrite mode)
void saveToFile(Node *root) {
    FILE *file = fopen(DB_FILE, "w");
    if (file == NULL) {
        printf("                                  [ERROR] Cannot open file for saving!\n");
        return;
    }
    saveHelper(root, file);
    fclose(file);
}

// load products from file, rebuild tree using insert()
Node *loadFromFile(Node *root) {
    FILE *file = fopen(DB_FILE, "r");
    if (file == NULL) return root;  // file doesn't exist yet = OK

    Product p;
    while (fscanf(file, "%d|%99[^|]|%49[^|]|%f|%d|%f\n",
                  &p.id, p.name, p.category,
                  &p.price, &p.stock, &p.discount) == 6) {
        root = insert(root, p);
    }

    fclose(file);
    printf("                                  [OK] Data loaded from %s\n", DB_FILE);
    return root;
}

// PRODUCT MANAGEMENT ---------------------------------------------------------------------------

// update product fields by ID
// if price changes, must delete + re-insert to maintain BST ordering
Node *updateProduct(Node *root, int id) {
    Node *found = searchById(root, id);
    if (found == NULL) {
        printf("                                  [INFO] Product ID %d not found.\n", id);
        return root;
    }

    Product updated = found->data;
    int choice;
    int inputValid;

    printf("\n                                  --- Current Product Data ---\n");
    printf("                                  ID       : %d\n", found->data.id);
    printf("                                  Name     : %s\n", found->data.name);
    printf("                                  Category : %s\n", found->data.category);
    printf("                                  Price    : Rp %.2f\n", found->data.price);
    printf("                                  Stock    : %d\n", found->data.stock);
    printf("                                  Discount : %.1f%%\n", found->data.discount);
    printf("                                  ----------------------------\n\n");

    printf("                                  What do you want to update?\n");
    printf("                                  1. Name\n");
    printf("                                  2. Category\n");
    printf("                                  3. Price\n");
    printf("                                  4. Stock\n");
    printf("                                  5. Discount\n");
    printf("                                  6. Update All\n");
    printf("                                  >> ");
    if (!safeReadInt(&choice) || choice < 1 || choice > 6) {
        printf("                                  [!] Invalid choice. Please enter a number between 1 and 6.\n");
        return root;
    }

    // update name
    if (choice == 1 || choice == 6) {
        char newName[100];
        do {
            printf("                                  Input new name[3-99 chars]: ");
            if (fgets(newName, sizeof(newName), stdin) == NULL) newName[0] = '\0';
            newName[strcspn(newName, "\r\n")] = '\0';
        } while (!isValidName(newName));
        strcpy(updated.name, newName);
    }

    // update category
    if (choice == 2 || choice == 6) {
        printf("\n                                  Select category:\n");
        printf("                                    1. Electronics\n");
        printf("                                    2. Food\n");
        printf("                                    3. Fashion\n");
        printf("                                    4. Sports\n");
        printf("                                    5. Books\n");
        printf("                                    6. Home\n\n");

        int catChoice;
        int catValid;
        do {
            printf("                                  Category[1-6]: ");
            catValid = safeReadInt(&catChoice);
            if (!catValid || catChoice < 1 || catChoice > 6) {
                printf("                                  [!] Please enter a number between 1 and 6.\n");
                catValid = 0;
            }
        } while (!catValid);

        if (catChoice == 1)      strcpy(updated.category, "Electronics");
        else if (catChoice == 2) strcpy(updated.category, "Food");
        else if (catChoice == 3) strcpy(updated.category, "Fashion");
        else if (catChoice == 4) strcpy(updated.category, "Sports");
        else if (catChoice == 5) strcpy(updated.category, "Books");
        else if (catChoice == 6) strcpy(updated.category, "Home");
    }

    // update price
    if (choice == 3 || choice == 6) {
        float newPrice;
        do {
            printf("                                  Input new price[> 0]: ");
            inputValid = safeReadFloat(&newPrice);
            if (!inputValid) {
                printf("                                  [!] Invalid input. Please enter a number.\n");
            } else if (!isValidPrice(newPrice)) {
                printf("                                  [!] Price must be greater than 0.\n");
                inputValid = 0;
            }
        } while (!inputValid);
        updated.price = newPrice;
    }

    // update stock
    if (choice == 4 || choice == 6) {
        int newStock;
        do {
            printf("                                  Input new stock[>= 0]: ");
            inputValid = safeReadInt(&newStock);
            if (!inputValid) {
                printf("                                  [!] Invalid input. Please enter a number.\n");
            } else if (!isValidStock(newStock)) {
                printf("                                  [!] Stock must be 0 or greater.\n");
                inputValid = 0;
            }
        } while (!inputValid);
        updated.stock = newStock;
    }

    // update discount
    if (choice == 5 || choice == 6) {
        float newDisc;
        do {
            printf("                                  Input new discount[0-100]%%: ");
            inputValid = safeReadFloat(&newDisc);
            if (!inputValid) {
                printf("                                  [!] Invalid input. Please enter a number.\n");
            } else if (!isValidDiscount(newDisc)) {
                printf("                                  [!] Discount must be between 0 and 100.\n");
                inputValid = 0;
            }
        } while (!inputValid);
        updated.discount = newDisc;
    }

    // if price changed, delete and re-insert (price = BST key)
    if (updated.price != found->data.price) {
        root = deleteNodeById(root, id);
        root = insert(root, updated);
    } else {
        found->data = updated;
    }

    saveToFile(root);
    printf("\n                                  [OK] Product ID %d updated successfully!\n", id);
    return root;
}

// add stock to existing product
Node *restockProduct(Node *root, int id, int addStock) {
    Node *found = searchById(root, id);
    if (found == NULL) {
        printf("                                  [INFO] Product ID %d not found.\n", id);
        return root;
    }

    int oldStock = found->data.stock;
    found->data.stock += addStock;

    printf("                                  [OK] %s restocked: %d -> %d (+%d)\n",
           found->data.name, oldStock, found->data.stock, addStock);

    saveToFile(root);
    return root;
}

// STATISTICS ---------------------------------------------------------------------------

// helper: total inventory value (discounted price * stock) recursive
static float calcValueHelper(Node *root) {
    if (root == NULL) return 0;

    float discountedPrice = root->data.price * (1.0 - root->data.discount / 100.0);
    float nodeValue = discountedPrice * root->data.stock;

    return nodeValue + calcValueHelper(root->left) + calcValueHelper(root->right);
}

float calcTotalInventoryValue(Node *root) {
    return calcValueHelper(root);
}

// helper: sum all stock across products
int calcTotalStock(Node *root) {
    if (root == NULL) return 0;
    return root->data.stock + calcTotalStock(root->left) + calcTotalStock(root->right);
}

// helper: count products with active discounts
int countDiscountedProducts(Node *root) {
    if (root == NULL) return 0;

    int count = (root->data.discount > 0) ? 1 : 0;
    return count + countDiscountedProducts(root->left) + countDiscountedProducts(root->right);
}

// display full inventory summary
void showStatistics(Node *root) {
    int totalProducts = countNodes(root);

    printf("\n                                  ========================================\n");
    printf("                                          INVENTORY STATISTICS\n");
    printf("                                  ========================================\n");

    if (totalProducts == 0) {
        printf("                                    No products in inventory.\n");
        printf("                                  ========================================\n");
        return;
    }

    printf("                                    Total products     : %d\n", totalProducts);
    printf("                                    Total stock units  : %d\n", calcTotalStock(root));
    printf("                                    Tree height (BST)  : %d\n", getTreeHeight(root));
    printf("                                    Discounted items   : %d\n", countDiscountedProducts(root));

    Node *cheapest  = findCheapest(root);
    Node *expensive = findMostExpensive(root);

    if (cheapest)
        printf("                                    Cheapest product   : %s (Rp %.2f)\n",
               cheapest->data.name, cheapest->data.price);
    if (expensive)
        printf("                                    Most expensive     : %s (Rp %.2f)\n",
               expensive->data.name, expensive->data.price);

    printf("                                    --------------------------------\n");
    printf("                                    Total inv. value   : Rp %.2f\n", calcTotalInventoryValue(root));
    printf("                                  ========================================\n");
}

// VALIDATION ---------------------------------------------------------------------------

// price must be positive
int isValidPrice(float price) {
    return price > 0;
}

// stock must be non-negative
int isValidStock(int stock) {
    return stock >= 0;
}

// discount must be between 0 and 100 inclusive
int isValidDiscount(float discount) {
    return discount >= 0 && discount <= 100;
}

// name must be 3-99 characters
int isValidName(char name[]) {
    int len = strlen(name);
    return len >= 3 && len <= 99;
}

// must be one of the predefined categories (case-sensitive)
int isValidCategory(char category[]) {
    return (strcmp(category, "Electronics") == 0 ||
            strcmp(category, "Food") == 0 ||
            strcmp(category, "Fashion") == 0 ||
            strcmp(category, "Sports") == 0 ||
            strcmp(category, "Books") == 0 ||
            strcmp(category, "Home") == 0);
}

// check if ID is not already in use
int isUniqueId(Node *root, int id) {
    return (searchById(root, id) == NULL);
}

// SAFE INPUT HELPERS ---------------------------------------------------------------------------

// read an integer safely from stdin (full line read + strict validation)
// returns 1 on success (value stored in *out), 0 on failure (*out unchanged)
int safeReadInt(int *out) {
    char buf[256];
    if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;

    // strip trailing newline/carriage return
    buf[strcspn(buf, "\r\n")] = '\0';

    // reject empty input
    if (buf[0] == '\0') return 0;

    char *endptr;
    long val = strtol(buf, &endptr, 10);

    // no conversion performed (e.g. input was only letters/spaces)
    if (endptr == buf) return 0;

    // skip trailing whitespace
    while (*endptr == ' ' || *endptr == '\t') endptr++;

    // reject if there are leftover non-whitespace characters
    if (*endptr != '\0') return 0;

    *out = (int)val;
    return 1;
}

// read a float safely from stdin (full line read + strict validation)
// returns 1 on success (value stored in *out), 0 on failure (*out unchanged)
int safeReadFloat(float *out) {
    char buf[256];
    if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;

    // strip trailing newline/carriage return
    buf[strcspn(buf, "\r\n")] = '\0';

    // reject empty input
    if (buf[0] == '\0') return 0;

    char *endptr;
    float val = strtof(buf, &endptr);

    // no conversion performed
    if (endptr == buf) return 0;

    // skip trailing whitespace
    while (*endptr == ' ' || *endptr == '\t') endptr++;

    // reject if there are leftover non-whitespace characters
    if (*endptr != '\0') return 0;

    *out = val;
    return 1;
}

// flush any leftover characters from stdin buffer
// prevents buffer desync between getch() and fgets()
void flushStdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// unified "Press Enter to continue" that uses fgets
// replaces inconsistent getch()/getchar() calls
void pressEnterToContinue(void) {
    printf("\n                                  Press Enter to continue ...");
    char buf[256];
    fgets(buf, sizeof(buf), stdin);
}
