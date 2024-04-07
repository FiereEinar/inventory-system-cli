#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#include "main.h"

int main()
{
    askUserToFullScreen();
    srand(time(NULL));
    struct Node *head = NULL;
    struct ProfitPerMonth monthlyProfits[MONTHS];
    initMonthlyProfits(monthlyProfits);             // fill the monthly values with zero and sets monthly names
    getItemsFromStorage(&head);
    char action;
    
    // Adding test items for development purposes only
    // addItemToList(&head, monthlyProfits, "pencil", 25, 10, 5, 15);
    // addItemToList(&head, monthlyProfits, "paper", 10, 55, 44, 15);
    // addItemToList(&head, monthlyProfits, "scissors", 15, 56, 47, 15);
    // addItemToList(&head, monthlyProfits, "ballpen", 18, 12, 10, 15);
    // addItemToList(&head, monthlyProfits, "scraper", 5, 200, 150, 15);
    // addItemToList(&head, monthlyProfits, "comb", 4, 46, 40, 15);

    while (true) {
        system("cls");
        menuPage();

        bannerUserInput();
        scanf("%c", &action);

        switch (action)
        {
        case '1':
            viewInventory(&head, monthlyProfits);
            break;
        case '2':
            sellItemHandler(&head, monthlyProfits);
            break;
        case '3':
            system("cls");
            viewReports(monthlyProfits);
            break;
        case 'q':
            return 0;
        }
    }
    // TODO: maybe clear the memory of head? there's an available function for that
}

void viewInventory(struct Node **head, struct ProfitPerMonth monthlyProfits[])
{
    char action;
    system("cls");

    inventoryPage(head);

    while (true) {
        bannerUserInput();
        fflush(stdin);
        scanf("%c", &action);

        switch (action)
        {
        case '1':
            addItemHandler(head, monthlyProfits);
            system("cls");
            inventoryPage(head);
            break;
        case '2':
            deleteItemHandler(head, monthlyProfits);
            system("cls");
            inventoryPage(head);
            break;
        case '3':
            editItemHandler(head);
            system("cls");
            inventoryPage(head);
            break;
        case '4':
            restockItem(head, monthlyProfits);
            system("cls");
            inventoryPage(head);
            break;
        case '5':
            searchItem(head);
            break;
        case '6':
            viewItemDetails(head);
            break;
        case 'b':
            return;
        }
    }
}
