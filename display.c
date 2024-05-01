#include "main.h"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      RECEIPT PAGE
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void display_recieptsPage()
{
    int receiptsCount = 0;

    display_topBar("Point of Sale");

    bannerBlankBorder();
    bannerBlankBorderTextCen("Receipts");
    bannerBlankBorder();

    display_printRecieptsHeader();
    bannerFullBorderSection();
    storage_printReceiptsMetaDataFromStorage(&receiptsCount);

    display_printMinimumScreenHeight(receiptsCount);
    bannerBlankBorder();

    bannerBlankBorderTextCen("1. view receipt");
    bannerBlankBorderTextCen("'b' to go back");

    bannerFullBorder();
}

void display_recieptsPromptPage(char *message1, char *message2)
{
    system("cls");
    int receiptsCount = 0;

    display_topBar("Point of Sale");

    bannerBlankBorder();
    bannerBlankBorderTextCen("Receipts");
    bannerBlankBorder();

    display_printRecieptsHeader();
    bannerFullBorderSection();
    storage_printReceiptsMetaDataFromStorage(&receiptsCount);

    display_printMinimumScreenHeight(receiptsCount);
    bannerBlankBorder();

    bannerBlankBorderTextCen(message1);
    bannerBlankBorderTextCen(message2);

    bannerFullBorder();
    bannerUserInput();
}

