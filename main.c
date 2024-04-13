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
    struct ReportPerMonth monthlyProfits[MONTHS];
    char categories[MAX_CATEGORIES][CATEGORY_NAME_LEN];
    int categoriesLen = 0;
    char action;

    initMonthlyProfits(monthlyProfits);             // fill the monthly values with zero and sets monthly names
    getItemsFromStorage(&head);
    initReportsFromStorage(monthlyProfits);

    while (true) {
        system("cls");
        menuPage();
        bannerUserInput();
        scanf("%c", &action);

        switch (action)
        {
        case '1':
            viewInventory(&head, monthlyProfits, categories, &categoriesLen);
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
}

// handles event cycle of the inventory page
void viewInventory(struct Node **head, struct ReportPerMonth monthlyProfits[], char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    char action;

    while (true) {
        system("cls");
        inventoryPage(head);
        bannerUserInput();
        fflush(stdin);
        scanf("%c", &action);

        switch (action)
        {
        case '1':
            addItemHandler(head, monthlyProfits);
            break;
        case '2':
            deleteItemHandler(head, monthlyProfits);
            break;
        case '3':
            editItemHandler(head);
            break;
        case '4':
            restockItemHandler(head, monthlyProfits);
            break;
        case '5':
            searchItemHandler(head);
            break;
        case '6':
            viewItemDetails(head);
            break;
        case 'c':
            viewCategories(categories, categoriesLen);
            break;
        case 'b':
            return;
        }
    }
}

void viewCategories(char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    char action;

    while (true) {
        system("cls");
        categoryPage(categories, categoriesLen);
        bannerUserInput();
        fflush(stdin);
        scanf("%c", &action);

        switch (action)
        {
        case '1':
            addCategoryHandler(categories, categoriesLen);
            break;
        case '2':
            deleteCategoryHandler(categories, categoriesLen);
            break;
        case 'b':
            return;
        }
    }
}
