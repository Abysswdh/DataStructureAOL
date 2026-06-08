/*
    Person 3: Damario | [2902601064]
    UI/UX & Filtering Features — Implementation

    E-Commerce Product Management System

    Using:
    - Console-based UI with formatted table display
    - AVL Tree integration (Person 1 & 2)
    - Recursive in-order filtering for category, discount, stock, budget
    - Input validation with safeReadInt/safeReadFloat (no more scanf bugs)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "menu.h"
#include "file.h"

// DISPLAY HELPERS ---------------------------------------------------------------------------

// print the top border and column headers for the product table
void printTableHeader(void) {
    printf("\n");
    printf("+------+--------------------------------+--------------------+-------+-----------------+----------+\n");
    printf("| %-4s | %-30s | %-18s | %-5s | %-15s | %-8s |\n",
           "ID", "Product Name", "Price (Rp)", "Stock", "Category", "Discount");
    printf("+------+--------------------------------+--------------------+-------+-----------------+----------+\n");
}

// print one product row formatted to match the header columns
// truncates name to 30 chars to keep table aligned
void printProductRow(Product p) {
    char truncName[31];
    strncpy(truncName, p.name, 30);
    truncName[30] = '\0';
    // if name was truncated, show "..." at the end
    if (strlen(p.name) > 30) {
        truncName[27] = '.';
        truncName[28] = '.';
        truncName[29] = '.';
    }
    printf("| %-4d | %-30s | %18.2f | %5d | %-15s | %5.1f%%  |\n",
           p.id, truncName, p.price, p.stock, p.category, p.discount);
}

// print the closing border line
void printTableFooter(void) {
    printf("+------+--------------------------------+--------------------+-------+-----------------+----------+\n");
    printf("\n");
}

// FILTERING ---------------------------------------------------------------------------

// recursive in-order traversal: print products matching the given category
void filterByCategory(Node *root, char category[]) {
    if (root == NULL) return;

    filterByCategory(root->left, category);

    if (strcmp(root->data.category, category) == 0) {
        printProductRow(root->data);
    }

    filterByCategory(root->right, category);
}

// recursive in-order traversal: print products with discount >= minDiscount
void filterByDiscount(Node *root, float minDiscount) {
    if (root == NULL) return;

    filterByDiscount(root->left, minDiscount);

    if (root->data.discount >= minDiscount) {
        printProductRow(root->data);
    }

    filterByDiscount(root->right, minDiscount);
}

// recursive in-order traversal: print products with stock >= minStock
void filterByStock(Node *root, int minStock) {
    if (root == NULL) return;

    filterByStock(root->left, minStock);

    if (root->data.stock >= minStock) {
        printProductRow(root->data);
    }

    filterByStock(root->right, minStock);
}

// recursive in-order traversal: print products where final price (after discount) <= budget and in stock
void budgetRecommend(Node *root, float budget) {
    if (root == NULL) return;

    budgetRecommend(root->left, budget);

    float finalPrice = root->data.price * (1.0 - root->data.discount / 100.0);
    if (finalPrice <= budget && root->data.stock > 0) {
        char truncName[31];
        strncpy(truncName, root->data.name, 30);
        truncName[30] = '\0';
        if (strlen(root->data.name) > 30) {
            truncName[27] = '.';
            truncName[28] = '.';
            truncName[29] = '.';
        }
        printf("| %-4d | %-30s | %18.2f | %5d | %-15s | %5.1f%%  | Rp %13.2f |\n",
               root->data.id,
               truncName,
               root->data.price,
               root->data.stock,
               root->data.category,
               root->data.discount,
               finalPrice);
    }

    budgetRecommend(root->right, budget);
}

// MENU SCREENS ---------------------------------------------------------------------------

// display the home screen with app title and mode selection
void showHomeScreen(void) {
    printf("\033[H\033[J");   // clear screen
    printf("====================================================\n");
    printf("      E-COMMERCE PRODUCT MANAGEMENT SYSTEM\n");
    printf("====================================================\n");
    printf("    System uses AVL Tree for efficient product\n");
    printf("    filtering with automatic balancing for\n");
    printf("    optimized searching.\n");
    printf("====================================================\n\n");
    printf("  1. Customer Mode\n");
    printf("  2. Admin Mode\n");
    printf("  3. Exit\n\n");
    printf(">> ");
}

// CUSTOMER MENU ---------------------------------------------------------------------------

// customer menu: view and search only, no data modification
void customerMenu(Node **rootPtr) {
    int choice = -1;

    do {
        printf("\033[H\033[J");   // clear screen
        printf("====================================================\n");
        printf("              CUSTOMER MENU\n");
        printf("====================================================\n\n");
        printf("  1. View All Products (Low to High)\n");
        printf("  2. View All Products (High to Low)\n");
        printf("  3. Search by Price Range\n");
        printf("  4. Filter by Category\n");
        printf("  5. Filter by Discount\n");
        printf("  6. Budget Recommendation\n");
        printf("  7. View Cheapest & Most Expensive\n");
        printf("  0. Back to Home\n\n");
        printf(">> ");
        if (!safeReadInt(&choice)) {
            printf("\n[!] Invalid input. Please enter a number (0-7).\n");
            pressEnterToContinue();
            choice = -1;
            continue;
        }

        // Option 1: view all products sorted by price ascending
        if (choice == 1) {
            printf("\033[H\033[J");
            printf("--- All Products (Price: Low -> High) ---\n");

            if (countNodes(*rootPtr) == 0) {
                printf("\nThere is no data !\n");
            } else {
                printTableHeader();
                inorder(*rootPtr);
                printTableFooter();
                printf("  Total products: %d\n", countNodes(*rootPtr));
            }

            pressEnterToContinue();
        }

        // Option 2: view all products sorted by price descending
        else if (choice == 2) {
            printf("\033[H\033[J");
            printf("--- All Products (Price: High -> Low) ---\n");

            if (countNodes(*rootPtr) == 0) {
                printf("\nThere is no data !\n");
            } else {
                printTableHeader();
                reverseInorder(*rootPtr);
                printTableFooter();
                printf("  Total products: %d\n", countNodes(*rootPtr));
            }

            pressEnterToContinue();
        }

        // Option 3: search products within a price range
        else if (choice == 3) {
            printf("\033[H\033[J");
            printf("--- Search by Price Range ---\n\n");

            float minPrice = 0, maxPrice = 0;
            int valid = 0;

            do {
                printf("Input minimum price[> 0]: ");
                valid = safeReadFloat(&minPrice);
                if (!valid) {
                    printf("[!] Invalid input. Please enter a number.\n");
                } else if (minPrice <= 0) {
                    printf("[!] Minimum price must be greater than 0.\n");
                    valid = 0;
                }
            } while (!valid);

            do {
                printf("Input maximum price[> min]: ");
                valid = safeReadFloat(&maxPrice);
                if (!valid) {
                    printf("[!] Invalid input. Please enter a number.\n");
                } else if (maxPrice <= minPrice) {
                    printf("[!] Maximum price must be greater than minimum (%.2f).\n", minPrice);
                    valid = 0;
                }
            } while (!valid);

            printf("\nProducts in range Rp %.2f - Rp %.2f:\n", minPrice, maxPrice);
            printTableHeader();
            searchRange(*rootPtr, minPrice, maxPrice);
            printTableFooter();

            pressEnterToContinue();
        }

        // Option 4: filter products by category
        else if (choice == 4) {
            printf("\033[H\033[J");
            printf("--- Filter by Category ---\n\n");
            printf("Available categories:\n");
            printf("  1. Electronics\n");
            printf("  2. Food\n");
            printf("  3. Fashion\n");
            printf("  4. Sports\n");
            printf("  5. Books\n");
            printf("  6. Home\n\n");

            int catChoice = 0;
            int catValid = 0;
            char category[50];

            do {
                printf("Select category[1-6]: ");
                catValid = safeReadInt(&catChoice);
                if (!catValid || catChoice < 1 || catChoice > 6) {
                    printf("[!] Please enter a number between 1 and 6.\n");
                    catValid = 0;
                }
            } while (!catValid);

            // map number to category string
            if (catChoice == 1)      strcpy(category, "Electronics");
            else if (catChoice == 2) strcpy(category, "Food");
            else if (catChoice == 3) strcpy(category, "Fashion");
            else if (catChoice == 4) strcpy(category, "Sports");
            else if (catChoice == 5) strcpy(category, "Books");
            else if (catChoice == 6) strcpy(category, "Home");

            printf("\nShowing products in category: %s\n", category);
            printTableHeader();
            filterByCategory(*rootPtr, category);
            printTableFooter();

            pressEnterToContinue();
        }

        // Option 5: filter products by minimum discount percentage
        else if (choice == 5) {
            printf("\033[H\033[J");
            printf("--- Filter by Discount ---\n\n");

            float minDiscount = -1;
            int discValid = 0;

            do {
                printf("Input minimum discount[0-100]%%: ");
                discValid = safeReadFloat(&minDiscount);
                if (!discValid) {
                    printf("[!] Invalid input. Please enter a number.\n");
                } else if (minDiscount < 0 || minDiscount > 100) {
                    printf("[!] Discount must be between 0 and 100.\n");
                    discValid = 0;
                }
            } while (!discValid);

            printf("\nShowing products with discount >= %.1f%%:\n", minDiscount);
            printTableHeader();
            filterByDiscount(*rootPtr, minDiscount);
            printTableFooter();

            pressEnterToContinue();
        }

        // Option 6: budget recommendation — show affordable products after discount
        else if (choice == 6) {
            printf("\033[H\033[J");
            printf("--- Budget Recommendation ---\n\n");

            float budget = 0;
            int budgetValid = 0;

            do {
                printf("Input your budget (Rp)[> 0]: ");
                budgetValid = safeReadFloat(&budget);
                if (!budgetValid) {
                    printf("[!] Invalid input. Please enter a number.\n");
                } else if (budget <= 0) {
                    printf("[!] Budget must be greater than 0.\n");
                    budgetValid = 0;
                }
            } while (!budgetValid);

            printf("\nProducts you can afford with Rp %.2f (after discount, in stock):\n", budget);
            printf("\n");
            printf("+------+--------------------------------+--------------------+-------+-----------------+----------+------------------+\n");
            printf("| %-4s | %-30s | %-18s | %-5s | %-15s | %-8s | %-16s |\n",
                   "ID", "Product Name", "Price (Rp)", "Stock", "Category", "Discount", "Final Price");
            printf("+------+--------------------------------+--------------------+-------+-----------------+----------+------------------+\n");
            budgetRecommend(*rootPtr, budget);
            printf("+------+--------------------------------+--------------------+-------+-----------------+----------+------------------+\n");
            printf("\n");

            pressEnterToContinue();
        }

        // Option 7: view cheapest and most expensive products
        else if (choice == 7) {
            printf("\033[H\033[J");
            printf("--- Cheapest & Most Expensive ---\n\n");

            if (countNodes(*rootPtr) == 0) {
                printf("There is no data !\n");
            } else {
                Node *cheapest  = findCheapest(*rootPtr);
                Node *expensive = findMostExpensive(*rootPtr);

                printf("========================================\n");
                printf("  CHEAPEST PRODUCT\n");
                printf("========================================\n");
                printf("  ID       : %d\n",       cheapest->data.id);
                printf("  Name     : %s\n",       cheapest->data.name);
                printf("  Category : %s\n",       cheapest->data.category);
                printf("  Price    : Rp %.2f\n",  cheapest->data.price);
                printf("  Stock    : %d\n",       cheapest->data.stock);
                printf("  Discount : %.1f%%\n",   cheapest->data.discount);
                printf("========================================\n\n");

                printf("========================================\n");
                printf("  MOST EXPENSIVE PRODUCT\n");
                printf("========================================\n");
                printf("  ID       : %d\n",       expensive->data.id);
                printf("  Name     : %s\n",       expensive->data.name);
                printf("  Category : %s\n",       expensive->data.category);
                printf("  Price    : Rp %.2f\n",  expensive->data.price);
                printf("  Stock    : %d\n",       expensive->data.stock);
                printf("  Discount : %.1f%%\n",   expensive->data.discount);
                printf("========================================\n");
            }

            pressEnterToContinue();
        }

        // Option 0: back to home
        else if (choice == 0) {
            // do nothing, loop exits
        }

        // invalid choice
        else {
            printf("\nPlease choose between 0 to 7!\n");
            pressEnterToContinue();
        }

    } while (choice != 0);
}

// ADMIN MENU ---------------------------------------------------------------------------

// admin menu: full CRUD access + statistics
void adminMenu(Node **rootPtr) {
    int choice = -1;

    do {
        printf("\033[H\033[J");   // clear screen
        printf("====================================================\n");
        printf("                ADMIN MENU\n");
        printf("====================================================\n\n");
        printf("  1. Add Product\n");
        printf("  2. Delete Product\n");
        printf("  3. Update Product\n");
        printf("  4. Restock Product\n");
        printf("  5. View All Products\n");
        printf("  6. View Statistics\n");
        printf("  7. Filter by Stock Availability\n");
        printf("  0. Back to Home\n\n");
        printf(">> ");
        if (!safeReadInt(&choice)) {
            printf("\n[!] Invalid input. Please enter a number (0-7).\n");
            pressEnterToContinue();
            choice = -1;
            continue;
        }

        // Option 1: add a new product with full validation
        if (choice == 1) {
            printf("\033[H\033[J");
            printf("--- Add New Product ---\n\n");

            Product p;
            int inputValid = 0;

            // initialize all fields to safe defaults
            p.id = 0;
            p.name[0] = '\0';
            p.category[0] = '\0';
            p.price = 0;
            p.stock = 0;
            p.discount = 0;

            // validate unique ID
            do {
                printf("Input product ID[> 0, unique]: ");
                inputValid = safeReadInt(&p.id);
                if (!inputValid) {
                    printf("[!] Invalid input. Please enter a number.\n");
                } else if (p.id <= 0) {
                    printf("[!] ID must be greater than 0.\n");
                    inputValid = 0;
                } else if (!isUniqueId(*rootPtr, p.id)) {
                    printf("[!] ID %d already exists.\n", p.id);
                    inputValid = 0;
                }
            } while (!inputValid);

            // validate product name
            do {
                printf("Input product name[3-99 chars]: ");
                if (fgets(p.name, sizeof(p.name), stdin) == NULL) p.name[0] = '\0';
                p.name[strcspn(p.name, "\r\n")] = '\0';
                if (!isValidName(p.name)) {
                    printf("[!] Name must be between 3 and 99 characters.\n");
                }
            } while (!isValidName(p.name));

            // validate category by selection
            printf("\nSelect category:\n");
            printf("  1. Electronics\n");
            printf("  2. Food\n");
            printf("  3. Fashion\n");
            printf("  4. Sports\n");
            printf("  5. Books\n");
            printf("  6. Home\n\n");

            int catChoice = 0;
            int catValid = 0;
            do {
                printf("Category[1-6]: ");
                catValid = safeReadInt(&catChoice);
                if (!catValid || catChoice < 1 || catChoice > 6) {
                    printf("[!] Please enter a number between 1 and 6.\n");
                    catValid = 0;
                }
            } while (!catValid);

            if (catChoice == 1)      strcpy(p.category, "Electronics");
            else if (catChoice == 2) strcpy(p.category, "Food");
            else if (catChoice == 3) strcpy(p.category, "Fashion");
            else if (catChoice == 4) strcpy(p.category, "Sports");
            else if (catChoice == 5) strcpy(p.category, "Books");
            else if (catChoice == 6) strcpy(p.category, "Home");

            // validate price
            do {
                printf("Input price (Rp)[> 0]: ");
                inputValid = safeReadFloat(&p.price);
                if (!inputValid) {
                    printf("[!] Invalid input. Please enter a number.\n");
                } else if (!isValidPrice(p.price)) {
                    printf("[!] Price must be greater than 0.\n");
                    inputValid = 0;
                }
            } while (!inputValid);

            // validate stock
            do {
                printf("Input stock[>= 0]: ");
                inputValid = safeReadInt(&p.stock);
                if (!inputValid) {
                    printf("[!] Invalid input. Please enter a number.\n");
                } else if (!isValidStock(p.stock)) {
                    printf("[!] Stock must be 0 or greater.\n");
                    inputValid = 0;
                }
            } while (!inputValid);

            // validate discount
            do {
                printf("Input discount[0-100]%%: ");
                inputValid = safeReadFloat(&p.discount);
                if (!inputValid) {
                    printf("[!] Invalid input. Please enter a number.\n");
                } else if (!isValidDiscount(p.discount)) {
                    printf("[!] Discount must be between 0 and 100.\n");
                    inputValid = 0;
                }
            } while (!inputValid);

            // insert into AVL tree and save
            *rootPtr = insert(*rootPtr, p);
            saveToFile(*rootPtr);

            printf("\n[OK] Product \"%s\" (ID:%d) added successfully!\n", p.name, p.id);

            // show the added product
            printTableHeader();
            printProductRow(p);
            printTableFooter();

            pressEnterToContinue();
        }

        // Option 2: delete a product by ID
        else if (choice == 2) {
            printf("\033[H\033[J");
            printf("--- Delete Product ---\n\n");

            if (countNodes(*rootPtr) == 0) {
                printf("There is no data !\n");
                pressEnterToContinue();
                continue;
            }

            // show current products for reference
            printf("Current products:\n");
            printTableHeader();
            inorder(*rootPtr);
            printTableFooter();

            int id = 0;
            int idValid = 0;

            do {
                printf("Input product ID to delete[> 0]: ");
                idValid = safeReadInt(&id);
                if (!idValid) {
                    printf("[!] Invalid input. Please enter a valid product ID number.\n");
                } else if (id <= 0) {
                    printf("[!] ID must be greater than 0.\n");
                    idValid = 0;
                }
            } while (!idValid);

            Node *found = searchById(*rootPtr, id);
            if (found == NULL) {
                printf("\n[INFO] Product ID %d not found.\n", id);
            } else {
                // confirm deletion
                char confirm;
                char confirmBuf[256];
                printf("\nDeleting: [ID:%d] %s — Rp %.2f\n",
                       found->data.id, found->data.name, found->data.price);

                do {
                    printf("Are you sure?[y|n]: ");
                    if (fgets(confirmBuf, sizeof(confirmBuf), stdin) == NULL) {
                        confirm = 'n';
                        break;
                    }
                    confirmBuf[strcspn(confirmBuf, "\r\n")] = '\0';
                    if (strlen(confirmBuf) == 1 && (confirmBuf[0] == 'y' || confirmBuf[0] == 'n')) {
                        confirm = confirmBuf[0];
                    } else {
                        confirm = '\0';
                        printf("[!] Please enter exactly 'y' or 'n' (lowercase only).\n");
                    }
                } while (confirm != 'y' && confirm != 'n');

                if (confirm == 'y') {
                    *rootPtr = deleteNodeById(*rootPtr, id);
                    saveToFile(*rootPtr);
                    printf("\n[OK] Product ID %d deleted successfully!\n", id);
                } else {
                    printf("\n[INFO] Deletion cancelled.\n");
                }
            }

            pressEnterToContinue();
        }

        // Option 3: update a product by ID (Person 2 handles the input internally)
        else if (choice == 3) {
            printf("\033[H\033[J");
            printf("--- Update Product ---\n\n");

            if (countNodes(*rootPtr) == 0) {
                printf("There is no data !\n");
                pressEnterToContinue();
                continue;
            }

            int id = 0;
            int idValid = 0;

            do {
                printf("Input product ID to update[> 0]: ");
                idValid = safeReadInt(&id);
                if (!idValid) {
                    printf("[!] Invalid input. Please enter a valid product ID number.\n");
                } else if (id <= 0) {
                    printf("[!] ID must be greater than 0.\n");
                    idValid = 0;
                }
            } while (!idValid);

            *rootPtr = updateProduct(*rootPtr, id);

            pressEnterToContinue();
        }

        // Option 4: restock a product by ID
        else if (choice == 4) {
            printf("\033[H\033[J");
            printf("--- Restock Product ---\n\n");

            if (countNodes(*rootPtr) == 0) {
                printf("There is no data !\n");
                pressEnterToContinue();
                continue;
            }

            int id = 0;
            int idValid = 0;

            do {
                printf("Input product ID to restock[> 0]: ");
                idValid = safeReadInt(&id);
                if (!idValid) {
                    printf("[!] Invalid input. Please enter a valid product ID number.\n");
                } else if (id <= 0) {
                    printf("[!] ID must be greater than 0.\n");
                    idValid = 0;
                }
            } while (!idValid);

            Node *found = searchById(*rootPtr, id);
            if (found == NULL) {
                printf("\n[INFO] Product ID %d not found.\n", id);
            } else {
                printf("Current stock for \"%s\": %d\n", found->data.name, found->data.stock);

                int addStock = 0;
                int stockValid = 0;
                do {
                    printf("Input quantity to add[> 0]: ");
                    stockValid = safeReadInt(&addStock);
                    if (!stockValid) {
                        printf("[!] Invalid input. Please enter a number.\n");
                    } else if (addStock <= 0) {
                        printf("[!] Quantity must be greater than 0.\n");
                        stockValid = 0;
                    }
                } while (!stockValid);

                *rootPtr = restockProduct(*rootPtr, id, addStock);
            }

            pressEnterToContinue();
        }

        // Option 5: view all products sorted by price ascending
        else if (choice == 5) {
            printf("\033[H\033[J");
            printf("--- All Products (Admin View) ---\n");

            if (countNodes(*rootPtr) == 0) {
                printf("\nThere is no data !\n");
            } else {
                printTableHeader();
                inorder(*rootPtr);
                printTableFooter();
                printf("  Total products: %d\n", countNodes(*rootPtr));
                printf("  Total inventory value: Rp %.2f\n", calcTotalInventoryValue(*rootPtr));
            }

            pressEnterToContinue();
        }

        // Option 6: view inventory statistics (Person 2's function)
        else if (choice == 6) {
            printf("\033[H\033[J");
            printf("--- Inventory Statistics ---\n");

            showStatistics(*rootPtr);

            pressEnterToContinue();
        }

        // Option 7: filter products by minimum stock availability
        else if (choice == 7) {
            printf("\033[H\033[J");
            printf("--- Filter by Stock Availability ---\n\n");

            int minStock = -1;
            int stockValid = 0;

            do {
                printf("Input minimum stock[>= 0]: ");
                stockValid = safeReadInt(&minStock);
                if (!stockValid) {
                    printf("[!] Invalid input. Please enter a number.\n");
                } else if (minStock < 0) {
                    printf("[!] Stock must be 0 or greater.\n");
                    stockValid = 0;
                }
            } while (!stockValid);

            printf("\nShowing products with stock >= %d:\n", minStock);
            printTableHeader();
            filterByStock(*rootPtr, minStock);
            printTableFooter();

            pressEnterToContinue();
        }

        // Option 0: back to home
        else if (choice == 0) {
            // do nothing, loop exits
        }

        // invalid choice
        else {
            printf("\nPlease choose between 0 to 7!\n");
            pressEnterToContinue();
        }

    } while (choice != 0);
}