void display_printRecieptsHeader()
{
    printf("::  \tReceipt ID:\t\t\t\t\tDate Purchased:\t\t\t\t\t\t\t\t\t  ::\n");
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      SETTINGS PAGE
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void display_settingsPage()
{
    display_topBar("Settings");

    for (int i = 0; i < 10; i++) bannerBlankBorder();

    bannerBlankBorderTextCen("Select an action");

    bannerBlankBorder();

    bannerBlankBorderTextCen("1. Change shop name");
    bannerBlankBorderTextCen("2. Change shop location");

    for (int i = 0; i < 14; i++) bannerBlankBorder();

    bannerBlankBorderTextCen("Enter 'b' to go back.");
    bannerFullBorder();
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      CART PAGE
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void display_pointOfSalePromptPage(struct CartItem items[], int *amountOfItems, char *message1, char *message2)
{
    system("cls");
    display_topBar("Point of Sale");

    bannerBlankBorder();
    bannerBlankBorderTextCen("Items on the cart");
    bannerBlankBorder();

    display_printCartItemsHeader();
    bannerFullBorderSection();
    display_printCartItems(items, amountOfItems);

    display_printMinimumScreenHeight(*amountOfItems);
    bannerBlankBorder();

    bannerBlankBorderTextCen(message1);
    bannerBlankBorderTextCen(message2);

    bannerFullBorder();
    bannerUserInput();
}

void display_pointOfSalePage(struct CartItem items[], int *amountOfItems)
{
    display_topBar("Point of Sale");

    bannerBlankBorder();
    bannerBlankBorderTextCen("Items on the cart");
    bannerBlankBorder();

    display_printCartItemsHeader();
    bannerFullBorderSection();
    display_printCartItems(items, amountOfItems);

    display_printMinimumScreenHeight(*amountOfItems);
    bannerBlankBorder();

    bannerBlankBorderTextCen("1. Add | 2. Delete | 3. Checkout | 4. Reset");
    bannerBlankBorderTextCen("'b' to go back | 'r' to view receipts");

    bannerFullBorder();
}

void display_printCartItemsHeader()
{
    printf("::  \tName:\t\t\t\t\tQuantity:\t\t\tTotal:\t\t\tProfit:\t\t\t\t  ::\n");
}

void display_printCartItems(struct CartItem items[], int *amountOfItems)
{
    if (*amountOfItems == 0) {
        printf("::  %s\t\t\t\t\t\t\t\t\t\t\t\t\t\t  ::\n", "No items on the cart.");
        return;
    }

    double totalProfit = 0;

    for (int i = 0; i < *amountOfItems; i++) {
        double total = items[i].quantity * items[i].price;
        totalProfit +=  items[i].profit;

        printf(
            "::  %3d.%-30s\t\t%-3d\t\t\t\tP%-10.2lf\t\tP%-10.2lf\t\t\t  ::\n",
             i + 1, items[i].name, items[i].quantity, total, items[i].profit
        );
    }

    bannerFullBorderSection();
    printf("::    Total Profit: \t\t\t\t\t\t\t\t\t\t\tP%-12.2lf\t\t\t  ::\n", totalProfit);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      CATEGORY PAGE
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// renders the items in a given the category
void display_itemCategoryPage(struct Node **head)
{
    bannerFullBorder();
    bannerBlankBorder();

    display_printItemHeader();

    bannerBlankBorder();
    bannerFullBorder();

    bannerBlankBorder();
    display_printItemList(head);

    display_printMinimumScreenHeight(utils_getListSize(head) - 5);
    bannerBlankBorder();

    bannerBlankBorderTextCen("");
    bannerBlankBorderTextCen("Enter any key to go back");

    bannerFullBorder();
}

void display_categoryPreview(char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    display_topBar("Categories available");

    bannerBlankBorder();
    display_printCategoryList(categories, categoriesLen);

    display_printMinimumScreenHeight(*categoriesLen - 5);
    bannerBlankBorder();
    bannerBlankBorder();

    bannerBlankBorderTextCen("Enter the index of category for this item, enter 'n' to set it to none");

    bannerFullBorder();
}

void display_categoryPromptPage(char categories[][CATEGORY_NAME_LEN], int *categoriesLen, char *message1, char *message2)
{
    system("cls");
    bannerFullBorder();
    bannerBlankBorder();

    display_printCategoryHeader();

    bannerBlankBorder();
    bannerFullBorder();

    bannerBlankBorder();
    display_printCategoryList(categories, categoriesLen);

    display_printMinimumScreenHeight(*categoriesLen - 5);
    bannerBlankBorder();

    bannerBlankBorderTextCen(message1);
    bannerBlankBorderTextCen(message2);

    bannerFullBorder();
    bannerUserInput();
}

void display_categoryPage(char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    bannerFullBorder();
    bannerBlankBorder();

    display_printCategoryHeader();

    bannerBlankBorder();
    bannerFullBorder();

    bannerBlankBorder();
    display_printCategoryList(categories, categoriesLen);

    display_printMinimumScreenHeight(*categoriesLen - 5);
    bannerBlankBorder();

    bannerBlankBorderTextCen("1. Add | 2. Delete | 3. Edit | 4. View");
    bannerBlankBorderTextCen("'b' to go back to Items Page");

    bannerFullBorder();
}

void display_printCategoryHeader()
{
    printf("::\tIndex\t\t\tCategory Name\t\t\t\t\t\t\t\t\t\t\t\t  ::\n");
}

void display_printCategoryList(char categories[][CATEGORY_NAME_LEN], int *categoriesLen)
{
    if (*categoriesLen == 0) {
        printf("::  %s\t\t\t\t\t\t\t\t\t\t\t\t\t\t  ::\n", "No categories available.");
        return;
    }
    for (int i = 0; i < *categoriesLen; i++) {
        printf("::  \t%-2d\t\t\t%-30s\t\t\t\t\t\t\t\t\t\t  ::\n", i + 1, categories[i]);
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// the layout breaks if the terminal is not full screen
void display_askUserToFullScreen()
{
    system("cls");
    printf("\n\n\n\n");
    printf("::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    printf("::                                                                          ::\n");
    printf("::                                                                          ::\n");
    printf("::              Please ensure that the window is at full screen             ::\n");
    printf("::                                                                          ::\n");
    printf("::                        Enter any key to continue                         ::\n");
    printf("::                                                                          ::\n");
    printf("::                                                                          ::\n");
    printf("::                                                                          ::\n");
    printf("::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    bannerUserInput();
    getch();
}

void display_itemDataPromptPage(struct Item item, char *header, char *message1, char *message2)
{
    system("cls");
    display_topBar(header);
    bannerBlankBorder();

    char stocksStr[20];
    utils_joinStocks(stocksStr, item.stocks, item.baseStocks);
    
    printf("::  Item Name:\t\t\t\t\t\t\t%-30s\t\t\t\t\t\t  ::\n", item.name);
    printf("::  Category:\t\t\t\t\t\t\t%-20s\t\t\t\t\t\t\t  ::\n", item.category);
    printf("::  Stocks:\t\t\t\t\t\t\t%-20s\t\t\t\t\t\t\t  ::\n", stocksStr);
    printf("::  Selling Price:\t\t\t\t\t\t%-20.2lf\t\t\t\t\t\t\t  ::\n", item.price);
    printf("::  Original Price:\t\t\t\t\t\t%-20.2lf\t\t\t\t\t\t\t  ::\n", item.originalPrice);
    printf("::  Profit per item:\t\t\t\t\t\t%-20.2lf\t\t\t\t\t\t\t  ::\n", item.profit);
    printf("::  Date Added:\t\t\t\t\t\t\t%-15s\t\t\t\t\t\t\t  ::\n", item.dateAdded);
    printf("::  Last Updated:\t\t\t\t\t\t%-15s\t\t\t\t\t\t\t  ::\n", item.lastUpdated);
    printf("::  Item ID:\t\t\t\t\t\t\t%-15s\t\t\t\t\t\t\t\t  ::\n", item.id);

    display_printMinimumScreenHeight(2);
    bannerBlankBorderTextCen(message1);
    bannerBlankBorderTextCen(message2);
    bannerFullBorder();
    bannerUserInput();
}

// the option 6. in inventory page to view the details of an item
void display_itemDataPage(struct Item item)
{
    display_topBar("Details of an item");
    bannerBlankBorder();

    char stocksStr[20];
    utils_joinStocks(stocksStr, item.stocks, item.baseStocks);
    
    printf("::  Item Name:\t\t\t\t\t\t\t%-30s\t\t\t\t\t\t  ::\n", item.name);
    printf("::  Category:\t\t\t\t\t\t\t%-20s\t\t\t\t\t\t\t  ::\n", item.category);
    printf("::  Stocks:\t\t\t\t\t\t\t%-20s\t\t\t\t\t\t\t  ::\n", stocksStr);
    printf("::  Selling Price:\t\t\t\t\t\t%-20.2lf\t\t\t\t\t\t\t  ::\n", item.price);
    printf("::  Original Price:\t\t\t\t\t\t%-20.2lf\t\t\t\t\t\t\t  ::\n", item.originalPrice);
    printf("::  Profit per item:\t\t\t\t\t\t%-20.2lf\t\t\t\t\t\t\t  ::\n", item.profit);
    printf("::  Date Added:\t\t\t\t\t\t\t%-15s\t\t\t\t\t\t\t  ::\n", item.dateAdded);
    printf("::  Last Updated:\t\t\t\t\t\t%-15s\t\t\t\t\t\t\t  ::\n", item.lastUpdated);
    printf("::  Item ID:\t\t\t\t\t\t\t%-15s\t\t\t\t\t\t\t\t  ::\n", item.id);

    display_printMinimumScreenHeight(3);
    bannerBlankBorderTextCen("Enter 'b' to go back.");
    bannerFullBorder();
}

// this page is mostly used in prompting the user for action or showing messages to users
void display_newUserMessagePage(char *header, char *bottomMessage, char *message1, char *message2, char *message3, char *message4, char *message5)
{
    system("cls");
    display_userMessagePage(header, bottomMessage, message1, message2, message3, message4, message5);
    bannerUserInput();
}

void display_userMessagePage(char *header, char *bottomMessage, char *message1, char *message2, char *message3, char *message4, char *message5)
{
    display_topBar(header);

    for (int i = 0; i < 11; i++) bannerBlankBorder();

    bannerBlankBorderTextCen(message1);
    bannerBlankBorderTextCen(message2);
    bannerBlankBorderTextCen(message3);
    bannerBlankBorderTextCen(message4);
    bannerBlankBorderTextCen(message5);

    for (int i = 0; i < 12; i++) bannerBlankBorder();

    bannerBlankBorderTextCen(bottomMessage);
    bannerFullBorder();
}

// prints the necessary amount of spaces to keep the same height of different pages
void display_printMinimumScreenHeight(int itemsLength)
{
    int min = 20;

    if (itemsLength >= min) return;
    for (int i = 0; i < min - itemsLength; i++) bannerBlankBorder();
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      SALES REPORT PAGE
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// per month report
void display_salesReportPage(struct ReportPerMonth monthlyProfits[])
{
    bannerFullBorder();
    bannerBlankBorder();
    display_printReportHeader();
    bannerFullBorderSection();

    display_printMonthlyReports(monthlyProfits);
    display_printMinimumScreenHeight(MONTHS - 6);

    bannerBlankBorderTextCen("Enter the number of the month to view more details");
    bannerBlankBorderTextCen("'e' Edit | 'b' Go back");
    bannerFullBorder();
}

void display_salesReportPromptPage(struct ReportPerMonth monthlyProfits[], char *message1, char *message2)
{
    system("cls");
    bannerFullBorder();
    bannerBlankBorder();
    display_printReportHeader();
    bannerFullBorderSection();

    display_printMonthlyReports(monthlyProfits);
    display_printMinimumScreenHeight(MONTHS - 6);

    bannerBlankBorderTextCen(message1);
    bannerBlankBorderTextCen(message2);
    bannerFullBorder();
    bannerUserInput();
}

// per day report
void display_salesPerDayReportPage(struct ReportPerDay day[], struct ReportPerMonth monthlyProfits[], int month)
{
    bannerFullBorder();
    bannerBlankBorder();
    display_printMonthlyReportHeader(monthlyProfits[month].month);
    bannerFullBorderSection();

    display_printPerDayReports(day, monthlyProfits, month);
    display_printMinimumScreenHeight(DAYS_IN_MONTH - 1);

    bannerBlankBorder();
    bannerBlankBorderTextCen("'e' Edit | 'b' Go back");
    bannerFullBorder();
}

void display_salesPerDayReportPromptPage(struct ReportPerDay day[], struct ReportPerMonth monthlyProfits[], int month, char *message1, char *message2)
{
    system("cls");
    bannerFullBorder();
    bannerBlankBorder();
    display_printMonthlyReportHeader(monthlyProfits[month].month);
    bannerFullBorderSection();

    display_printPerDayReports(day, monthlyProfits, month);
    display_printMinimumScreenHeight(DAYS_IN_MONTH);

    // bannerBlankBorder();
    bannerBlankBorderTextCen(message1);
    bannerBlankBorderTextCen(message2);
    bannerFullBorder();
    bannerUserInput();
}

// each individual month report
void display_printMonthlyReports(struct ReportPerMonth monthlyProfits[])
{
    for (int i = 0; i < MONTHS; i++)
    {
        printf
        (
            "::  %d. %-15s\t\t\t\tP%-10.2lf\t\tP%-10.2lf\t\tP%-10.2lf\t\tP%-10.2lf\t  ::\n", 
            i + 1, monthlyProfits[i].month, monthlyProfits[i].costs, monthlyProfits[i].additionalCosts, monthlyProfits[i].revenue, sales_getProfitForMonth(monthlyProfits[i])
        );
    }
    bannerFullBorderSection();

    // the total at the bottom
    printf
    (
        "::  Total\t\t\t\t\tP%-10.2lf\t\tP%-10.2lf\t\tP%-10.2lf\t\tP%-10.2lf\t  ::\n", 
        sales_getTotalCosts(monthlyProfits), sales_getTotalExtraCosts(monthlyProfits), sales_getTotalRevenue(monthlyProfits), sales_getTotalProfit(monthlyProfits)
    );
}

// each individual day report
void display_printPerDayReports(struct ReportPerDay day[], struct ReportPerMonth monthlyProfits[], int month)
{
    for (int i = 0; i < DAYS_IN_MONTH; i++)
    {
        printf
        (
            "::  %-4d  \t\t\t\t\tP%-10.2lf\t\tP%-10.2lf\t\tP%-10.2lf\t\tP%-10.2lf\t  ::\n", 
            i + 1, day[i].costs, day[i].additionalCosts, day[i].revenue, sales_getProfitForDay(day[i])
        );
    }
    bannerFullBorderSection();

    // the total at the bottom
    printf
    (
        "::  Total\t\t\t\t\tP%-10.2lf\t\tP%-10.2lf\t\tP%-10.2lf\t\tP%-10.2lf\t  ::\n", 
        sales_getTotalCostsOfMonth(monthlyProfits, month), sales_getTotalExtraCostsOfMonth(monthlyProfits, month), sales_getTotalRevenueOfMonth(monthlyProfits, month), sales_getTotalProfitOfMonth(monthlyProfits, month)
    );
}

void display_printMonthlyReportHeader(char *month)
{
    printf("::  %s\t\t\t\t\tCosts\t\t\tExtra Costs\t\tRevenue\t\t\tProfit\t\t  ::\n", month);
}

void display_printReportHeader()
{
    printf("::  Month\t\t\t\t\tCosts\t\t\tExtra Costs\t\tRevenue\t\t\tProfit\t\t  ::\n");
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      INVENTORY PAGE
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void display_inventoryPromptPage(struct Node **head, char *bottomMessage1, char *bottomMessage2)
{
    system("cls");

    struct Node *sorted = NULL;
    item_getSortedItems(head, &sorted);

    bannerFullBorder();
    bannerBlankBorder();
    display_printItemHeader();
    bannerBlankBorder();
    bannerFullBorder();
    bannerBlankBorder();
    display_printItemList(&sorted);
    // utils_freeLinkedList(&sorted);

    display_printMinimumScreenHeight(utils_getListSize(head) - 5);
    bannerBlankBorder();

    bannerBlankBorderTextCen(bottomMessage1);
    bannerBlankBorderTextCen(bottomMessage2);

    bannerFullBorder();
    bannerUserInput();
}

// renders the items in the inventory
void display_inventoryPage(struct Node **head)
{
    char sortedBy[30], text[15];
    item_determineWhatToSort(text);
    sprintf(sortedBy, "Sorted by: %s", text);

    struct Node *sorted = NULL;
    item_getSortedItems(head, &sorted);

    bannerFullBorder();
    bannerBlankBorder();
    display_printItemHeader();
    bannerBlankBorder();
    bannerFullBorder();
    bannerBlankBorder();
    display_printItemList(&sorted);
    // utils_freeLinkedList(&sorted);

    display_printMinimumScreenHeight(utils_getListSize(head) - 5);
    bannerBlankBorderTextCen(sortedBy);
    // bannerBlankBorder();

    bannerBlankBorderTextCen("1. Add | 2. Delete | 3. Edit | 4. Restock | 5. Search | 6. Item Details");
    bannerBlankBorderTextCen("'b' Go Back | 'c' Categories Page | 's' Sort Items");

    bannerFullBorder();
}

void display_printItemHeader()
{
    printf(
        "::  %-10s %-30s\t %-30s %-10s\t %-12s %-12s %-13s  ::\n",
        "ID", "Item", "Category", "Stocks", "Price", "Profit", "Status"
    );
}

// each individual item in the inventory
void display_printItemList(struct Node **head)
{
    struct Node *current = *head;
    int i = 1;

    if (current == NULL)
    {
        printf("::  No items in the inventory.\t\t\t\t\t\t\t\t\t\t\t\t\t\t  ::\n");
        return;
    }
    // loop over the entire list and print the data on each iteration
    while (current != NULL)
    {
        char stocksPercentage[12];
        char stocksStatus[12];
        // utils_getStringPercentage(stocksPercentage, current->data.stocks, current->data.baseStocks);
        utils_joinStocks(stocksPercentage, current->data.stocks, current->data.baseStocks);
        item_getStockStatus(stocksStatus, current->data.stocks, current->data.baseStocks);

        printf(
            "::  %-10s %-30s\t %-30s %-10s\t P%-11.2lf P%-11.2lf %-13s  ::\n",
            current->data.id, current->data.name, current->data.category, stocksPercentage, current->data.price, current->data.profit, stocksStatus
        );

        // iterator
        current = current->next;
        i++;
    }
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// renders the main menu
void display_menuPage()
{
    display_topBar("Inventory Management System");

    for (int i = 0; i < 10; i++) bannerBlankBorder();

    bannerBlankBorderTextCen("Select a page by number");

    bannerBlankBorder();

    bannerBlankBorderTextCen("1. Inventory Page");
    bannerBlankBorderTextCen("2. Point of Sale Page");
    bannerBlankBorderTextCen("3. Sales Reports");
    bannerBlankBorderTextCen("4. Settings");

    for (int i = 0; i < 12; i++) bannerBlankBorder();

    bannerBlankBorderTextCen("Type 'q' to quit.");
    bannerFullBorder();
}

// the top bars of each pages
void display_topBar(char *text) {
    bannerFullBorder();

    bannerBlankBorder();
    bannerBlankBorderTextCen(text);
    bannerBlankBorder();

    bannerFullBorder();
}

// prints a full line
void bannerFullBorder () {
    for ( int i = 0; i < 14; i++ )
        printf ("::::::::::");
    printf ("\n");
}

// prints a text at the center with borders
void bannerBlankBorderTextCen (char *text) {
    // calculate the padding needed to center
    int length = strlen (text) - 1;  // Remove the terminal '\0'
    int pad = (length >= 134) ? 0 : ((134 - length) / 2);

    printf (":: %*s%s%*s ::\n", pad, " ", text, 133 - pad - length, " ");
}

// prints a text at the center with borders
void bannerBlankBorderText (char *text) {
    printf (":: %134s ::\n", text);
}

// prints a text at the left with borders
void bannerBlankBorderTextLeft (char *text) {
    printf (":: %-134s ::\n", text);
}

// prints a blank line with borders
void bannerBlankBorder () {
    printf (":: %134s ::\n", "");
}

// prints the (>>>) for accepting user input
void bannerUserInput () {
    printf ("\n>>> ");
}

// prints a full line
void bannerFullBorderSection () {
    printf (":: ______________________________________________________________________________________________________________________________________ ::\n");
}
