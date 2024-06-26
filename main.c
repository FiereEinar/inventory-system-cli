#include "main.h"

void main_initialize(struct Node **head, struct Cart *cart, struct ReportPerMonth monthlyProfits[], char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    cart->amountOfItems = 0;                                                // this keeps track of the lenght of cart items
    settings.sortBy = 7;                                                    // sorting setting is initialized by 7. First Added. See the Settings header to view the options
    strcpy(settings.shopName, "[Default Shop Name]");
    strcpy(settings.shopLocation, "[Default Shop Location]");
    sales_initMonthlyProfits(monthlyProfits);                               // fill all the values with zero and sets monthly names
    
    storage_getSettingsFromStorage();
    storage_getItemsFromStorage(head);
    storage_getCategoriesFromStorage(categories, categoriesLen);
    storage_checkReportsFromStorage(monthlyProfits);                        // check the storage if there is any record, if yes then read it, if no then make one and initialize it with zeros
}

int main()
{
    display_askUserToFullScreen();
    srand(time(NULL));

    struct Node *head = NULL;                                               // this is where the items are stored
    struct ReportPerMonth monthlyProfits[MONTHS];                           // this is where the reports are stored
    char categories[MAX_CATEGORIES][CATEGORY_NAME_LEN];                     // this is where the categories are stored, just an array of strings
    int categoriesLen = 0;                                                  // this keeps track of the lenght of categories
    struct Cart cart;                                                       // this stores the items in the cart
    char action;                                                            // action of user

    main_initialize(&head, &cart, monthlyProfits, categories, &categoriesLen);

    while (true) {
        system("cls");
        display_menuPage();

        bannerUserInput();
        scanf("%c", &action);
        fflush(stdin);

        switch (action)
        {
        case '1':
            main_inventoryPageSessionHandler(&head, monthlyProfits, categories, &categoriesLen);
            break;
        case '2':
            main_posPageSessionHandler(&cart, &head, monthlyProfits);
            break;
        case '3':
            main_reportsPageSessionHandler(monthlyProfits);
            break;
        case '4':
            main_settingsPageSessionHandler();
            break;
        case 'q':
            return 0;
        }
    }
}

// all the functions defined in main are page handlers

// TODO: maybe add sorting feature? or just show stocks that are low
void main_inventoryPageSessionHandler(struct Node **head, struct ReportPerMonth monthlyProfits[], char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    char action;

    while (true) {
        system("cls");
        display_inventoryPage(head);

        bannerUserInput();
        fflush(stdin);
        scanf("%c", &action);
        fflush(stdin);

        switch (action)
        {
        case '1':
            item_addItemHandler(head, monthlyProfits, categories, categoriesLen);
            break;
        case '2':
            item_deleteItemHandler(head, monthlyProfits);
            break;
        case '3':
            item_editItemHandler(head, categories, categoriesLen);
            break;
        case '4':
            item_restockItemHandler(head, monthlyProfits);
            break;
        case '5':
            item_searchItemHandler(head);
            break;
        case '6':
            item_viewItemDetailsHandler(head);
            break;
        case 'c':
            main_categoriesPageSessionHandler(head, categories, categoriesLen);
            break;
        case 's':
            item_changeSortingHandler(head);
            break;
        case 'b':
            return;
        }
    }
}

void main_categoriesPageSessionHandler(struct Node **head, char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    char action;

    while (true) {
        system("cls");
        display_categoryPage(categories, categoriesLen);
        
        bannerUserInput();
        fflush(stdin);
        scanf("%c", &action);
        fflush(stdin);

        switch (action)
        {
        case '1':
            category_addCategoryHandler(categories, categoriesLen);
            break;
        case '2':
            category_deleteCategoryHandler(categories, categoriesLen);
            break;
        case '3':
            category_editCategoryHandler(head, categories, categoriesLen);
            break;
        case '4':
            category_viewCategoryItems(head, categories, categoriesLen);
            break;
        case 'b':
            return;
        }
    }
}

// TODO: add total profit from purchases at the bottom
void main_posPageSessionHandler(struct Cart *cart, struct Node **head, struct ReportPerMonth monthlyProfits[])
{
    char action;

    while (true){
        system("cls");
        display_pointOfSalePage(cart->items, &cart->amountOfItems);
        
        bannerUserInput();
        scanf("%c", &action);
        fflush(stdin);

        switch (action)
        {
        case '1':
            pos_addCartItemHandler(cart, head);
            break;
        case '2':
            pos_deleteCartItemHandler(cart);
            break;
        case '3':
            pos_checkoutHandler(cart, head, monthlyProfits);
            break;
        case '4':
            pos_resetCartHandler(cart);
            break;
        case 'r':
            main_receiptsPageSessionHandler();
            break;
        case 'b':
            return;
        }
    }
}

void main_receiptsPageSessionHandler()
{
    char action;
    
    while (true)
    {
        system("cls");
        display_recieptsPage();

        bannerUserInput();
        scanf("%c", &action);
        fflush(stdin);

        switch (action)
        {
        case '1':
            pos_viewReceiptHandler();
            break;
        case '2':
            pos_deleteReceiptHandler();
            break;
        
        case 'b':
            return;
        }
    }   
}

// TODO: allow editing for records
void main_reportsPageSessionHandler(struct ReportPerMonth monthlyProfits[])
{
    char action[2];

    while (true) {
        system("cls");
        display_salesReportPage(monthlyProfits);

        bannerUserInput();
        fflush(stdin);
        scanf("%s", action);
        fflush(stdin);

        if (strcmp(action, "b") == 0 || strcmp(action, "B") == 0) return;

        if (strcmp(action, "e") == 0 || strcmp(action, "E") == 0) {
            sales_editPerMonthReportsHandler(monthlyProfits);
            continue;
        } 

        int month = atoi(action) - 1;

        if (month < 0 || month > 11) {
            display_newUserMessagePage("Editing Reports", "", "Invalid input.", "", "", "", "");
            sleep(SLEEP_TIME);
            continue;
        }

        main_perDayReportsOfMonthSessionHandler(monthlyProfits, month);
    }
}

void main_perDayReportsOfMonthSessionHandler(struct ReportPerMonth monthlyProfits[], int month)
{
    char action[2];

    while (true) {
        system("cls");
        display_salesPerDayReportPage(monthlyProfits[month].day, monthlyProfits, month); 
        
        bannerUserInput();
        scanf("%s", action);
        fflush(stdin);

        if (strcmp(action, "b") == 0 || strcmp(action, "B") == 0) return;
        
        if (strcmp(action, "e") == 0 || strcmp(action, "E") == 0) {
            sales_editPerDayReportsHandler(monthlyProfits, month);
            continue;
        }
    }
}

void main_settingsPageSessionHandler()
{
    char action;

    while (true) {
        system("cls");
        display_settingsPage();

        bannerUserInput();
        scanf("%c", &action);
        fflush(stdin);

        switch (action)
        {
        case '1':
            settings_changeShopNameHandler();
            break;
        case '2':
            settings_changeShopLocationHandler();
            break;
        case 'b':
            return;
        }
    }
}