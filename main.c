/*
    E-Commerce Product Management System
    Using BST

    Person 1: Noel (Manuel Fides) — BST Core Logic
    Person 2: Aby (Putra Abyasa Wedha) — File Handling & Product Management
    Person 3: Damario — UI/UX & Filtering Features
*/

#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include "file.h"
#include "menu.h"

// MAIN ---------------------------------------------------------------------------

int main() {
    system("mode con: cols=120 lines=40");
    
    Node *root = NULL;

    // auto-load from file at startup
    root = loadFromFile(root);

    int choice = -1;
    do {
        showHomeScreen();
        if (!safeReadInt(&choice)) {
            printf("\n                                  [!] Invalid input. Please enter a number (1-3).\n");
            pressEnterToContinue();
            choice = -1;
            continue;
        }

        switch (choice) {
            case 1:
                customerMenu(&root);
                break;
            case 2:
                adminMenu(&root);
                break;
            case 3:
                printf("\n                                  Saving data...\n");
                saveToFile(root);
                printf("                                  Goodbye!\n");
                break;
            default:
                printf("\n                                  Please choose 1-3!\n");
                pressEnterToContinue();
        }
    } while (choice != 3);

    // cleanup memory
    freeTree(root);
    return 0;
}
