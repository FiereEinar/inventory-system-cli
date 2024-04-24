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

    // TODO: transfer this here 
    settings.sortBy = 0;
    struct Node *head = NULL;                                       // this is where the items are stored
    struct ReportPerMonth monthlyProfits[MONTHS];                   // this is where the reports are stored
    char categories[MAX_CATEGORIES][CATEGORY_NAME_LEN];             // this is where the categories are stored
    int categoriesLen = 0;                                          // this keeps track of the lenght of categories
    struct Cart cart;                                               // this stores the items in the cart
    cart.amountOfItems = 0;                                         // this keeps track of the lenght of cart items
    char action;                                                    // action of user

    // initializations
    initMonthlyProfits(monthlyProfits);                             // fill all the values with zero and sets monthly names
    getItemsFromStorage(&head);
    getCategoriesFromStorage(categories, &categoriesLen);
    checkReportsFromStorage(monthlyProfits);                        // check the storage if there is any record, if yes then read it, if no then make one and initialize it with zeros

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
            viewPos(&cart, &head, monthlyProfits);
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

// all the funcitons defined in main are page handlers

// TODO: maybe add sorting feature? or just show stocks that are low
void viewInventory(struct Node **head, struct ReportPerMonth monthlyProfits[], char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    char action;

    while (true) {
        system("cls");
        // TODO: sort here based on sortBy value first
        inventoryPage(head);

        bannerUserInput();
        fflush(stdin);
        scanf("%c", &action);

        switch (action)
        {
        case '1':
            addItemHandler(head, monthlyProfits, categories, categoriesLen);
            break;
        case '2':
            deleteItemHandler(head, monthlyProfits);
            break;
        case '3':
            editItemHandler(head, categories, categoriesLen);
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
            viewCategories(head, categories, categoriesLen);
            break;
        case 's':
            sortItemsHandler(head);
            break;
        case 'b':
            return;
        }
    }
}

void viewCategories(struct Node **head, char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
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
        case '3':
            editCategoryHandler(head, categories, categoriesLen);
            break;
        case '4':
            viewCategoryItems(head, categories, categoriesLen);
            break;
        case 'b':
            return;
        }
    }
}

// TODO: add total profit from purchases at the bottom
void viewPos(struct Cart *cart, struct Node **head, struct ReportPerMonth monthlyProfits[])
{
    char action;

    while (true){
        system("cls");
        pointOfSalePage(cart->items, &cart->amountOfItems);
        bannerUserInput();
        scanf("%c", &action);

        switch (action)
        {
        case '1':
            addCartItemHandler(cart, head);
            break;
        case '2':
            deleteCartItemHandler(cart);
            break;
        case '3':
            checkoutHandler(cart, head, monthlyProfits);
            break;
        case '4':
            resetCartHandler(cart);
            break;
        case 'r':
            viewReceipts();
            break;
        case 'b':
            return;
        }
    }
}

void viewReceipts()
{
    char action;
    
    while (true)
    {
        system("cls");
        recieptsPage();
        bannerUserInput();
        scanf("%c", &action);

        switch (action)
        {
        case '1':
            viewReceiptHandler();
            break;
        
        case 'b':
            return;
        }
    }   
}

// TODO: allow editing for records
void viewReports(struct ReportPerMonth monthlyProfits[])
{
    char action[2];

    while (true) {
        system("cls");
        salesReportPage(monthlyProfits);

        bannerUserInput();
        fflush(stdin);
        scanf("%s", action);

        if (strcmp(action, "b") == 0 || strcmp(action, "B") == 0) return;

        // if the user enters the number of month, it will be a string. so convert it to int
        int month = atoi(action) - 1;

        if (month < 0 || month > 11) continue;

        // render the report per day by passing the record using the month entered
        system("cls");
        salesPerDayReportPage(monthlyProfits[month].day, monthlyProfits, month);
        
        // allows the user to go back to monthly report
        bannerUserInput();
        fflush(stdin);
        scanf("%s", action);
    }
}

