#ifndef MAIN_H
#define MAIN_H

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      HEADER FILES
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>
#include <stdbool.h>

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      CONSTANTS
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#define NAME_SIZE               30
#define ID_LENGTH               10
#define MONTHS                  12
#define SLEEP_TIME              1                               // the pause when showing information or errors before returning to previous page
#define DATE_LENGTH             30
#define MONTH_NAME_LEN          15
#define DAYS_IN_MONTH           31
#define CATEGORY_NAME_LEN       30                              // length of the name of an items category
#define MAX_CATEGORIES          30                              // maximum amount of categories
#define MAX_CART_ITEMS          15                              // maximum amount of item in the cart
#define MAX_RECEIPT_LENGTH      4096                            // the receipt is one giant string and this is the size

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      STRUCTS
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// structure of an individual item
struct Item
{
    char name[NAME_SIZE];
    int baseStocks;
    int stocks;
    double price;
    double originalPrice;
    double profit;
    char dateAdded[DATE_LENGTH];
    char lastUpdated[DATE_LENGTH];
    char id[ID_LENGTH];
    char category[CATEGORY_NAME_LEN];
};

// structure of an individual node in a linked list
struct Node
{
    struct Item data;                                           // contains the Item
    struct Node *next;                                          // a pointer that points to the next item
};

struct ReportPerDay
{
    double costs;
    double additionalCosts;
    double revenue;
    double profit;
};

// structure of sales report on each month, will be stored in array of 12(months)
struct ReportPerMonth
{
    char month[MONTH_NAME_LEN];                                 // name of the month
    struct ReportPerDay day[DAYS_IN_MONTH];                     // per day report stored in array[31]
    double costs;
    double additionalCosts;
    double revenue;
    double profit;
};

// structure of an individual item in a cart, will be stored in an array
struct CartItem 
{
    char name[NAME_SIZE];
    char itemId[ID_LENGTH];
    int quantity;
    double price;
    double profit;
};

// structure of a session cart
struct Cart 
{
    struct CartItem items[MAX_CART_ITEMS];
    int amountOfItems;
    char cartId[ID_LENGTH];
};

// settings stored in a struct, might wanna add more 
struct Settings
{
    int sortBy;                                                 // what to sort the item by. 0. Default | 1. Low Stock | 2. High Stocks | 3. Low Price | 4. High Price | 5. Low Profit | 6. High Profit
};

// global declaration of settings
struct Settings settings;
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
//      FUNCTION PROTOTYPES
//
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      FILE: main.c
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void main_initialize(struct Node **head, struct Cart *cart, struct ReportPerMonth monthlyProfits[], char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
void main_inventoryPageSessionHandler(struct Node **head, struct ReportPerMonth monthlyProfits[], char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
void main_categoriesPageSessionHandler(struct Node **head, char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
void main_posPageSessionHandler(struct Cart *cart, struct Node **head, struct ReportPerMonth monthlyProfits[]);
void main_receiptsPageSessionHandler();
void main_reportsPageSessionHandler(struct ReportPerMonth monthlyProfits[]);
void main_perDayReportsOfMonthSessionHandler(struct ReportPerMonth monthlyProfits[], int month);
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      FILE: item.c
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// handlers
void item_addItemHandler(struct Node **head, struct ReportPerMonth monthlyProfits[], char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
void item_deleteItemHandler(struct Node **head, struct ReportPerMonth monthlyProfits[]);
void item_editItemHandler(struct Node **head, char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
void item_searchItemHandler(struct Node **head);
void item_restockItemHandler(struct Node **head, struct ReportPerMonth monthlyProfits[]);
void item_changeSortingHandler(struct Node **head);
void item_viewItemDetailsHandler(struct Node **head);

void item_addItemMetaData(struct Node **head, struct ReportPerMonth monthlyProfits[], char name[], int stocks, double price, double originalPrice, char category[], double additionalCost);
struct Node *item_getItemById(struct Node **list, char itemId[]);
void item_addItemToList(struct Node **head, struct Item newItem);
void item_updateItemsCategory(struct Node **head, char *oldCategory, char *newCategory);
void item_getItemsByCategory(struct Node **head, char *category, struct Node **placeholder);
void item_getStockStatus(char *status, int stocks, int baseStocks);
void item_determineWhatToSort(char *placeholder);
void item_getSortedItems(struct Node **head, struct Node **output);
void item_sortItemsBy(bool (*callbackFn)(struct Item, struct Item), struct Node **head, struct Node **output);

// callback functions for sorting options
bool item_isStocksLessThan(struct Item item1, struct Item item2);
bool item_isStocksGreaterThan(struct Item item1,struct Item item2);

bool item_isPriceLessThan(struct Item item1, struct Item item2);
bool item_isPriceGreaterThan(struct Item item1,struct Item item2);

bool item_isProfitLessThan(struct Item item1, struct Item item2);
bool item_isProfitGreaterThan(struct Item item1,struct Item item2);

void item_insertItemAt(int index, struct Node **destination, struct Node **toInsert);
void item_regenerateItemIdList(struct Node **head);
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      FILE: utils.c
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
char* utils_getCurrentTime();
char* utils_getCurrentDate();
void utils_generateId(char placeholder[]);
int utils_getCurrentMonthInt();
int utils_getCurrentDayInt();
int utils_getListSize(struct Node **head);
void utils_freeLinkedList(struct Node **head);
void utils_clearNewline(char *string);
void utils_updateDate(char *placeholder);
void utils_joinStocks(char *output, int stock, int base);
void utils_toLowercase(char *str);
void utils_capitalizeFirst(char *str);
void utils_getStringPercentage(char *output, int number, int base);
double utils_getPercentage(int number, int base);
void utils_centerText(int length, char *text);
void utils_clearAllNewline(char *str);
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      FILE: sales.c
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void sales_initMonthlyProfits(struct ReportPerMonth monthlyProfits[]);
// total for the year
double sales_getTotalProfit(struct ReportPerMonth monthlyProfits[]);
double sales_getTotalCosts(struct ReportPerMonth monthlyProfits[]);
double sales_getTotalRevenue(struct ReportPerMonth monthlyProfits[]);
double sales_getTotalExtraCosts(struct ReportPerMonth monthlyProfits[]);

//total for the month
double sales_getTotalProfitOfMonth(struct ReportPerMonth monthlyProfits[], int month);
double sales_getTotalCostsOfMonth(struct ReportPerMonth monthlyProfits[], int month);
double sales_getTotalRevenueOfMonth(struct ReportPerMonth monthlyProfits[], int month);
double sales_getTotalExtraCostsOfMonth(struct ReportPerMonth monthlyProfits[], int month);

double sales_getProfitForMonth(struct ReportPerMonth monthlyProfits);
double sales_getProfitForDay(struct ReportPerDay day);
void sales_updateRevenue(struct ReportPerMonth monthlyProfits[], double price, double quantity);
void sales_updateProfit(struct ReportPerMonth monthlyProfits[], double profit, double quantity);
void sales_updateCosts(struct ReportPerMonth monthlyProfits[], int addedStocks, double originalPrice);
void sales_updateAdditionalCosts(struct ReportPerMonth monthlyProfits[], double additionalCosts);
void sales_reduceCosts(struct ReportPerMonth monthlyProfits[], double deduction);
void sales_updateReportsFromStorage(struct ReportPerMonth monthlyProfits[]);
void sales_reflectToMonthlyCostsOnDeletion(struct ReportPerMonth monthlyProfits[], double deduction);
void sales_editPerMonthReportsHandler(struct ReportPerMonth monthlyProfits[]);
void sales_editPerDayReportsHandler(struct ReportPerMonth monthlyProfits[], int month);
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      FILE: display.c
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// RECEIPT PAGE
void display_recieptsPage();
void display_recieptsPromptPage(char *message1, char *message2);
void display_printRecieptsHeader();
// CART PAGE
void display_pointOfSalePromptPage(struct CartItem items[], int *amountOfItems, char *message1, char *message2);
void display_pointOfSalePage(struct CartItem items[], int *amountOfItems);
void display_printCartItemsHeader();
void display_printCartItems(struct CartItem items[], int *amountOfItems);
// CATEGORY PAGE
void display_itemCategoryPage(struct Node **head);
void display_categoryPreview(char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
void display_categoryPromptPage(char categories[][CATEGORY_NAME_LEN], int *categoriesLen, char *message1, char *message2);
void display_categoryPage(char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
void display_printCategoryHeader();
void display_printCategoryList(char categories[][CATEGORY_NAME_LEN], int *categoriesLen);

void display_askUserToFullScreen();
void display_itemDataPromptPage(struct Item item, char *header, char *message1, char *message2);
void display_itemDataPage(struct Item item);
void display_newUserMessagePage(char *header, char *bottomMessage, char *message1, char *message2, char *message3, char *message4, char *message5);
void display_userMessagePage(char *header, char *bottomMessage, char *message1, char *message2, char *message3, char *message4, char *message5);
void display_printMinimumScreenHeight(int itemsLength);

// SALES REPORT PAGE
void display_salesReportPage(struct ReportPerMonth monthlyProfits[]);
void display_salesReportPromptPage(struct ReportPerMonth monthlyProfits[], char *message1, char *message2);
void display_salesPerDayReportPage(struct ReportPerDay day[], struct ReportPerMonth monthlyProfits[], int month);
void display_printPerDayReports(struct ReportPerDay day[], struct ReportPerMonth monthlyProfits[], int month);
void display_salesPerDayReportPromptPage(struct ReportPerDay day[], struct ReportPerMonth monthlyProfits[], int month, char *message1, char *message2);
void display_printMonthlyReports(struct ReportPerMonth monthlyProfits[]);
void display_printReportHeader();
void display_printMonthlyReportHeader(char *month);
// INVENTORY PAGE
void display_inventoryPromptPage(struct Node **head, char *bottomMessage1, char *bottomMessage2);
void display_inventoryPage(struct Node **head);
void display_printItemList(struct Node **head);
void display_printItemHeader();
// MENU PAGE
void display_menuPage();

void display_topBar(char *text);
void bannerFullBorder();
void bannerBlankBorderTextCen (char *text);
void bannerBlankBorderText (char *text);
void bannerBlankBorderTextLeft (char *text);
void bannerBlankBorder ();
void bannerUserInput();
void bannerFullBorderSection();
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      FILE: storage.c
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// item
void storage_addItemToStorage(struct Item item);
void storage_getItemsFromStorage(struct Node **head);
void storage_deleteItemFromStorageById(char *id);
void storage_editItemFromStorageById(char *id, struct Item item);
// reports
void storage_checkReportsFromStorage(struct ReportPerMonth monthlyProfits[]);
void storage_initializeReportsFromStorage(struct ReportPerMonth monthlyProfits[]);
void storage_getReportsFromStorage(struct ReportPerMonth monthlyProfits[]);
void storage_updatePerMonthReportDataFromStorage(int month, struct ReportPerMonth monthData);
void storage_updatePerDayDataFromStorage(char *month, int day, struct ReportPerDay dayData);
// category
void storage_addCategoryToStorage(char *category);
void storage_getCategoriesFromStorage(char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
void storage_deleteCategoryFromStorage(char *category);
void storage_editCategoryFromStorage(char *oldCategory, char *newCategory);
// receipt
void storage_addRecieptToStorage(char *reciept, char *id);
void storage_addRecieptMetaDataToStorage(char *recieptId, char *datePurchased);
void storage_printReceiptsMetaDataFromStorage(int *counter);
int storage_getReceiptFromStorageById(char *id, char *receiptBuffer);

void storage_getSettingsFromStorage();
void storage_updateSettingsFromStorage();
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      FILE: category.c
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// handlers
void category_addCategoryHandler(char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
void category_deleteCategoryHandler(char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
void category_editCategoryHandler(struct Node **head, char categories[][CATEGORY_NAME_LEN], int *categoriesLen);

void category_addCategory(char categories[][CATEGORY_NAME_LEN], int *categoriesLen, char *categoryToAdd);
bool category_isValidCategory(char categories[][CATEGORY_NAME_LEN], int *categoriesLen, char *category);
void category_itemCategoryPrompter(char *placeholder, char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
void category_viewCategoryItems(struct Node **head, char categories[][CATEGORY_NAME_LEN], int *categoriesLen);
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//      FILE: pos.c
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// handlers
void pos_addCartItemHandler(struct Cart *cart, struct Node **head);
void pos_deleteCartItemHandler(struct Cart *cart);
void pos_resetCartHandler(struct Cart *cart);
void pos_checkoutHandler(struct Cart *cart, struct Node **head, struct ReportPerMonth monthlyProfits[]);

void pos_addItemToCart(struct Cart *cart, int quantity, struct Item item);
void pos_resetCart(struct Cart *cart);
void pos_generateReceipt(struct Cart *cart, char *receiptBuffer);
void pos_saveRecieptMetaData(char *recieptId);
void pos_viewReceiptHandler();
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#endif
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
/*

    ███╗  ██╗       ███████╗         ██████╗        ██╗  ██╗                                                    ██╗  ██╗
    ████╗ ██║       ╚═██║══╝        ██╔════╝        ██║ ██╔╝                                                    ╚██╗██╔╝
    ██╔██╗██║         ██║           ██║             █████╔╝             https://github.com/FiereEinar            ╚███╔╝ 
    ██║╚████║         ██║           ██║             ██╔═██╗                                                      ██╔██╗ 
    ██║ ╚███║       ███████╗        ╚██████╗        ██║  ██╗                                                    ██╔╝ ██╗  ██╗
    ╚═╝  ╚══╝       ╚══════╝         ╚═════╝        ╚═╝  ╚═╝                                                    ╚═╝  ╚═╝  ╚═╝

*/
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
